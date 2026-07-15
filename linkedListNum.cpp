//
// Created by PC on 7/6/2026.
//

#include "linkedListNum.hpp"

#include <iostream>

#include "utils.hpp"

linkedListNum::linkedListNum() : readerLock(1), readerCount(0), head(nullptr), totalTasks(0), totalTimeTaken(0) {}
void linkedListNum::put(int value) {
    std::unique_lock dataAccessLock(writerLock);
    auto *newNode = new numNode{value, nullptr};
    if (head == nullptr) {
        head = newNode;
        dataAccessLock.unlock();
        std::cout << "Adding to head: " << value << "\n";
        increaseTasks();
        return;
    }
    auto curr = head;
    while (curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = newNode;
    dataAccessLock.unlock();
    increaseTasks();
    std::cout << "Adding: " << value << "\n";
}
void linkedListNum::findAndPop(int value) {
    std::unique_lock dataAccessLock(writerLock);
    auto curr = head;
    numNode *prev = nullptr;
    while (curr && curr->value != value) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == nullptr) {
        dataAccessLock.unlock();
        std::cout << "Nothing to pop\n";
        increaseTasks();
        return;
    }
    if (prev == nullptr) {
        head = curr->next;
    } else {
        prev->next = curr->next;
    }
    delete curr;
    dataAccessLock.unlock();
    increaseTasks();
    std::cout << "Remove value: " << value << "\n";
}
bool linkedListNum::find(int value) {
    readerLock.acquire();
    readerCount++;
    if (readerCount == 1) {
        writerLock.lock();
    }
    readerLock.release();
    auto curr = head;
    while (curr && curr->value != value) {
        curr = curr->next;
    }
    if (curr == nullptr) {
        readerLock.acquire();
        readerCount--;
        if (readerCount == 0) {
            writerLock.unlock();
        }
        readerLock.release();
        increaseTasks();
        return false;
    }
    readerLock.acquire();
    readerCount--;
    if (readerCount == 0) {
        writerLock.unlock();
    }
    readerLock.release();
    increaseTasks();
    return true;
}
void linkedListNum::test(threadManager &manager, int time) {
    auto start = std::chrono::steady_clock::now();
    while (true) {
        auto randVal = utils::getRandom(0, 1);
        if (randVal == 0) {
            manager.submit(new std::function([this]() { this->findAndPop(utils::getRandom(1, 100)); }));
        } else if (randVal == 1) {
            manager.submit(new std::function([this]() { this->put(utils::getRandom(1, 100)); }));
        } else {
            manager.submit(new std::function([this]() { this->find(utils::getRandom(1, 100)); }));
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if (elapsedMs > time) {
            totalTimeTaken = elapsedMs;
            return;
        }
    }
}

void linkedListNum::getStats() {
    std::cout << "Total tasks: " << totalTasks << "\n";
    std::cout << "Total time taken : " << totalTimeTaken << "\n";
}
void linkedListNum::increaseTasks() {
    std::unique_lock tasksLock(statsLock);
    totalTasks++;
}

linkedListNum::~linkedListNum() {
    std::unique_lock dataAccessLock(writerLock);
    while (head != nullptr) {
        numNode *temp = head;
        head = head->next;
        delete temp;
    }
    dataAccessLock.unlock();
}
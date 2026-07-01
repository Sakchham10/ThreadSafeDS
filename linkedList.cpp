#include "linkedList.hpp"
#include <iostream>
#include "threadManager.hpp"
#include "utils.hpp"

void linkedList::insertNode(int value) {
    std::unique_lock activationLock(tManager->threadActivationLock);
    tManager->cv.wait(activationLock, [this] {
        return tManager->shouldThreadsBeLocked == false;
    });
    activationLock.unlock();
    std::unique_lock accessLock(dataAccessLock);
    node *newNode = new node{value, nullptr};
    if (head == nullptr) {
        head = newNode;
        accessLock.unlock();
        return;
    }
    node *curr = head;
    while (curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = newNode;
    accessLock.unlock();
}

void linkedList::deleteNode(int value) {
    std::unique_lock activationLock(tManager->threadActivationLock);
    tManager->cv.wait(activationLock, [this] {
        return tManager->shouldThreadsBeLocked == false;
    });
    activationLock.unlock();
    std::unique_lock accessLock(dataAccessLock);
    if (head == nullptr) {
        std::cout << "No node in linkedlist. Delete aborted.\n";
        accessLock.unlock();
        return;
    }
    node *curr = head;
    node *prev = nullptr;
    while (curr != nullptr && curr->value != value) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == nullptr) {
        std::cout << "Delete aborted,value not found\n";
        accessLock.unlock();
        return;
    }
    if (prev == nullptr) {
        std::cout << "Deleting the head\n";
        node *temp = head;
        head = head->next;
        delete temp;
        accessLock.unlock();
        return;
    }
    prev->next = curr->next;
    std::cout << "Deleting the value " << value << "\n";
    delete curr;
    accessLock.unlock();
}

bool linkedList::findNode(int value) {
    std::unique_lock activationLock(tManager->threadActivationLock);
    tManager->cv.wait(activationLock, [this] {
        return tManager->shouldThreadsBeLocked == false;
    });
    activationLock.unlock();
    std::unique_lock accessLock(dataAccessLock);
    if (head == nullptr) {
        std::cout << "No node in linkedlist. Can't find anything.\n";
        accessLock.unlock();
        return false;
    }
    node *curr = head;
    while (curr != nullptr && curr->value != value) {
        curr = curr->next;
    }
    if (curr == nullptr) {
        std::cout << value << " not found. Find aborted.\n";
        accessLock.unlock();
        return false;
    }
    accessLock.unlock();
    return true;
}

bool linkedList::findAndReplaceNode(int currValue, int value) {
    std::unique_lock activationLock(tManager->threadActivationLock);
    tManager->cv.wait(activationLock, [this] {
        return tManager->shouldThreadsBeLocked == false;
    });
    activationLock.unlock();
    std::unique_lock accessLock(dataAccessLock);
    if (head == nullptr) {
        std::cout << "No node in linkedlist, aborting replace\n";
        accessLock.unlock();
        return false;
    }
    node *curr = head;
    while (curr != nullptr && curr->value != currValue) {
        curr = curr->next;
    }
    if (curr == nullptr) {
        std::cout << currValue << " not found. Replace aborted\n";
        accessLock.unlock();
        return false;
    }
    curr->value = value;
    std::cout << currValue << " replaced with " << value << "\n";
    accessLock.unlock();
    return true;
}

void linkedList::init(int startRange, int endRange) {
    //inserts in a predictable manner to see the value first. So no multithreading here.
    tManager->unlockAllThreads();
    for (int i = startRange; i <= endRange; i++) {
        this->insertNode(i);
    }
    std::cout << "Completed intializtion.\nLL starts at: " << startRange << "\nLL ends at: " << endRange << "\n";
    this->startRange = startRange;
    this->endRange = endRange;
    tManager->lockAllThreads();
}

void linkedList::startTest(int timeToRun, int totalThreads) {
    auto start = std::chrono::steady_clock::now();
    while (true) {
        auto diff = std::chrono::steady_clock::now() - start;
        long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        tManager->clearThreads();
        tManager->lockAllThreads();
        if (elapsed > timeToRun) {
            return;
        }
        for (int i = 0; i < totalThreads; i++) {
            int randomNumber = utils::getRandom(0, 3);
            if (randomNumber == 0) {
                std::function func = [this] {
                    this->insertNode(utils::getRandom(startRange, endRange));
                };
                tManager->runTask(func);
            } else if (randomNumber == 1) {
                std::function func = [this] {
                    this->deleteNode(utils::getRandom(startRange, endRange));
                };
                tManager->runTask(func);
            } else if (randomNumber == 2) {
                std::function func = [this] {
                    this->findNode(utils::getRandom(startRange, endRange));
                };
                tManager->runTask(func);
            } else {
                std::function func = [this] {
                    this->findAndReplaceNode(utils::getRandom(startRange, endRange),
                                             utils::getRandom(startRange, endRange));
                };
                tManager->runTask(func);
            }
        }
        tManager->unlockAllThreads();
        tManager->joinAllThreads();
    }
}

linkedList::linkedList(threadManager *tManager) {
    head = nullptr;
    this->tManager = tManager;
}

linkedList::~linkedList() {
    while (head != nullptr) {
        node *temp = head;
        head = temp->next;
        delete temp;
    }
}
//
// Created by Sakchham Sangroula on 6/28/26.
//

#include "threadManager.hpp"

threadManager::threadManager() {
    threads = std::vector<std::thread>();
}

void threadManager::unlockAllThreads() {
    std::unique_lock lock(threadActivationLock);
    shouldThreadsBeLocked = false;
    cv.notify_all();
    threadActivationLock.unlock();
}

void threadManager::lockAllThreads() {
    std::unique_lock lock(threadActivationLock);
    shouldThreadsBeLocked = true;
    threadActivationLock.unlock();
}

void threadManager::runTask(std::function<void()> func) {
    threads.emplace_back(func);
}

void threadManager::clearThreads() {
    threads.clear();
}

void threadManager::joinAllThreads() {
    for (int i = 0; i < this->threads.size(); i++) {
        threads[i].join();
    }
}
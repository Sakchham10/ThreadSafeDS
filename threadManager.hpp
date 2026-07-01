//
// Created by Sakchham Sangroula on 6/28/26.
//

#ifndef THREADSAFEDATASTRUCTURE_THREADMANAGER_HPP
#define THREADSAFEDATASTRUCTURE_THREADMANAGER_HPP
#include <thread>
#include <vector>

class threadManager {
    std::vector<std::thread> threads;
    int totalThreads;

public:
    bool shouldThreadsBeLocked = false;

    std::mutex threadActivationLock;

    std::condition_variable cv;

    threadManager();

    void unlockAllThreads();

    void lockAllThreads();

    void runTask(std::function<void()> func);

    void clearThreads();

    void joinAllThreads();
};


#endif //THREADSAFEDATASTRUCTURE_THREADMANAGER_HPP
// Created by PC on 7/6/2026.
//

#ifndef THREADSAFEDATASTRUCTURE_LINKEDLISTNUM_HPP
#define THREADSAFEDATASTRUCTURE_LINKEDLISTNUM_HPP
#include <mutex>
#include <semaphore>

#include "threadManager.hpp"


struct numNode {
    int value;
    numNode *next;
};

class linkedListNum {
    std::mutex writerLock;
    std::mutex statsLock;
    std::binary_semaphore readerLock;
    int readerCount;
    numNode *head;
    int totalTasks;
    long long totalTimeTaken;
    void increaseTasks();

public:
    void put(int value);
    void findAndPop(int value);
    void test(threadManager &thrdMngr, int time);
    void getStats();
    bool find(int value);
    linkedListNum();
    ~linkedListNum();
};


#endif // THREADSAFEDATASTRUCTURE_LINKEDLISTNUM_HPP
#ifndef THREADSAFEDATASTRUCTURE_LINKEDLIST_HPP
#define THREADSAFEDATASTRUCTURE_LINKEDLIST_HPP
#include <mutex>
#include <semaphore>

#include "threadManager.hpp"


struct node {
   int value;
   node *next;
};

class linkedList {
public:
   threadManager *tManager;

   void init(int startRange, int endRange);

   linkedList(threadManager *tManager);

   ~linkedList();

   void startTest(int timeToRun, int totalThreads);

private:
   node *head;
   std::binary_semaphore readerSemaphore;
   std::binary_semaphore writerSemaphore;
   int totalTimeToRun;
   int totalOperationsCompleted;
   int startRange;
   int endRange;
   int readerCount;

   void insertNode(int value);

   void deleteNode(int value);

   bool findNode(int value);

   bool findAndReplaceNode(int currValue, int value);
};
#endif
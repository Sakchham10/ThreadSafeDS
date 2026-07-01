#include "linkedList.hpp"
#include "threadManager.hpp"

int main() {
    threadManager tManager = threadManager();
    linkedList ll = linkedList(&tManager);
    ll.init(10, 100);
    ll.startTest(100, 10);
}

//linkedList will take the threadManger and use the threads there?
//each object will take from a pool of threadManger.
//
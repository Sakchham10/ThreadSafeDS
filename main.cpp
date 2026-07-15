#include <iostream>
#include "linkedListNum.hpp"
#include "threadManager.hpp"

int main() {
    threadManager manager = threadManager(1);
    linkedListNum myList = linkedListNum();
    try {
        myList.test(manager, 4000);
        manager.shutDown();
    } catch (std::runtime_error &e) {
        manager.shutDown();
    }
    myList.getStats();
}
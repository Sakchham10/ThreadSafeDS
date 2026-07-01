cmake_minimum_required(VERSION 4.1)
project(ThreadSafeDataStructure)

set(CMAKE_CXX_STANDARD 20)

add_executable(ThreadSafeDataStructure main.cpp
        linkedList.cpp
        linkedList.hpp
        queue.cpp
        queue.hpp
        threadManager.hpp
        threadManager.cpp
        utils.cpp
        utils.hpp
)
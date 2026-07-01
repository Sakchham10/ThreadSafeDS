# Thread-Safe Data Structures in C++

This repository is an educational project for implementing basic data
structures in a thread-safe manner while exploring multithreading in C++.

The goal is to experiment with concepts such as:

- Creating and coordinating multiple threads
- Protecting shared state with mutexes
- Synchronizing work with condition variables
- Avoiding data races during concurrent operations
- Stress-testing data structures with randomized operations

## Current Status

The project currently includes:

- A linked list supporting insert, delete, find, and replace operations
- A thread manager used to start, synchronize, and join worker threads
- A randomized concurrent test workload
- A queue scaffold for a future thread-safe queue implementation

The linked list currently uses a single mutex to protect access to the entire
structure. This is intentionally a learning-oriented implementation rather
than a production-ready concurrent container.

## Requirements

- A C++20-compatible compiler
- CMake 4.1 or newer

## Build and Run

```sh
cmake -S . -B build
cmake --build build
./build/ThreadSafeDataStructure
```

The executable initializes a linked list and runs concurrent randomized
operations against it.

## Project Direction

Future work may include implementing additional data structures, improving
lock granularity, comparing synchronization strategies, and adding more
repeatable concurrency tests.

cmake_minimum_required(VERSION 3.26)
project(Gator_Library)

set(CMAKE_CXX_STANDARD 17)

add_executable(Gator_Library main.cpp
        RedBlackTree.cpp
        MinHeap.cpp
        Book.cpp
        Node.cpp
        Reservation.cpp)

//
// Created by Aseem Baranwal on 11/5/23.
//

#ifndef GATOR_LIBRARY_MINHEAP_H
#define GATOR_LIBRARY_MINHEAP_H

#include "Reservation.h"
#include <vector>
#include <iostream>
#include <unordered_map>

class MinHeap {
private:
    int waitlistSize;

    static int parent(int idx);
    static int leftChild(int idx);
    static int rightChild(int idx);
    void heapifyUp(int idx);
    void heapifyDown(int idx);

public:
    std::vector<Reservation *> heap;
    std::unordered_map<int, int> userIDToIndex;

    MinHeap();

    void insertReservation(Reservation *reservation);

    int deleteReservation();
};

#endif //GATOR_LIBRARY_MINHEAP_H

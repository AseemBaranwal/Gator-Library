//
// Created by Aseem Baranwal on 11/12/23.
//

#include "MinHeap.h"

int MinHeap::parent(int idx) {
    return (idx - 1) / 2;
}

int MinHeap::leftChild(int idx) {
    return 2 * idx + 1;
}

int MinHeap::rightChild(int idx) {
    return 2 * idx + 2;
}

void MinHeap::heapifyUp(int idx) {
    if (idx > 0 and
        (heap[parent(idx)]->priority > heap[idx]->priority or
         (heap[parent(idx)]->priority == heap[idx]->priority and
          heap[parent(idx)]->timestamp > heap[idx]->timestamp))) {
        std::swap(heap[idx], heap[parent(idx)]);
        userIDToIndex[heap[idx]->userID] = idx;
        userIDToIndex[heap[parent(idx)]->userID] = parent(idx);
        heapifyUp(parent(idx));
    }
}

void MinHeap::heapifyDown(int idx) {
    int minIdx = idx;
    if (leftChild(idx) < (int)heap.size() and
        (heap[leftChild(idx)]->priority < heap[minIdx]->priority or
         (heap[leftChild(idx)]->priority == heap[minIdx]->priority and
          heap[leftChild(idx)]->timestamp < heap[minIdx]->timestamp))) {
        minIdx = leftChild(idx);
    }
    if (rightChild(idx) < (int)heap.size() and
        (heap[rightChild(idx)]->priority < heap[minIdx]->priority or
         (heap[rightChild(idx)]->priority == heap[minIdx]->priority and
          heap[rightChild(idx)]->timestamp < heap[minIdx]->timestamp))) {
        minIdx = rightChild(idx);
    }
    if (minIdx != idx) {
        std::swap(heap[idx], heap[minIdx]);
        userIDToIndex[heap[idx]->userID] = idx;
        userIDToIndex[heap[minIdx]->userID] = minIdx;
        heapifyDown(minIdx);
    }
}

MinHeap::MinHeap() {
    heap = std::vector<Reservation *>();
    userIDToIndex = std::unordered_map<int, int>();
    waitlistSize = 0;
}

void MinHeap::insertReservation(Reservation *reservation) {
    heap.push_back(reservation);
    userIDToIndex[reservation->userID] = static_cast<int>(heap.size()) - 1;
    heapifyUp(static_cast<int>(heap.size()) - 1);
    waitlistSize++;
}

int MinHeap::deleteReservation() {
    if (heap.empty()) {
        std::cout << "Heap is empty" << std::endl;
        return -1;
    }
    int userID = heap[0]->userID;
    userIDToIndex.erase(userID);
    std::swap(heap[0], heap[heap.size() - 1]);
    heap.pop_back();
    waitlistSize--;
    heapifyDown(0);
    return userID;
}

//
// Created by Aseem Baranwal on 11/5/23.
//

#ifndef GATOR_LIBRARY_NODE_H
#define GATOR_LIBRARY_NODE_H

#include "Book.h"
#include "MinHeap.h"

// RBTree Node
class Node {
public:
    Node *left, *right;
    Node *parent;
    bool isRed;
    Book *book; // Book Details
    MinHeap *minHeap; // MinHeap for the reservations

    Node();
    void printBook();
};

#endif //GATOR_LIBRARY_NODE_H

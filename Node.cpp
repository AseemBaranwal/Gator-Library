//
// Created by Aseem Baranwal on 11/12/23.
//

#include "Node.h"
#include <iostream>

Node::Node() {
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    this->isRed = true;
    this->book = nullptr;
    this->minHeap = new MinHeap();
}

void Node::printBook() {
    std::cout << "BookID = " << this->book->bookID << std::endl;
    std::cout << "Title = \"" << this->book->bookName << "\"" << std::endl;
    std::cout << "Author = \"" << this->book->bookAuthor << "\"" << std::endl;
    std::cout << "Availability = \"" << (this->book->isAvailable ? "Yes" : "No") << "\"" << std::endl;
    std::cout << "BorrowedBy = " << (this->book->isAvailable ? "None" : std::to_string(this->book->borrowerID))
              << std::endl;
    std::cout << "Reservations = [";
    if (!this->minHeap->heap.empty()) {
        for (int j = 0; j < this->minHeap->heap.size() - 1; j++) {
            std::cout << (this->minHeap->heap[j]->userID) << ", ";
        }
        std::cout << (this->minHeap->heap.back()->userID);
    }
    std::cout << "]" << std::endl << std::endl;
}

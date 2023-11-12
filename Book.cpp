//
// Created by Aseem Baranwal on 11/5/23.
//

#include "Book.h"

Book::Book() {
    this->bookID = 0;
    this->bookName = "";
    this->bookAuthor = "";
    this->isAvailable = true;
    this->borrowerID = 0;
}

Book::Book(int bookID, const std::string &bookName, const std::string &bookAuthor, bool isAvailable) {
    this->bookID = bookID;
    this->bookName = bookName;
    this->bookAuthor = bookAuthor;
    this->isAvailable = isAvailable;
    this->borrowerID = 0;
}

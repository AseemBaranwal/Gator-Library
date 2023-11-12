//
// Created by Aseem Baranwal on 11/5/23.
//

#ifndef GATOR_LIBRARY_BOOK_H
#define GATOR_LIBRARY_BOOK_H

#include <string>

struct Book {
    int bookID;
    std::string bookName;
    std::string bookAuthor;
    bool isAvailable;
    int borrowerID;

    Book();

    Book(int bookID, const std::string &bookName, const std::string &bookAuthor, bool isAvailable);

};

#endif //GATOR_LIBRARY_BOOK_H
//
// Created by Aseem Baranwal on 11/5/23.
//


#include "RedBlackTree.h"
#include <fstream>
#include <sstream>

using namespace std;

class GatorLibrary {
private:
    static int findSmallestGreater(vector<Node *> &books, int bookID);

    int flipCount = 0;
public:
    RedBlackTree *redBlackTree = new RedBlackTree();

    void PrintBook(int bookID) const;

    void PrintBooks(int bookID1, int bookID2) const;

    void InsertBook(int bookID, const string& bookName, string authorName, bool availabilityStatus);

    void BorrowBook(int patronID, int bookID, int patronPriority) const;

    void ReturnBook(int patronID, int bookID) const;

    void DeleteBook(int bookID);

    void FindClosestBook(int bookID) const;

    void ColorFlipCount() const;

    static void Quit();
};

int GatorLibrary::findSmallestGreater(vector<Node *> &books, int bookID) {
    int l = 0, r = (int) books.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (books[mid]->book->bookID < bookID) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return l;
}

void GatorLibrary::PrintBook(int bookID) const {
    Node *currBook = redBlackTree->search(bookID);
    if (currBook == nullptr) {
        cout << "BookID not found in the Library" << endl;
        return;
    }
    RedBlackTree::printBook(currBook);
}

void GatorLibrary::PrintBooks(int bookID1, int bookID2) const {
    if (bookID1 > bookID2) {
        cout << "Invalid Input" << endl;
        return;
    }
    redBlackTree->printBooks(bookID1, bookID2);
}

void GatorLibrary::InsertBook(int bookID, const string& bookName, string authorName, bool availabilityStatus) {
    if (redBlackTree->search(bookID) != nullptr) {
        cout << "BookID already exists" << endl;
        return;
    }
    Node *newNode = new Node();
    newNode->book = new Book(bookID, bookName, authorName, availabilityStatus);
    flipCount += redBlackTree->insertBook(newNode);
}

void GatorLibrary::BorrowBook(int patronID, int bookID, int patronPriority) const {
    Node *bookNode = redBlackTree->search(bookID);
    if (bookNode == nullptr) {
        cout << "BookID not found in the Library" << endl;
        return;
    }
    if (!bookNode->book->isAvailable) {
        if (bookNode->book->borrowerID == patronID) {
            cout << "Patron already borrowed the book" << endl;
            return;
        }
        if (bookNode->minHeap->userIDToIndex.find(patronID) != bookNode->minHeap->userIDToIndex.end()) {
            cout << "Patron already in the waitlist" << endl;
            return;
        }
        bookNode->minHeap->insertReservation(new Reservation(patronID, patronPriority));
        cout << "Book " << bookID << " Reserved by Patron " << patronID << endl << endl;
        return;
    }
    bookNode->book->isAvailable = false;
    bookNode->book->borrowerID = patronID;
    cout << "Book " << bookID << " Borrowed by Patron " << patronID << endl << endl;
}

void GatorLibrary::ReturnBook(int patronID, int bookID) const {
    Node *bookNode = redBlackTree->search(bookID);
    if (bookNode == nullptr) {
        cout << "BookID not found in the Library" << endl;
        return;
    }
    if (bookNode->book->isAvailable) {
        cout << "Book already available in the Library" << endl;
        return;
    }
    if (bookNode->book->borrowerID != patronID) {
        cout << "Patron did not borrow the book" << endl;
        return;
    }
    bookNode->book->isAvailable = true;
    bookNode->book->borrowerID = 0;
    cout << "Book " << bookID << " Returned by Patron " << patronID << endl << endl;
    if (bookNode->minHeap->heap.empty()) {
        return;
    }
    int borrowerID = bookNode->minHeap->deleteReservation();
    cout << "Book " << bookID << " Allotted to Patron " << borrowerID << endl << endl;
}

void GatorLibrary::DeleteBook(int bookID) {
    Node *bookNode = redBlackTree->search(bookID);
    if (bookNode == nullptr) {
        cout << "Book not found in the Library" << endl;
        return;
    }
    cout << "Book " << bookID << " is no longer available. ";
    if (!bookNode->book->isAvailable) {
        cout << endl;
        return;
    }
    cout << "Reservations made by Patrons ";
    for (int i = 0; i < bookNode->minHeap->heap.size() - 1; i++) {
        cout << bookNode->minHeap->heap[i]->userID << ", ";
    }
    cout << bookNode->minHeap->heap.back()->userID << " have been cancelled!" << endl << endl;
    flipCount += redBlackTree->deleteBookWithKey(bookID);
}

void GatorLibrary::FindClosestBook(int bookID) const {
    if (redBlackTree->root == nullptr) {
        cout << "Library is empty" << endl;
        return;
    }
    if (redBlackTree->search(bookID) != nullptr) {
        PrintBook(bookID);
        return;
    }
    vector<Node *> books;
    redBlackTree->inorderTraversal(redBlackTree->root, books, -1e6, 1e6);
    int idx = findSmallestGreater(books, bookID);
    if (idx == -1) {
        PrintBook(0);
    } else if (idx == books.size()) {
        PrintBook(books.back()->book->bookID);
    } else {
        if (bookID - books[idx - 1]->book->bookID == books[idx]->book->bookID - bookID) {
            PrintBook(books[idx - 1]->book->bookID);
            PrintBook(books[idx]->book->bookID);
        } else if (bookID - books[idx - 1]->book->bookID < books[idx]->book->bookID - bookID) {
            PrintBook(books[idx - 1]->book->bookID);
        } else {
            PrintBook(books[idx]->book->bookID);
        }
    }
}

void GatorLibrary::ColorFlipCount() const {
    cout << "Colour Flip Count : " << flipCount << endl << endl;
}

void GatorLibrary::Quit() {
    cout << "Program Terminated!!" << endl;
    exit(0);
}


int main(int argc, char **argv) {
    auto *gatorLibrary = new GatorLibrary();
    if (argc < 2) {
        cout << "Invalid Usage" << endl;
        cout << "Usage: " << argv[0] << " file_name" << endl;
        return 1;
    }
    fstream fin(argv[1], ios::in);
    if (!fin) {
        cout << "Error opening file" << endl;
        return 1;
    }

    // Redirect cout to output file
//    cout.rdbuf(fout.rdbuf());

    string line;
    while (getline(fin, line)) {
        string function;
        string args;

        size_t pos = line.find_first_of('(');
        if (pos != string::npos) {
            function = line.substr(0, pos);
            line = line.substr(pos + 1);
        } else {
            cout << "Invalid Input" << endl;
            line.clear();
            return 1;
        }

        size_t openParenPos = line.find('(');
        size_t closeParenPos = line.find(')');
        args = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
        istringstream ss(args);
        string token;
        vector<string> values;
        // Use the function and fetch arguments
        if (function == "PrintBook") {
            int bookID = stoi(args);
            gatorLibrary->PrintBook(bookID);
        } else if (function == "PrintBooks") {
            // Print all the books in the given range
            while (getline(ss, token, ',')) {
                size_t start = token.find_first_not_of(" \t");
                size_t end = token.find_last_not_of(" \t");
                token = token.substr(start, end - start + 1);
                values.push_back(token);
            }
            int bookID1 = stoi(values[0]);
            int bookID2 = stoi(values[1]);
            gatorLibrary->PrintBooks(bookID1, bookID2);
        } else if (function == "InsertBook") {
            // Insert the book with the given details into a Red Black Tree
            while (getline(ss, token, ',')) {
                size_t start = token.find_first_not_of(" \t");
                size_t end = token.find_last_not_of(" \t");
                token = token.substr(start, end - start + 1);

                if (!token.empty() && (token.front() == '\"' || token.front() == '\'')) {
                    token = token.substr(1, token.length() - 2);
                }

                values.push_back(token);
            }
            int bookID = stoi(values[0]);
            string bookName = values[1];
            string authorName = values[2];
            bool availabilityStatus = values[3] == "Yes";
            gatorLibrary->InsertBook(bookID, bookName, authorName, availabilityStatus);
        } else if (function == "BorrowBook") {
            // Borrow the book with the given bookID by the patron with the given patronID
            while (getline(ss, token, ',')) {
                size_t start = token.find_first_not_of(" \t");
                size_t end = token.find_last_not_of(" \t");
                token = token.substr(start, end - start + 1);
                values.push_back(token);
            }
            int patronID = stoi(values[0]);
            int bookID = stoi(values[1]);
            int patronPriority = stoi(values[2]);
            gatorLibrary->BorrowBook(patronID, bookID, patronPriority);
        } else if (function == "ReturnBook") {
            // Return the book with the given bookID by the patron with the given patronID
            while (getline(ss, token, ',')) {
                size_t start = token.find_first_not_of(" \t");
                size_t end = token.find_last_not_of(" \t");
                token = token.substr(start, end - start + 1);
                values.push_back(token);
            }
            int patronID = stoi(values[0]);
            int bookID = stoi(values[1]);
            gatorLibrary->ReturnBook(patronID, bookID);
        } else if (function == "DeleteBook") {
            // Delete the book with the given bookID from the Red Black Tree
            int bookID = stoi(args);
            gatorLibrary->DeleteBook(bookID);
        } else if (function == "FindClosestBook") {
            // Find the closest book to the given bookID
            int bookID = stoi(args);
            gatorLibrary->FindClosestBook(bookID);
        } else if (function == "ColorFlipCount") {
            // Print the color flip count
            gatorLibrary->ColorFlipCount();
        } else if (function == "Quit") {
            // Quit the program
            GatorLibrary::Quit();
        } else {
            cout << "Invalid Input" << endl << endl;
            return 1;
        }
    }

}
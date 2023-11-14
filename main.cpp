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

    void InsertBook(int bookID, const string &bookName, string authorName, bool availabilityStatus);

    void BorrowBook(int patronID, int bookID, int patronPriority) const;

    void ReturnBook(int patronID, int bookID) const;

    void DeleteBook(int bookID);

    void FindClosestBook(int bookID) const;

    void ColorFlipCount() const;

    static void Quit();
};

int GatorLibrary::findSmallestGreater(vector<Node *> &books, int bookID) {
    // Utility function used to find the closest book to the given bookID
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
    // Print the book with the given bookID using the Red Black Tree printBook function
    Node *currBook = redBlackTree->search(bookID);
    if (currBook == nullptr) {
        cout << "Book " << bookID << " not found in the library" << endl << endl;
        return;
    }
    RedBlackTree::printBook(currBook);
}

void GatorLibrary::PrintBooks(int bookID1, int bookID2) const {
    // Print all the books in the given range using the Red Black Tree printBooks function
    if (bookID1 > bookID2) {
        cout << "Invalid Input" << endl;
        return;
    }
    redBlackTree->printBooks(bookID1, bookID2);
}

void GatorLibrary::InsertBook(int bookID, const string &bookName, string authorName, bool availabilityStatus) {
    // Insert the book with the given details into a Red Black Tree as a node
    if (redBlackTree->search(bookID) != nullptr) {
        cout << "BookID already exists" << endl;
        return;
    }
    Node *newNode = new Node();
    newNode->book = new Book(bookID, bookName, authorName, availabilityStatus);
    flipCount += redBlackTree->insertBook(newNode);
}

void GatorLibrary::BorrowBook(int patronID, int bookID, int patronPriority) const {
    // Borrow the book with the given bookID by the patron with the given patronID
    // Find if the requested book is present
    Node *bookNode = redBlackTree->search(bookID);
    if (bookNode == nullptr) {
        cout << "Book " << bookID << " not found in the library" << endl << endl;
        return;
    }
    if (bookNode->book->borrowerID == patronID) {
        // Already borrowed by the patron, hence follow the testcase and say that it is reserved, but don't add to heap
        cout << "Book " << bookID << " Reserved by Patron " << patronID << endl << endl;
        return;
    }
    if (!bookNode->book->isAvailable) {
        // Book is not available, hence add to the heap
        // Also print the patrons whose reservations have been cancelled
        if (bookNode->minHeap->userIDToIndex.find(patronID) != bookNode->minHeap->userIDToIndex.end()) {
            cout << "Patron already in the waitlist" << endl << endl;
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
    // Return the book with the given bookID by the patron with the given patronID
    Node *bookNode = redBlackTree->search(bookID);
    if (bookNode == nullptr) {
        // Book not found
        cout << "Book " << bookID << " not found in the library" << endl << endl;
        return;
    }
    if (bookNode->book->isAvailable) {
        // Considering no duplicate books, this case will never occur
        cout << "Book already available in the Library" << endl << endl;
        return;
    }
    if (bookNode->book->borrowerID != patronID) {
        // Patron did not borrow the book
        cout << "Patron did not borrow the book" << endl << endl;
        return;
    }
    bookNode->book->isAvailable = true;
    bookNode->book->borrowerID = 0;
    cout << "Book " << bookID << " Returned by Patron " << patronID << endl << endl;
    if (bookNode->minHeap->heap.empty()) {
        return;
    }
    int borrowerID = bookNode->minHeap->deleteReservation();
    bookNode->book->borrowerID = borrowerID;
    bookNode->book->isAvailable = false;
    cout << "Book " << bookID << " Allotted to Patron " << borrowerID << endl << endl;
}

void GatorLibrary::DeleteBook(int bookID) {
    // Delete the book with the given bookID from the Red Black Tree as a node and remove all the reservations for the current bookID
    Node *bookNode = redBlackTree->search(bookID);
    if (bookNode == nullptr) {
        cout << "Book not found in the Library" << endl << endl;
        return;
    }
    cout << "Book " << bookID << " is no longer available. ";
    if (!bookNode->minHeap->heap.empty()) {
        // At least Someone has borrowed the book
        string reservations = bookNode->minHeap->heap.size() > 1 ? "Reservations" : "Reservation";
        string patrons = bookNode->minHeap->heap.size() > 1 ? "patrons " : "patron ";
        string hasHave = bookNode->minHeap->heap.size() > 1 ? " have" : " has";
        cout << reservations << " made by " << patrons;
        for (int i = 0; i < (int) bookNode->minHeap->heap.size() - 1; i++) {
            cout << bookNode->minHeap->heap[i]->userID << ", ";
        }
        cout << bookNode->minHeap->heap.back()->userID << hasHave << " been cancelled!" << endl << endl;
    } else {
        // There are no reservations
        cout << endl << endl;
    }
    flipCount += redBlackTree->deleteBookWithKey(bookID);
}

void GatorLibrary::FindClosestBook(int bookID) const {
    // Find the closest book to the given bookID
    if (redBlackTree->root == nullptr) {
        cout << "Library is empty" << endl;
        return;
    }
    if (redBlackTree->search(bookID) != nullptr) {
        // Book is present in the library, hence directly print the book and return
        PrintBook(bookID);
        return;
    }
    // Fetch the list of books and compare both the ends of the books with the bookID. If difference is same, print both in the order
    vector<Node *> books;
    redBlackTree->inorderTraversal(redBlackTree->root, books, -1e6, 1e6);
    int idx = findSmallestGreater(books, bookID);
    if (idx == -1) {
        PrintBook(0);
    } else if (idx == books.size()) {
        PrintBook(books.back()->book->bookID);
    } else {
        if (bookID - books[idx - 1]->book->bookID == books[idx]->book->bookID - bookID) {
            // Same difference
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
    // Print the color flip count
    cout << "Colour Flip Count : " << flipCount << endl << endl;
}

void GatorLibrary::Quit() {
    // Quit the program
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
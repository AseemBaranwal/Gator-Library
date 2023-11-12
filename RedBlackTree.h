//
// Created by Aseem Baranwal on 11/6/23.
//

#ifndef GATOR_LIBRARY_REDBLACKTREE_H
#define GATOR_LIBRARY_REDBLACKTREE_H

#include "Node.h"
#include <vector>
#include <iostream>

class RedBlackTree {
private:
    void leftRotate(Node *node);

    void rightRotate(Node *node);

    int fixRRViolations(Node *node);

    int deleteNode(int key);

    int fixDoubleBlack(Node *child, Node *parent);

public:
    Node *root;

    RedBlackTree();

    int insertBook(Node *newNode);

    int deleteBookWithKey(int key);

    Node *search(int key) const;

    static void printBook(Node *node);

    void inorderTraversal(Node *node, std::vector<Node *> &res, int start, int end);

    void preorder(Node *node);

    void printBooks(int bookID1, int bookID2);
};

#endif //GATOR_LIBRARY_REDBLACKTREE_H

//
// Created by Aseem Baranwal on 11/12/23.
//

// RedBlackTree.cpp

#include "RedBlackTree.h"

RedBlackTree::RedBlackTree() {
    root = nullptr;
}

void RedBlackTree::leftRotate(Node *node) {
    Node *y = node->right;
    node->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == nullptr) {
        root = y;
    } else if (node == node->parent->left) {
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }
    y->left = node;
    node->parent = y;
}

void RedBlackTree::rightRotate(Node *node) {
    Node *y = node->left;
    node->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == nullptr) {
        root = y;
    } else if (node == node->parent->right) {
        node->parent->right = y;
    } else {
        node->parent->left = y;
    }
    y->right = node;
    node->parent = y;
}

int RedBlackTree::fixRRViolations(Node *node) {
    int colorFlips = 0;
    while (node->parent and node->parent->isRed) {
        // If grandparent doesn't exist, then we are done
        Node *pp = node->parent;
        if (pp->parent == nullptr) {
            colorFlips += pp->isRed;
            pp->isRed = false;
            return colorFlips;
        }

        Node *gp = pp->parent;
        if (gp->right and gp->right->isRed) {
            // Case I: XYr
            colorFlips += pp->isRed;
            pp->isRed = false;
            colorFlips += gp->right->isRed;
            gp->right->isRed = false;
            colorFlips += !(gp->isRed);
            gp->isRed = true;
            node = gp;
        } else {
            // Case II: XYb
            bool XIsLeftChild = (pp == gp->left);
            bool YIsLeftChild = (node == pp->left);
            if (XIsLeftChild and YIsLeftChild) {
                // Case IIa: LLb
                colorFlips += pp->isRed;
                pp->isRed = false;
                colorFlips += !(gp->isRed);
                gp->isRed = true;
                rightRotate(gp);
            } else if (!XIsLeftChild and YIsLeftChild) {
                // Case IIb: RLb
                node = pp;
                rightRotate(node);
                colorFlips += node->parent->isRed;
                node->parent->isRed = false;
                colorFlips += !(node->parent->parent->isRed);
                node->parent->parent->isRed = true;
                leftRotate(node->parent->parent);
            } else if (!XIsLeftChild and !YIsLeftChild) {
                // Case IIc: RRb
                colorFlips += pp->isRed;
                pp->isRed = false;
                colorFlips += !(gp->isRed);
                gp->isRed = true;
                leftRotate(gp);
            } else {
                // Case IId: LRb
                node = pp;
                leftRotate(node);
                colorFlips += node->parent->isRed;
                node->parent->isRed = false;
                colorFlips += !(node->parent->parent->isRed);
                node->parent->parent->isRed = true;
                rightRotate(node->parent->parent);
            }
        }
    }
    colorFlips += root->isRed;
    root->isRed = false;
    return colorFlips;
}

int RedBlackTree::deleteNode(int key) {
    int colorFlips = 0;
    Node *node = search(key);
    if (node == nullptr) {
        std::cout << "Key not found in the tree" << std::endl;
        return 0;
    }

    Node *child, *parent;
    bool isRed;

    if (node->left != nullptr and node->right != nullptr) {
        Node *replace = node;
        replace = replace->right;
        while (replace->left != nullptr) {
            replace = replace->left;
        }

        if (node->parent != nullptr) {
            if (node->parent->left == node) {
                node->parent->left = replace;
            } else {
                node->parent->right = replace;
            }
        } else {
            root = replace;
        }

        child = replace->right;
        parent = replace->parent;
        isRed = replace->isRed;

        if (parent == node) {
            parent = replace;
        } else {
            if (child != nullptr) {
                child->parent = parent;
            }
            parent->left = child;
            replace->right = node->right;
            node->right->parent = replace;
        }

        replace->parent = node->parent;
        colorFlips += (replace->isRed != node->isRed);
        replace->isRed = node->isRed;
        replace->left = node->left;
        node->left->parent = replace;

        if (!isRed) {
            colorFlips += fixDoubleBlack(child, parent);
        } else {
            if (child != nullptr) {
                colorFlips += child->isRed;
                child->isRed = false;
            }
        }
        delete node;
        return colorFlips;
    }

    if (node->left != nullptr) {
        child = node->left;
    } else {
        child = node->right;
    }

    parent = node->parent;
    isRed = node->isRed;

    if (child != nullptr) {
        child->parent = parent;
    }

    if (parent != nullptr) {
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    } else {
        root = child;
    }

    if (!isRed) {
        colorFlips += fixDoubleBlack(child, parent);
    } else {
        if (child != nullptr) {
            colorFlips += child->isRed;
            child->isRed = false;
        }
    }

    delete node;
    return colorFlips;
}

int RedBlackTree::fixDoubleBlack(Node *child, Node *parent) {
    int flipCount = 0;
    while (child and child != root and child->isRed == 0) {
        if (child == parent->left) {
            Node *sibling = parent->right;
            if (sibling and sibling->isRed) {
                flipCount += sibling->isRed;
                sibling->isRed = false;
                flipCount += parent->isRed;
                parent->isRed = true;
                leftRotate(parent);
                sibling = parent->right;
            }
            if (sibling->left->isRed == 0 and sibling->right->isRed == 0) {
                flipCount += !(sibling->isRed);
                sibling->isRed = true;
                child = parent;
                parent = child->parent;
            } else {
                if (sibling->right->isRed == 0) {
                    flipCount += sibling->left->isRed;
                    sibling->left->isRed = false;
                    flipCount += !(sibling->isRed);
                    sibling->isRed = true;
                    rightRotate(sibling);
                    sibling = parent->right;
                }
                flipCount += (sibling->isRed != parent->isRed);
                sibling->isRed = parent->isRed;
                flipCount += parent->isRed;
                parent->isRed = false;
                flipCount += sibling->right->isRed;
                sibling->right->isRed = false;
                leftRotate(parent);
                child = root;
            }
        } else {
            Node *sibling = parent->left;
            if (sibling and sibling->isRed) {
                flipCount += sibling->isRed;
                sibling->isRed = false;
                flipCount += !(parent->isRed);
                parent->isRed = true;
                rightRotate(parent);
                sibling = parent->left;
            }
            if (sibling->left->isRed == 0 and sibling->right->isRed == 0) {
                flipCount += !(sibling->isRed);
                sibling->isRed = true;
                child = parent;
                parent = child->parent;
            } else {
                if (sibling->left->isRed == 0) {
                    flipCount += sibling->right->isRed;
                    sibling->right->isRed = false;
                    flipCount += !(sibling->isRed);
                    sibling->isRed = true;
                    leftRotate(sibling);
                    sibling = parent->left;
                }
                flipCount += (sibling->isRed != parent->isRed);
                sibling->isRed = parent->isRed;
                flipCount += parent->isRed;
                parent->isRed = false;
                flipCount += sibling->left->isRed;
                sibling->left->isRed = false;
                rightRotate(parent);
                child = root;
            }
        }
    }
    if (child != nullptr) {
        flipCount += child->isRed;
        child->isRed = false;
    }
    return flipCount;
}

void RedBlackTree::inorderTraversal(Node *node, std::vector<Node *> &res, int start, int end) {
    if (node == nullptr) {
        return;
    }
    if (node->book->bookID > start)
        inorderTraversal(node->left, res, start, end);
    if (node->book->bookID >= start and node->book->bookID <= end) {
        res.push_back(node);
    }
    if (node->book->bookID < end)
        inorderTraversal(node->right, res, start, end);
}

int RedBlackTree::insertBook(Node *newNode) {
    int colorFlips = 0;
    if(!root){
        root = newNode;
        root->isRed = false;
        return 0;
    }
    Node *y = nullptr;
    Node *x = root;
    while (x != nullptr) {
        y = x;
        if (newNode->book->bookID < x->book->bookID) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    newNode->parent = y;
    if (y == nullptr) {
        root = newNode;
    } else if (newNode->book->bookID < y->book->bookID) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }
    colorFlips += fixRRViolations(newNode);
    // Print PreOrder traversal
    return colorFlips;
}

int RedBlackTree::deleteBookWithKey(int key) {
    return deleteNode(key);
}

Node *RedBlackTree::search(int key) const {
    Node *node = root;
    while (node != nullptr) {
        if (key == node->book->bookID) {
            return node;
        } else if (key < node->book->bookID) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return nullptr;
}

void RedBlackTree::printBook(Node *node) {
    node->printBook();
}

void RedBlackTree::printBooks(int bookID1, int bookID2) {
    std::vector<Node *> books;
    inorderTraversal(root, books, bookID1, bookID2);
    for (Node *currBook: books) {
        currBook->printBook();
    }
}

void RedBlackTree::preorder(Node *node) {
    if (node == nullptr) {
        std::cout << "NULL ";
        return;
    }
    std::cout << node->book->bookID << " ";
    preorder(node->left);
    preorder(node->right);
}

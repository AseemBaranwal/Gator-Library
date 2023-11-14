//
// Created by Aseem Baranwal on 11/12/23.
//

// RedBlackTree.cpp

#include "RedBlackTree.h"

RedBlackTree::RedBlackTree() {
    root = nullptr;
}

void RedBlackTree::leftRotate(Node *node) {
    // Rotate the node and its right child
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
    // Rotate the node and its left child
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
    // In case of Red-Red violation, we need to perform color flips and rotations
    // Keep on adding the values in colorFlips which will be returned at the end
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
            // No rotation required, colors are flipped for the parent, grandparent and uncle
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
                // Rotate the parent in the direction of the node
                node = pp;
                rightRotate(node);
                colorFlips += node->parent->isRed;
                node->parent->isRed = false;
                colorFlips += !(node->parent->parent->isRed);
                node->parent->parent->isRed = true;
                // Rotate the grandparent in the opposite direction
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
                // Rotate the parent in the direction of the node
                leftRotate(node);
                colorFlips += node->parent->isRed;
                node->parent->isRed = false;
                colorFlips += !(node->parent->parent->isRed);
                node->parent->parent->isRed = true;
                // Rotate the grandparent in the opposite direction
                rightRotate(node->parent->parent);
            }
        }
    }
    colorFlips += root->isRed;
    root->isRed = false;
    return colorFlips;
}

int RedBlackTree::deleteNode(int key) {
    // Deletes the node with the key present in the tree and balances the tree
    // Calls the fixDoubleBlack function to fix the double black violation
    // Keep on adding the values in colorFlips which will be returned at the end
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
    // Inorder traversal of the tree (Left, Root, Right)
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
    // Inserts the node in the tree and balances the tree
    // Keep on adding the values in colorFlips which will be returned at the end
    int colorFlips = 0;
    if (!root) {
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
    // Deletes the node with the key present in the tree and balances the tree
    // Uses the utility function deleteNode which returns the number of flipCount
    return deleteNode(key);
}

Node *RedBlackTree::search(int key) const {
    // Searches the node with the key present in the tree
    // Returns the node if found, else returns nullptr
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
    // Prints all the books in the range [bookID1, bookID2] both inclusive
    std::vector<Node *> books;
    inorderTraversal(root, books, bookID1, bookID2);
    for (Node *currBook: books) {
        currBook->printBook();
    }
}

void RedBlackTree::preorder(Node *node) {
    // Preorder traversal of the tree (Root, Left, Right)
    // Note: This is just for debugging purposes
    if (node == nullptr) {
        std::cout << "NULL ";
        return;
    }
    std::cout << node->book->bookID << " ";
    preorder(node->left);
    preorder(node->right);
}

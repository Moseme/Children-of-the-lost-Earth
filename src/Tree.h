// OWNER: Student C. Binary Search Tree keyed by Resource ID.
#ifndef TREE_H
#define TREE_H
#include <string>
#include "Resource.h"

class BST {
private:
    struct Node {
        Resource data;
        Node* left;
        Node* right;
        Node(const Resource& r) : data(r), left(NULL), right(NULL) {}
    };
    Node* root;

    // recursive helpers (TODO)
    Node* insert(Node* n, const Resource& r, bool& ok);
    Node* remove(Node* n, const std::string& id, bool& ok);
    void destroy(Node* n);

public:
    BST() : root(NULL) {}
    ~BST();

    bool insert(const Resource& r);
    bool remove(const std::string& id);
    Resource* search(const std::string& id);
    void inorder() const;     // sorted by ID
    void levelOrder() const;  // needs a queue (reuse Student B's Queue or write a local one)
    bool empty() const { return root == NULL; }
};

#endif

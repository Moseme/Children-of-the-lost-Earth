// OWNER: Student C.
// A BINARY SEARCH TREE (BST) of resources, ordered by Resource ID.
//
//   Example: LAB103 is the root.
//     its left subtree  holds IDs that come before it (EQP201, EQP202, ...)
//     its right subtree holds IDs that come after it  (LAB104, ...)
//
// BST rule: for every node, all IDs in its LEFT subtree come before it (alphabetically)
// and all IDs in its RIGHT subtree come after it. This is what makes searching fast:
// each comparison throws away one whole side of the tree.
//
//   insert / search / remove : O(log n) on average (a reasonably balanced tree)
//                              O(n) in the WORST case (IDs inserted in sorted order give a "stick")
//   inorder traversal        : visits the IDs in sorted order, O(n)
//
// Like the hash table, the tree stores POINTERS to the resources in the master list (one copy only).
#ifndef TREE_H
#define TREE_H
#include <string>
#include "Resource.h"

class BST {
private:
    struct Node {
        Resource* data;       // points at the resource stored in the master list
        Node* left;           // smaller IDs
        Node* right;          // larger IDs
        Node(Resource* r) : data(r), left(NULL), right(NULL) {}
    };

    Node* root;
    int count;

    // used while printing traversals (so a huge tree does not flood the screen)
    mutable int printLimit;
    mutable int printedCount;

    BST(const BST&);                 // copying forbidden (declared, never written)
    BST& operator=(const BST&);

    void destroy(Node* n);
    Node* removeNode(Node* n, const std::string& id, bool& removed);
    Node* findMin(Node* n) const;
    int heightOf(const Node* n) const;
    void printNode(const Node* n) const;
    void finishPrint() const;
    void inorderWalk(const Node* n) const;
    void preorderWalk(const Node* n) const;
    void postorderWalk(const Node* n) const;
    void drawNode(const Node* n, int depth, int maxDepth) const;

public:
    BST();
    ~BST();

    bool insert(Resource* r);                                   // false if NULL or ID already present
    bool remove(const std::string& id);                         // false if the ID is not in the tree
    Resource* search(const std::string& id) const;              // NULL if not found
    Resource* search(const std::string& id, long& comparisons) const;   // also counts comparisons

    // Traversals: each prints at most 'limit' rows.
    void inorder(int limit) const;      // left, node, right   -> sorted order
    void preorder(int limit) const;     // node, left, right   -> good for copying a tree
    void postorder(int limit) const;    // left, right, node   -> children before parents (used to delete)
    void levelOrder(int limit) const;   // row by row, top to bottom (breadth-first, uses a queue)

    void drawSideways(int maxDepth) const;   // picture of the tree turned 90 degrees

    int size() const { return count; }
    bool empty() const { return count == 0; }
    int height() const { return heightOf(root); }    // number of levels (0 for an empty tree)
};

#endif

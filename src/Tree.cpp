#include "Tree.h"

BST::~BST() { destroy(root); }
void BST::destroy(Node* n) { if (!n) return; destroy(n->left); destroy(n->right); delete n; }

bool BST::insert(const Resource&) { /* TODO */ return false; }
bool BST::remove(const std::string&) { /* TODO */ return false; }
Resource* BST::search(const std::string&) { /* TODO */ return NULL; }
void BST::inorder() const { /* TODO */ }
void BST::levelOrder() const { /* TODO */ }

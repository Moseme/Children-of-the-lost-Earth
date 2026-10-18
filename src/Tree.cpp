#include "Tree.h"
#include "Utils.h"
#include <iostream>

BST::BST() : root(NULL), count(0), printLimit(0), printedCount(0) {
}

BST::~BST() {
    destroy(root);
}

// Postorder: delete the children first, then the node itself.
void BST::destroy(Node* n) {
    if (n == NULL) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

// ---------------------------------------------------------------- insert
// Walk down from the root: go left if the new ID is smaller, right if larger,
// until we reach an empty spot, then hang the new node there.
bool BST::insert(Resource* r) {
    if (r == NULL) {
        return false;
    }
    Node* newNode = new Node(r);
    if (root == NULL) {
        root = newNode;
        count++;
        return true;
    }
    Node* current = root;
    while (true) {
        int result = r->id.compare(current->data->id);
        if (result == 0) {
            delete newNode;                 // duplicate ID: do not insert
            return false;
        }
        if (result < 0) {
            if (current->left == NULL) { current->left = newNode; break; }
            current = current->left;
        } else {
            if (current->right == NULL) { current->right = newNode; break; }
            current = current->right;
        }
    }
    count++;
    return true;
}

// ---------------------------------------------------------------- search
Resource* BST::search(const std::string& id, long& comparisons) const {
    const Node* current = root;
    while (current != NULL) {
        comparisons++;
        int result = id.compare(current->data->id);
        if (result == 0) {
            return current->data;
        }
        current = (result < 0) ? current->left : current->right;   // discard one whole side
    }
    return NULL;
}

Resource* BST::search(const std::string& id) const {
    long ignored = 0;
    return search(id, ignored);
}

// ---------------------------------------------------------------- remove
BST::Node* BST::findMin(Node* n) const {
    while (n->left != NULL) {
        n = n->left;
    }
    return n;
}

// Three cases when removing a node:
//   1) no children      : just remove it
//   2) one child        : the child takes its place
//   3) two children     : copy in the "inorder successor" (the smallest ID in the right subtree),
//                         then remove that successor node from the right subtree
BST::Node* BST::removeNode(Node* n, const std::string& id, bool& removed) {
    if (n == NULL) {
        return NULL;                         // not found
    }
    int result = id.compare(n->data->id);
    if (result < 0) {
        n->left = removeNode(n->left, id, removed);
    } else if (result > 0) {
        n->right = removeNode(n->right, id, removed);
    } else {
        removed = true;
        if (n->left == NULL) {               // case 1 or 2: no left child
            Node* rightChild = n->right;
            delete n;
            return rightChild;
        }
        if (n->right == NULL) {              // case 2: only a left child
            Node* leftChild = n->left;
            delete n;
            return leftChild;
        }
        Node* successor = findMin(n->right); // case 3
        n->data = successor->data;
        bool ignored = false;
        n->right = removeNode(n->right, successor->data->id, ignored);
    }
    return n;
}

bool BST::remove(const std::string& id) {
    bool removed = false;
    root = removeNode(root, id, removed);
    if (removed) count--;
    return removed;
}

// ---------------------------------------------------------------- traversals
void BST::printNode(const Node* n) const {
    if (printedCount < printLimit) {
        printResourceRow(*n->data);
    }
    printedCount++;
}

void BST::finishPrint() const {
    if (printedCount > printLimit) {
        std::cout << "... (showing the first " << printLimit << " of " << printedCount << ")\n";
    }
}

void BST::inorderWalk(const Node* n) const {
    if (n == NULL) return;
    inorderWalk(n->left);
    printNode(n);
    inorderWalk(n->right);
}

void BST::preorderWalk(const Node* n) const {
    if (n == NULL) return;
    printNode(n);
    preorderWalk(n->left);
    preorderWalk(n->right);
}

void BST::postorderWalk(const Node* n) const {
    if (n == NULL) return;
    postorderWalk(n->left);
    postorderWalk(n->right);
    printNode(n);
}

void BST::inorder(int limit) const {
    if (root == NULL) { std::cout << "(the tree is empty)\n"; return; }
    printResourceHeader();
    printLimit = limit;
    printedCount = 0;
    inorderWalk(root);
    finishPrint();
}

void BST::preorder(int limit) const {
    if (root == NULL) { std::cout << "(the tree is empty)\n"; return; }
    printResourceHeader();
    printLimit = limit;
    printedCount = 0;
    preorderWalk(root);
    finishPrint();
}

void BST::postorder(int limit) const {
    if (root == NULL) { std::cout << "(the tree is empty)\n"; return; }
    printResourceHeader();
    printLimit = limit;
    printedCount = 0;
    postorderWalk(root);
    finishPrint();
}

// Level order uses a QUEUE of nodes: take a node from the front, print it, and add its
// children at the back. Every node enters the queue once, so an array of size 'count' is enough.
void BST::levelOrder(int limit) const {
    if (root == NULL) { std::cout << "(the tree is empty)\n"; return; }
    const Node** queue = new const Node*[count];
    int head = 0;
    int tail = 0;
    queue[tail++] = root;
    int printed = 0;
    int level = 0;
    printResourceHeader();
    while (head < tail) {
        int levelSize = tail - head;               // how many nodes are on this level
        if (printed < limit) std::cout << "-- level " << level << " --\n";
        for (int i = 0; i < levelSize; i++) {
            const Node* current = queue[head++];
            if (printed < limit) printResourceRow(*current->data);
            printed++;
            if (current->left != NULL) queue[tail++] = current->left;
            if (current->right != NULL) queue[tail++] = current->right;
        }
        level++;
    }
    if (printed > limit) {
        std::cout << "... (showing the first " << limit << " of " << printed << ")\n";
    }
    delete[] queue;
}

// ---------------------------------------------------------------- picture and statistics
int BST::heightOf(const Node* n) const {
    if (n == NULL) return 0;
    int leftHeight = heightOf(n->left);
    int rightHeight = heightOf(n->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Reverse inorder (right, node, left) printed with indentation shows the tree turned sideways:
// the root is at the far left, larger IDs above it and smaller IDs below it.
void BST::drawNode(const Node* n, int depth, int maxDepth) const {
    if (n == NULL || depth > maxDepth) return;
    drawNode(n->right, depth + 1, maxDepth);
    std::cout << std::string(depth * 6, ' ') << n->data->id << "\n";
    drawNode(n->left, depth + 1, maxDepth);
}

void BST::drawSideways(int maxDepth) const {
    if (root == NULL) { std::cout << "(the tree is empty)\n"; return; }
    std::cout << "(root on the left; larger IDs above, smaller IDs below)\n";
    drawNode(root, 0, maxDepth);
    if (heightOf(root) > maxDepth + 1) {
        std::cout << "(only the top " << (maxDepth + 1) << " levels are drawn)\n";
    }
}

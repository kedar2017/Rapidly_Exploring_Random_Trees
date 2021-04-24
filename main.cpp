#include <iostream>
#include "Geometry.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Position* pos = new Position(0,0);
    Node* root = new Node(pos);
    Tree* tree = new Tree(root);

    std::vector<Node*> nodesToBeAdd;

    for (int i = 0; i < 10; ++i) {
        nodesToBeAdd.push_back(new Node(new Position(3,i)));
    }

    for (Node* node:nodesToBeAdd) {
        tree->addNode(tree->root, node);
    }

    Node* findClosest = new Node(new Position(14,100));
    Node* nearest = tree->findNearest(findClosest);
    std::cout << nearest->pos->posX << std::endl;
    std::cout << nearest->pos->posY << std::endl;

    return 0;
}

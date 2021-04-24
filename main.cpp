#include <iostream>
#include "Geometry.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    int winX = 100;
    int winY = 100;
    Space space(winX, winY);

    Position* pos = new Position(0,0);
    Node* root = new Node(pos);
    Tree* tree = new Tree(root);
    Node* randomNode = space.getRandomNode();
    Node* nearestNode = tree->findNearest(randomNode);
    return 0;
}

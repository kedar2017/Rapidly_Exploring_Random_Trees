#include <iostream>
#include <cstdio>
#include "string"
#include "Geometry.h"

Node* generateRandomNode(Space* space){
    return space->getRandomNode();
}

Node* nearestNodeTree(Tree* tree, Node* sampledNode){
    return tree->findNearest(sampledNode);
}

bool checkGoaltoTree(Tree* tree, Node* goalNode, float* threshold){
    Node* nearestNode = nearestNodeTree(tree,goalNode);
    if (radialPosCheckFloat(goalNode->pos,nearestNode->pos,threshold))
        return true;
    return false;
}

Node* expandTree(Tree* tree, Node* startNode, Node* endNode){
    return tree->expandToRandom(startNode, endNode);
}

void updateFreeSpace(Space* space, Node* randomedNode){
    return space->removeNodeFreeSpace(randomedNode);
}

bool checkLineCollision(Space* space, Position* start, Position* end){

    for (Obstacle* obs:space->obstacles){
        if (linePassesObstacle(obs,start,end)){
            return true;
        }
    }
    return false;
}

bool checkPointCollision(Space* space, Position* point){

    for (Obstacle* obs:space->obstacles){
        if (insidePolygon(obs,point))
            return true;
    }
    return false;
}

void printTree(Tree* tree){
    Node* root = tree->root;
    std::vector<Node*> stack;
    stack.push_back(root);
    while (!stack.empty()) {
        Node* currN = stack.front();
        stack.erase(stack.begin());
        for (Node* child:currN->children) {
            stack.insert(stack.begin(),child);
            std::cout << "Node location"  << std::endl;
            std::cout << std::to_string(child->pos->posX) + " and " + std::to_string(child->pos->posY) << std::endl;
        }
    }
}

void run(Space* space){

    Position* pos = new Position(space->start[0],space->start[1]);
    Node* root = new Node(pos);
    Tree* tree = new Tree(root);
    space->removeNodeFreeSpace(root);
    float threshold = 2;
    Node* goalNode = new Node(new Position(space->goal[0],space->goal[1]));
    int iterations = 0;

    while (checkGoaltoTree(tree,goalNode,&threshold)){
        iterations++;

        Node* randomNode = generateRandomNode(space);
        if (checkPointCollision(space,randomNode->getPos())){
            continue;
        }
        Node* nearestNode = nearestNodeTree(tree, randomNode);
        if (checkLineCollision(space, nearestNode->getPos(), randomNode->getPos())){
            continue;
        }

        Node* removeNodefromSpace = expandTree(tree,nearestNode,randomNode);
        updateFreeSpace(space,removeNodefromSpace);
    }
    printTree(tree);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    int winX = 100;
    int winY = 100;

    std::vector<int> start{10,10};
    std::vector<int> goal{45,23};
    std::vector<std::vector<int>> obstacles{{10,78,78}};
    Space* space = new Space(winX, winY, start, goal, obstacles);

    run(space);

    return 0;
}

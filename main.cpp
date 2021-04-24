#include <iostream>
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

void expandTree(Tree* tree, Node* startNode, Node* endNode){
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

void run(Space* space){

    Position* pos = new Position(space->start[0],space->start[1]);
    Node* root = new Node(pos);
    Tree* tree = new Tree(root);
    float threshold = 0.001;
    Node* goalNode = new Node(new Position(space->goal[0],space->goal[1]));

    while (checkGoaltoTree(tree,goalNode,&threshold)){
        Node* randomNode = generateRandomNode(space);
        if (checkPointCollision(space,randomNode->getPos())){
            continue;
        }
        Node* nearestNode = nearestNodeTree(tree, randomNode);
        if (checkLineCollision(space, nearestNode->getPos(), randomNode->getPos())){
            continue;
        }
        expandTree(tree,nearestNode,randomNode);
        updateFreeSpace(space,randomNode);
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    int winX = 100;
    int winY = 100;

    std::vector<int> start{10,10};
    std::vector<int> goal{90,90};
    std::vector<std::vector<int>> obstacles{{30,40,10},{0,0,0}};
    Space* space = new Space(winX, winY, start, goal, obstacles);

    run(space);

    return 0;
}

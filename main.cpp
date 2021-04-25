#include <iostream>
#include "fstream"
#include <cstdio>
#include "string"
#include "Geometry.h"
#include "json.h"

using json = nlohmann::json;
/*
json j2 = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {
                       {"everything", 42}
               }},
        {"{1,2}", {1, 0, 2},{3,2},{4,5}},
        {"object", {
                       {"currency", "USD"},
                     {"value", 42.99}
               }}
};
*/
json j2;

Node* generateRandomNode(Space* space){
    return space->getRandomNode();
}

Node* nearestNodeTree(Tree* tree, Node* sampledNode){
    return tree->findNearest(sampledNode);
}

bool checkGoaltoTree(Tree* tree, Node* goalNode, int* threshold){
    Node* nearestNode = nearestNodeTree(tree,goalNode);
    if (radialPosCheck(goalNode->pos,nearestNode->pos,threshold))
        return true;
    return false;
}

Node* createNewNodeNearest(Tree* tree, Node* startNode, Node* endNode){
    return tree->createNewNodetoNearest(startNode,endNode);
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

void printJSON(Tree* tree){
    for (auto node: tree->treeNs){
        int posX = node->pos->posX;
        int posY = node->pos->posY;
        std::string key = "{"+std::to_string(posX)+","+std::to_string(posY)+"}";
        std::string value = "";
        for (auto child:node->children){
            value = value + "{"+std::to_string(child->pos->posX)+","+std::to_string(child->pos->posY)+"},";
        }
        j2[key] = value;
    }
    return;
}

void run(Space* space){

    Position* pos = new Position(space->start[0],space->start[1]);
    Node* root = new Node(pos);
    Tree* tree = new Tree(root);
    space->removeNodeFreeSpace(root);
    int threshold = 5;
    Node* goalNode = new Node(new Position(space->goal[0],space->goal[1]));
    int iterations = 0;

    while (checkGoaltoTree(tree,goalNode,&threshold)){
        iterations++;
        Node* randomNode = generateRandomNode(space);
        if (checkPointCollision(space,randomNode->getPos())){
            continue;
        }
        Node* nearestNode = nearestNodeTree(tree, randomNode);
        Node* removeNodefromSpace = createNewNodeNearest(tree,nearestNode,randomNode);

        if (checkLineCollision(space, nearestNode->getPos(), removeNodefromSpace->getPos())){
            continue;
        }
        removeNodefromSpace = expandTree(tree,nearestNode,randomNode);
        updateFreeSpace(space,removeNodefromSpace);
    }
    printTree(tree);
    printJSON(tree);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    int winX = 50;
    int winY = 50;

    std::vector<int> start{10,10};
    std::vector<int> goal{45,45};
    std::vector<std::vector<int>> obstacles{{5,40,25},{6,17,40},{4,35,15}};
    Space* space = new Space(winX, winY, start, goal, obstacles);
    std::ofstream print("pretty.json");

    run(space);

    print << std::setw(4) << j2 << std::endl;
    return 0;
}

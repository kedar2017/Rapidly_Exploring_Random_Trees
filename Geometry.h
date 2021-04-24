//
// Created by kedar joshi on 4/23/21.
//

#ifndef UNTITLED1_GEOMETRY_H
#define UNTITLED1_GEOMETRY_H

#include "vector"
#include "math.h"
#include "random"

class Position{
public:
    int posX;
    int posY;

    Position(int x, int y){
        posX = x;
        posY = y;
    }
};

class Node{
public:

    Position* pos;
    Node* parent;
    std::vector<Node*> children;

    Node(Position* pos){
        this->pos = pos;
        return;
    }

     Position* getPos(){
        return this->pos;
    }

    void setPos(Position* pos){
        this->pos = pos;
        return;
    }

    void setParent(Node* parent){
        this->parent = parent;
        return;
    }

    Node* getParent(){
        return this->parent;
    }

    void addChild(Node* child){
        this->children.push_back(child);
    }
};

class Tree{
public:

    Node* root;
    std::vector<Node*> treeNs;

    Tree(Node* rootInit){
        root = rootInit;
        root->parent = NULL;
        treeNs.push_back(rootInit);
    }

    void addNode(Node* addEnd, Node* addEr){
        addEnd->children.push_back(addEr);
        addEr->parent = addEnd;
        this->treeNs.push_back(addEr);
        return;
    }

    Node* findNearest(Node* randomNode){
        float minDist = INFINITY;
        Node* nearestNode;
        for (Node* node:this->treeNs){
            float  dist = estDist(node, randomNode);
            if( dist < minDist){
                minDist = dist;
                nearestNode = node;
            }
        }
        return nearestNode;
    }

    float estDist(Node* node, Node* randomNode){

        return sqrt(pow(node->pos->posY - randomNode->pos->posY, 2) + pow(node->pos->posX - randomNode->pos->posX,2));
    }

    void expandToRandom(Node* randomNode){
        return;
    }

};

class Space{
public:

    int winX;
    int winY;
    std::vector<std::vector<int>> points;

    Space(int x, int y){
        this->winX = x;
        this->winY = y;

        for (int i = 0; i < winX; ++i) {
            for (int j = 0; j < winY; ++j){
                points.push_back({i,j});
            }
        }
        return;
    }

    std::vector<int> getRandomPoint(){
        std::random_device randomDevice;
        std::mt19937  engine{randomDevice()};
        std::uniform_real_distribution<double> dist(0, this->points.size());
        return this->points[dist(engine)];
    }

    Node* getRandomNode(){
        std::vector<int> randomPoint = this->getRandomPoint();
        return new Node(new Position(randomPoint[0], randomPoint[1]));
    }
};
#endif //UNTITLED1_GEOMETRY_H

//
// Created by kedar joshi on 4/23/21.
//

#ifndef UNTITLED1_GEOMETRY_H
#define UNTITLED1_GEOMETRY_H

#include "vector"
#include "math.h"
#include "random"
#include "map"

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
        this->parent = NULL;
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

    Node* createNewNodetoNearest(Node* nearestNode, Node* randomNode){
        int DELTA = 5;
        Node* addedNodeFromSpace;
        if (estDist(nearestNode, randomNode)<DELTA){
            addedNodeFromSpace = randomNode;
        }
        else {
            float theta = atan2(abs(nearestNode->pos->posY-randomNode->pos->posY),abs(nearestNode->pos->posX-randomNode->pos->posX));
            Node* expandTo = new Node(new Position(nearestNode->pos->posX + DELTA * cos(theta), nearestNode->pos->posY + DELTA * sin(theta)));
            addedNodeFromSpace = expandTo;
        }
        return addedNodeFromSpace;
    }

    Node* expandToRandom(Node* expandFrom, Node* randomNode){
        int DELTA = 5;
        Node* addedNodeFromSpace;
        if (estDist(expandFrom, randomNode)<DELTA){
            addNode(expandFrom, randomNode);
            addedNodeFromSpace = randomNode;
        }
        else {
            float theta = atan2(abs(expandFrom->pos->posY-randomNode->pos->posY),abs(expandFrom->pos->posX-randomNode->pos->posX));
            Node* expandTo = new Node(new Position(expandFrom->pos->posX + DELTA * cos(theta), expandFrom->pos->posY + DELTA * sin(theta)));
            addNode(expandFrom, expandTo);
            addedNodeFromSpace = expandTo;
        }
        return addedNodeFromSpace;
    }

};

class Obstacle{
public:

    Position* center;
    int radius;

    Obstacle(Position* center, int radius){
        this->center = center;
        this->radius= radius;
        return;
    }
};

class Space{
public:

    int winX;
    int winY;
    std::vector<int> start;
    std::vector<int> goal;
    std::vector<std::vector<int>> points;
    std::vector<Obstacle*> obstacles;

    Space(int x, int y, std::vector<int> start, std::vector<int> goal, std::vector<std::vector<int>> obst){
        this->winX = x;
        this->winY = y;
        this->start = start;
        this->goal = goal;

        for (int i = 0; i < winX; ++i) {
            for (int j = 0; j < winY; ++j){
                this->points.push_back({i,j});
            }
        }
        initObstacles(obst);
        return;
    }

    void initObstacles(std::vector<std::vector<int>> tuples){
        for (int i = 0; i < tuples.size(); ++i) {
            Position* centerPos = new Position(tuples[i][1],tuples[i][2]);
            Obstacle* obstacleNew = new Obstacle(centerPos,tuples[i][0]);
            this->obstacles.push_back(obstacleNew);
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

    void removeNodeFreeSpace(Node* node){
        int posX = node->getPos()->posX;
        int posY = node->getPos()->posY;
        int vecIndex = 0;
        for(auto point:this->points)
        {
            if(point[0] == posX && point[1] == posY)
            {
                this->points.erase(this->points.begin()+vecIndex);
                break;
            }
            vecIndex++;
        }
        return;
    }
};

bool radialPosCheck(Position* checkPoint, Position* center, int* radius){
    float radialPos = sqrt(pow(checkPoint->posX -center->posX,2)+pow(checkPoint->posY -center->posY,2));
    if (radialPos <= *radius){
        return false;
    }
    return true;
}

bool radialPosCheckFloat(Position* checkPoint, Position* center, int radius){
    float radialPos = sqrt(pow(checkPoint->posX -center->posX,2)+pow(checkPoint->posY -center->posY,2));
    if (radialPos < radius){
        return true;
    }
    return false;
}

bool insidePolygon(Obstacle* obstacle, Position* point){
    if (radialPosCheckFloat(point, obstacle->center, obstacle->radius)){
        return true;
    }
    return false;
}

int distToLine(Obstacle* obs, Position* start, Position* end){
    int x0 = obs->center->posX;
    int x1 = start->posX;
    int x2 = end->posX;
    int y0 = obs->center->posY;
    int y1 = start->posY;
    int y2 = end->posY;

    int nume = abs((x2-x1)*(y1-y0) - (x1-x0)*(y2-y1));
    int deno = sqrt(pow(x2-x1,2) + pow(y2-y1,2));

    return nume/deno;
}

bool linePassesObstacle(Obstacle* obs, Position* start, Position* end){

    if (insidePolygon(obs,start) || insidePolygon(obs,end))
        return true;
    if (distToLine(obs,start,end) < obs->radius){
        return true;
    }
    return false;
}


#endif //UNTITLED1_GEOMETRY_H


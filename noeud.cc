/**
* \name noeud.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 2.0
*/

//Architecture: Fig 11 b1

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "noeud.h"
#include "error.h"
#include "tools.h"
#include "constantes.h"
using namespace std;

//Static methods used for Dijkstra
static void initTN(vector<Noeud*>& tn);
static void initTA(vector<unsigned int>& ta, const vector<Noeud*>& tn);
static void sortTA(vector<unsigned int>& ta, const vector<Noeud*>& tn, Noeud* index);
static bool tnEmpty(const vector<Noeud*>& tn);
static unsigned int findMinAccess(const vector<unsigned int>& ta, 
        const vector<Noeud*>& tn);
static double linkValue(Noeud* noeud1, Noeud* noeud2);

//Constructors & destructor:
Noeud::Noeud(unsigned int uid, double x, double y, unsigned int size):
    uid(uid), size(size), in(true), parent(nullptr), access(infinite_time) {
    position.centre.x = x; position.centre.y = y; position.rayon = sqrt(size);
}

Housing::Housing(unsigned int uid, double x, double y, unsigned int size):
    Noeud(uid, x, y, size) {
}

Transport::Transport(unsigned int uid, double x, double y, unsigned int size):
    Noeud(uid, x, y, size) {
}

Production::Production(unsigned int uid, double x, double y, unsigned int size):
    Noeud(uid, x, y, size) {
}

Noeud::~Noeud() {}

//General methods:
unsigned int Noeud::getUid() {
    return uid;
}

void Noeud::setUid(unsigned int x) {
    uid = x;
}

Cercle Noeud::getPosition() {
    return position;
}

Coords Noeud::getCoords() {
    return position.centre;
}

void Noeud::setCoords(double x, double y) {
    position.centre.x = x;
    position.centre.y = y;
}

unsigned int Noeud::getSize() {
    return size;
}

void Noeud::setSize(unsigned int x) {
    position.rayon = sqrt(x);
    size = x;
}

vector<Noeud*> Noeud::getLiens() {
    return liens;
}

void Noeud::setLiens(Noeud* linkUid) {
    liens.push_back(linkUid);
}

void Noeud::removeLien(unsigned int index) {
    swap(liens[index], liens.back());
    liens.pop_back();
}

bool Noeud::getIn() {
    return in;
}

void Noeud::setIn(bool value) {
    in = value;
}

Noeud* Noeud::getParent() {
    return parent;
}

void Noeud::setParent(Noeud* value) {
    parent = value;
}

double Noeud::getAccess() {
    return access;
}

void Noeud::setAccess(double value) {
    access = value;
}

void Noeud::outputInfo(ofstream& str) {
    str << "\t" << uid << " " << position.centre.x << " " << position.centre.y << " ";
    str << size << "\n"; 
} //writes its uid x y size into a stream

//Error functions (node):
bool Noeud::testReservedUid() {
    if(uid == no_link) {
        cout << error::reserved_uid() << endl;
        return true;
    }
    return false;
}

bool Noeud::testCapacityProblem() {
    if(size < min_capacity) {
        cout << error::too_little_capacity(getSize()) << endl;
        return true;
    }

    if(size > max_capacity) {
        cout << error::too_much_capacity(getSize()) << endl;
        return true;
    }

    return false;
}

bool Noeud::testMaxLink() {
    return false;
}

bool Noeud::testIdenticalUid(const vector<Noeud*> ensemble) {
    if(ensemble.empty() == true) {
        return false;
    }

    for(unsigned int i = 0; i < ensemble.size(); ++i) {
        if(getUid() == ensemble[i]->getUid()) {
            cout << error::identical_uid(getUid()) << endl;
            return true;
        }
    }

    return false;
}

bool Noeud::testNodeNodeSuperposition(const vector<Noeud*> ensemble, 
    double safeDistance) {
    if(ensemble.empty() == true) {
        return false;
    }

     for(unsigned int i = 0; i < ensemble.size(); ++i) {
         if(intersectionCC(getPosition(), ensemble[i]->getPosition(), safeDistance)) {
            cout << error::node_node_superposition(getUid(), 
                ensemble[i]->getUid()) << endl;
            return true;
        }
    }

    return false;
}

bool Noeud::testNodeValidity(const vector<Noeud*> ensemble, double safeDistance) {
    bool notValid = testCapacityProblem() or testReservedUid()
            or testIdenticalUid(ensemble) 
                or testNodeNodeSuperposition(ensemble, safeDistance);

    if(notValid == true) {
        return false; //noeud IS NOT valid
    }
    
    return true;
} //runs before creating a node

void Noeud::dijkstra(vector<Noeud*>& tn, string nodeType) {
    return;
}

void Noeud::updateShortestPathToProd(vector<Noeud*>& ensemble, Noeud* goal) {
    return;
}

void Noeud::updateShortestPathToTrans(vector<Noeud*>& ensemble, Noeud* goal) {
    return;
}

double Noeud::mtaHP() {
    return 0;
}

double Noeud::mtaHT() {
    return 0;
}

vector<Noeud*> Noeud::getShortestProd() {
    vector<Noeud*> empty;
    return empty;
}

vector<Noeud*> Noeud::getShortestTrans() {
    vector<Noeud*> empty;
    return empty;
}

void Noeud::drawShortestPath() {
    return;
}

//Housing methods:
string Housing::getType() {
    return "housing";
}

void Housing::draw() {
    drawCircle(position);
}

bool Housing::testMaxLink() {
    if(liens.empty() == true) return false;
    if(liens.size() > max_link) return true;
    return false;
}

vector<Noeud*> Housing::getShortestProd() {
    return shortestPathToProd;
}

vector<Noeud*> Housing::getShortestTrans() {
    return shortestPathToTrans;
}

void Housing::dijkstra(vector<Noeud*>& tn, string nodeType) {
    //initialize
    initTN(tn);
    access = 0; //current node = beginning node
    vector<unsigned int> ta;
    initTA(ta, tn);
    sortTA(ta, tn, this);

    //main loop
    while(not tnEmpty(tn)) {
        unsigned int n = findMinAccess(ta, tn);
        if(tn[n]->getType() == nodeType and tn[n]->getParent() != nullptr) {
            if(nodeType == "transport") updateShortestPathToTrans(tn, tn[n]);
            if(nodeType == "production") updateShortestPathToProd(tn, tn[n]);
            return;
        }

        //stop dijkstra if it goes rogue
        if(tn[n] != this and tn[n]->getParent() == nullptr) return;

        tn[n]->setIn(false); //we dealt with this node
        //not allowed to pass through production, ie a production "has no links"
        if(not (tn[n]->getType() == "production")) {
            for(unsigned int i = 0; i < tn[n]->getLiens().size(); ++i) {
                //if node has not been checked
                if(tn[n]->getLiens()[i]->getIn() == true) {
                    double alt = tn[n]->getAccess() 
                        + linkValue(tn[n]->getLiens()[i], tn[n]);
                    //if new path > old path
                    if(tn[n]->getLiens()[i]->getAccess() > alt) {
                        tn[n]->getLiens()[i]->setAccess(alt);
                        tn[n]->getLiens()[i]->setParent(tn[n]); //update old path
                        sortTA(ta, tn, tn[n]->getLiens()[i]); //update TA
                    }
                }
            }
        }
    }
    return;
}

void Housing::updateShortestPathToProd(vector<Noeud*>& ensemble, Noeud* goal) {
    if(not shortestPathToProd.empty()) shortestPathToProd.clear();
    Noeud* newParent = goal;

    while(newParent != nullptr) {
        shortestPathToProd.push_back(newParent);
        newParent = newParent->getParent();
    }
}

void Housing::updateShortestPathToTrans(vector<Noeud*>& ensemble, Noeud* goal) {
    if(not shortestPathToTrans.empty()) shortestPathToTrans.clear();
    Noeud* newParent = goal;

    while(newParent != nullptr) {
        shortestPathToTrans.push_back(newParent);
        newParent = newParent->getParent();
    }
}

double Housing::mtaHP() {
    if(shortestPathToProd.empty()) return infinite_time;
    double mtaHP(0);
    
    for(unsigned int i = 0; i < shortestPathToProd.size()-1; ++i) {
        mtaHP += linkValue(shortestPathToProd[i], shortestPathToProd[i+1]);
    }

    return mtaHP;
}

double Housing::mtaHT() {
    if(shortestPathToTrans.empty()) return infinite_time;
    double mtaHT(0);
    
    for(unsigned int i = 0; i < shortestPathToTrans.size()-1; ++i) {
        mtaHT += linkValue(shortestPathToTrans[i], shortestPathToTrans[i+1]);
    }

    return mtaHT;
}

void Housing::drawShortestPath() {
    setColor(GREEN);

    if(not shortestPathToProd.empty()) {
        for(unsigned int i = 0; i < shortestPathToProd.size()-1; ++i) {
            drawSegment(shortestPathToProd[i]->getCoords(), 
                shortestPathToProd[i+1]->getCoords());
        }

        for(unsigned int i = 0; i < shortestPathToProd.size()-1; ++i) {
            shortestPathToProd[i]->draw();
        }
    }

    if(not shortestPathToTrans.empty()) {
        for(unsigned int i = 0; i < shortestPathToTrans.size()-1; ++i) {
            drawSegment(shortestPathToTrans[i]->getCoords(), 
                shortestPathToTrans[i+1]->getCoords());
        }

        for(unsigned int i = 0; i < shortestPathToTrans.size()-1; ++i) {
            shortestPathToTrans[i]->draw();
        }
    }
    return;
}

//Transport methods:
string Transport::getType() {
    return "transport";
}

void Transport::draw() {
    drawCircle(position);
    double x = position.centre.x;
    double y = position.centre.y;
    double r = position.rayon;

    //Dessin des traits (points de dessin pour les segments du noeud transport)
    Coords p1{x - r, y};
    Coords p2{x + r, y};
    drawSegment(p1, p2);

    Coords p3{x, y - r};
    Coords p4{x, y + r};
    drawSegment(p3, p4);

    Coords p5{x + r*cos(5*M_PI/4), y + r*sin(5*M_PI/4)};
    Coords p6{x + r*cos(M_PI/4), y + r*sin(M_PI/4)};
    drawSegment(p5, p6);

    Coords p7{x + r*cos(-M_PI/4), y + r*sin(-M_PI/4)};
    Coords p8{x + r*cos(3*M_PI/4), y + r*sin(3*M_PI/4)};
    drawSegment(p7, p8);
}

//Production methods:
string Production::getType() {
    return "production";
}

void Production::draw() {
    drawCircle(position);
    double x = position.centre.x;
    double y = position.centre.y;
    double d = 2 * position.rayon;

    //Dimensions du rectangle du noeud production
    const double rectLength = 0.75;
    const double rectHeight = 0.125;

    Coords point{x - d*rectLength/2, y - d*rectHeight/2};
    drawRectangle(point, d*rectLength, d*rectHeight);
}

//Static Dijkstra methods definition
void initTN(vector<Noeud*>& tn) {
    for(unsigned int i = 0; i < tn.size(); ++i) {
        tn[i]->setIn(true);
        tn[i]->setAccess(infinite_time);
        tn[i]->setParent(nullptr);
    }   
}

void initTA(vector<unsigned int>& ta, const vector<Noeud*>& tn) {
    for(unsigned int i = 0; i < tn.size(); ++i) {
        ta.push_back(i);
    }
}

void sortTA(vector<unsigned int>& ta, const vector<Noeud*>& tn, Noeud* index) {
    unsigned int indexInTA;
    bool keepGoing = true;

    for(unsigned int i = 0; i < tn.size() and keepGoing; ++i) {
        if(tn[ta[i]] == index) {
            indexInTA = i;
            keepGoing = false;
        }
    }

    while(indexInTA > 0 
            and tn[ta[indexInTA]]->getAccess() < tn[ta[indexInTA-1]]->getAccess()) {
        swap(ta[indexInTA], ta[indexInTA-1]);
        --indexInTA;
    }
}

bool tnEmpty(const vector<Noeud*>& tn) {
    for(unsigned int i = 0; i < tn.size(); ++i) {
        if(tn[i]->getIn()) return false; //there exists an in that is true
        //ie TN is not "empty"
    }
    return true;
}

unsigned int findMinAccess(const vector<unsigned int>& ta, const vector<Noeud*>& tn) {
    for(unsigned int i = 0; i < ta.size(); ++i) {
        if(tn[ta[i]]->getIn() == true) {
            return ta[i];
        }
        //first element of TA that has in = true
    }
    return 0;
}

double linkValue(Noeud* noeud1, Noeud* noeud2) {
    double distance, speed;

    Vecteur dist;
    creeVecteur(noeud1->getCoords(), noeud2->getCoords(), dist);
    distance = norme(dist);

    if(noeud1->getType() == "transport" and noeud2->getType() == "transport") {
        speed = fast_speed;
    } else {
        speed = default_speed;
    }

    return distance/speed;
}
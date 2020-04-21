/**
* \name noeud.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date Mars 2020
* \version 1.0
*/

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
static void initTA(vector<unsigned int>& ta, const vector<Noeud*>& tn);
static void sortTA(vector<unsigned int>& ta, const vector<Noeud*>& tn);
static bool tnEmpty(const vector<Noeud*>& tn);
static unsigned int findMinAccess(const vector<unsigned int>& ta, const vector<Noeud*>& tn);
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

Noeud::~Noeud() {
    //Ville: delete ensembleNoeuds[i]; (delete calls this destructor)
    //Ville: then swap and pop_back to properly remove from main vector
    //Destructor (for rendu 3):
    //remove node from its neighbors' list of connections
}

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

bool Noeud::testNodeNodeSuperposition(const vector<Noeud*> ensemble) {
    if(ensemble.empty() == true) {
        return false;
    }

     for(unsigned int i = 0; i < ensemble.size(); ++i) {
         if(intersectionCC(getPosition(), ensemble[i]->getPosition())) {
            cout << error::node_node_superposition(getUid(), 
                ensemble[i]->getUid()) << endl;
            return true;
        }
    }

    return false;
}

bool Noeud::testNodeValidity(const vector<Noeud*> ensemble) {
    bool notValid = testCapacityProblem() or testReservedUid()
            or testIdenticalUid(ensemble) or testNodeNodeSuperposition(ensemble);

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
    for(unsigned int i = 0; i < tn.size(); ++i) {
        tn[i]->setIn(true);
        tn[i]->setAccess(infinite_time);
        tn[i]->setParent(nullptr);
    }
    access = 0; //current node = beginning node
    vector<unsigned int> ta;
    initTA(ta, tn);
    sortTA(ta, tn);

    //main loop
    while(not tnEmpty(tn)) {
        unsigned int n = findMinAccess(ta, tn);
        if(tn[n]->getType() == nodeType and tn[n]->getParent() != nullptr) {
            if(nodeType == "transport") {
                cout << "DIJKSTRA DONE, FINAL INDEX: " << n << " FOR TRANS, NODE N. " << uid << endl;
                updateShortestPathToTrans(tn, tn[n]);
                return;
            }
            if(nodeType == "production") {
                cout << "DIJKSTRA DONE, FINAL INDEX: " << n << " FOR PROD, NODE N. " << uid << endl;
                updateShortestPathToProd(tn, tn[n]);
                return;
            }
            return;
        }

        tn[n]->setIn(false); //we dealt with this node
        //not allowed to pass through production, ie a production "has no links"
        if(not (tn[n]->getType() == "production")) {
            for(unsigned int i = 0; i < tn[n]->getLiens().size(); ++i) {
                if(tn[n]->getLiens()[i]->getIn() == true) { //if node has not been checked
                    double alt = tn[n]->getAccess() + linkValue(tn[n]->getLiens()[i], tn[n]);
                    if(tn[n]->getLiens()[i]->getAccess() > alt) { //if new road > old road
                        tn[n]->getLiens()[i]->setAccess(alt);
                        tn[n]->getLiens()[i]->setParent(tn[n]); //update old road
                        sortTA(ta, tn);
                    }
                }

            } //end of for loop for TN[n]'s links

        } //end of big if (for production)

    } //end of while

    return;
}

void Housing::updateShortestPathToProd(vector<Noeud*>& ensemble, Noeud* goal) {
    //shortestPathToProd.push_back(goal);
    Noeud* newParent = goal;

    while(newParent != nullptr) {
        shortestPathToProd.push_back(newParent);
        newParent = newParent->getParent();
    }
    // if(goal->getParent() != nullptr) {
    //     shortestPathToProd.push_back(goal);
    //     updateShortestPathToProd(ensemble, goal->getParent());
    // }
}

void Housing::updateShortestPathToTrans(vector<Noeud*>& ensemble, Noeud* goal) {
    Noeud* newParent = goal;

    while(newParent != nullptr) {
        shortestPathToTrans.push_back(newParent);
        newParent = newParent->getParent();
    }
    // if(goal->getParent() != nullptr) {
    //     shortestPathToTrans.push_back(goal);
    //     updateShortestPathToTrans(ensemble, goal->getParent());
    // }
}

double Housing::mtaHP() {
    if(shortestPathToProd.empty()) return infinite_time;
    double mtaHP(0);
    
    for(unsigned int i = 0; i < shortestPathToProd.size()-1; ++i) {
        mtaHP += linkValue(shortestPathToProd[i], shortestPathToProd[i+1]);
    }

    //take into account original node (no real need now)
    //mtaHP += linkValue(shortestPathToProd.back(), this); //prolly a pb here?
    //cout << "original node mta value prod: " << linkValue(shortestPathToProd.back(), this) << endl;
    cout << "full mta value prod: " << mtaHP << endl;

    return mtaHP;
}

double Housing::mtaHT() {
    if(shortestPathToTrans.empty()) return infinite_time;
    double mtaHT(0);
    
    for(unsigned int i = 0; i < shortestPathToTrans.size()-1; ++i) {
        mtaHT += linkValue(shortestPathToTrans[i], shortestPathToTrans[i+1]);
    }

    //take into account original node
    //mtaHT += linkValue(shortestPathToTrans.back(), this);
    //cout << "original node mta value trans: " << linkValue(shortestPathToTrans.back(), this) << endl;
    cout << "full mta value trans: " << mtaHT << endl;

    return mtaHT;
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

    //Dessin des traits (points de dessin pour le rectangle du noeud production)
    Coords p1{x - d*rectLength/2, y - d*rectHeight/2};
    Coords p2{x + d*rectLength/2, y - d*rectHeight/2};
    drawSegment(p1, p2);

    Coords p3{x - d*rectLength/2, y + d*rectHeight/2};
    Coords p4{x + d*rectLength/2, y + d*rectHeight/2};
    drawSegment(p3, p4);

    Coords p5{x - d*rectLength/2, y - d*rectHeight/2};
    Coords p6{x - d*rectLength/2, y + d*rectHeight/2};
    drawSegment(p5, p6);

    Coords p8{x + d*rectLength/2, y - d*rectHeight/2};
    Coords p7{x + d*rectLength/2, y + d*rectHeight/2};
    drawSegment(p7, p8);
}

//Static Dijkstra methods definition
void initTA(vector<unsigned int>& ta, const vector<Noeud*>& tn) {
    for(unsigned int i = 0; i < tn.size(); ++i) {
        ta.push_back(i);
    }
}

void sortTA(vector<unsigned int>& ta, const vector<Noeud*>& tn) {
    //we want to sort ta, using tn[i]->getAccess() values
    vector<double> temp;
    for(unsigned int i = 0; i < tn.size(); ++i) {
        temp.push_back(tn[i]->getAccess());
    } //setup a temporary vector which will contain the access values

    vector<unsigned int> tempTA;
    initTA(tempTA, tn); //since we are doing a full sort, we need a new TA vector

    for(unsigned int i = 1; i < temp.size(); ++i) {  
        double key = temp[i]; 
        int key2 = tempTA[i];
        int j = i - 1;  

        while((j >= 0) and (key <= temp[j])) {
            temp[j + 1] = temp[j];   
            tempTA[j + 1] = tempTA[j]; //sneakily sort ta at the same time as temp
            --j;  
        }

        temp[j + 1] = key;  
        tempTA[j + 1] = key2;
    }

    ta = tempTA;
} //insertion sort but adapted to this project

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
    cout << "if i can see this message it means i fucked up, findMinAccess() doesn't work" << endl;
    return 0;
} //its ok to not have a return here, this function is only called while AT LEAST ONE in = true

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

    cout << "this is my link value: " << distance/speed << endl;

    return distance/speed;
}
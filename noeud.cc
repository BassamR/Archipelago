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
    uid(uid), size(size), in(true), parent(no_link), access(infinite_time) {
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

vector<Noeud*> Noeud::getLiens() const {
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

unsigned int Noeud::getParent() {
    return parent;
}

void Noeud::setParent(unsigned int value) {
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
    if(getSize() < min_capacity) {
        cout << error::too_little_capacity(getSize()) << endl;
        return true;
    }

    if(getSize() > max_capacity) {
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

void Noeud::dijkstra(std::vector<Noeud*>& tn, std::string nodeType) {
    return;
}

void Noeud::updateShortestPath(vector<Noeud*> ensemble, unsigned int goal, string type) {
    return;   
}

double Noeud::mtaHP() {
    return 0;
}

double Noeud::mtaHT() {
    return 0;
}

//Housing methods:
string Housing::getType() {
    return "housing";
}

void Housing::draw() {
    cout << "housing draw called" << endl;
    setColor(BLACK);
    
    drawCircle(position);
}

bool Housing::testMaxLink() {
    if(liens.empty() == true) return false;
    if(liens.size() > max_link) return true;
    return false;
}

void Housing::dijkstra(vector<Noeud*>& tn, string nodeType) {
    cout << "dijkstra called on a node" << endl;
    //initialize
    for(unsigned int i = 0; i < tn.size(); ++i) {
        tn[i]->setIn(true);
        tn[i]->setAccess(infinite_time);
        tn[i]->setParent(no_link);
    }
    access = 0; //current node = beginning node
    vector<unsigned int> ta;
    initTA(ta, tn);
    cout << "init ta to be sure: " << endl;
    for(unsigned int i = 0; i < ta.size(); ++i) {
        cout << ta[i] << " ";
    }
    cout << endl;
    sortTA(ta, tn);
    cout << "sorted ta to be sure: " << endl;
    for(unsigned int i = 0; i < ta.size(); ++i) {
        cout << ta[i] << " ";
    }
    cout << endl;

    //main loop
    while(not tnEmpty(tn)) {
        cout << "while loop called (ie findminaccess is set)" << endl;
        unsigned int n = findMinAccess(ta, tn);
        cout << "i calculated this value for n: " << n << endl;
        if(tn[n]->getType() == nodeType) {
            cout << "DIJKSTRA DONE, FINAL INDEX: " << n << endl;
            if(nodeType == "transport") updateShortestPath(tn, n, "transport");
            if(nodeType == "production") updateShortestPath(tn, n, "production");
            return;
        }

        cout << "this node's in value: " << tn[n]->getIn() << endl;

        tn[n]->setIn(false); //we dealt with this node
        cout << "this node's in value: " << tn[n]->getIn() << endl;
        //if we're checking a production, do nothing because not allowed to pass through production
        //ie a production "has no links"
        if(not (tn[n]->getType() == "production")) {
            cout << "link size: " << tn[n]->getLiens().size() << endl;
            for(unsigned int i = 0; i < tn[n]->getLiens().size(); ++i) {
                cout << "we started to check links" << endl;
                if(tn[n]->getLiens()[i]->getIn() == true) { //if node has not been checked
                    double alt = tn[n]->getAccess() + linkValue(tn[n]->getLiens()[i], tn[n]);
                    if(tn[n]->getLiens()[i]->getAccess() > alt) { //if new road > old road
                        tn[n]->getLiens()[i]->setAccess(alt);
                        tn[n]->getLiens()[i]->setParent(n); //update old road
                        sortTA(ta, tn);
                        cout << "a step of dijkstra is done, here's the sorted ta" << endl;
                        for(unsigned int i = 0; i < ta.size(); ++i) {
                            cout << ta[i] << " ";
                        }
                        cout << endl;
                    }
                }

            } //end of for loop for TN[n]'s links

        } //end of big if (for production)

    } //end of while

    return;
}

void Housing::updateShortestPath(const std::vector<Noeud*>& ensemble, unsigned int goal, string type) {
    cout << "im a recursive function being called" << endl;

    if(type == "production") {
        cout << "type is production for recursive" << endl;
        cout << "ensemble[goal]->getParent() is: " << ensemble[goal]->getParent() << endl;
        if(ensemble[goal]->getParent() != no_link) {
            shortestPathToProd.push_back(ensemble[goal]);
            cout << "just pushed back" << endl;
            updateShortestPath(ensemble, ensemble[goal]->getParent(), "production");
            cout << "im a recursive function being called" << endl;
        }
    } else if(type == "transport") {
        if(ensemble[goal]->getParent() != no_link) {
            shortestPathToTrans.push_back(ensemble[goal]);
            updateShortestPath(ensemble, ensemble[goal]->getParent(), "transport");
        }
    } else {
        cout << "im a recursive function that does nothign" << endl;

        return;
    }
}

double Housing::mtaHP() {
    for(unsigned int i = 0; i < shortestPathToProd.size(); ++i) {
        cout << shortestPathToProd[i] << endl;
    }

    if(shortestPathToProd.empty()) return infinite_time;
    double mtaHP(0);
    
    for(unsigned int i = 1; i < shortestPathToProd.size(); ++i) {
        mtaHP += linkValue(shortestPathToProd[i-1], shortestPathToProd[i]);
    }

    //take into account original node
    mtaHP += linkValue(shortestPathToProd.back(), this);

    return mtaHP;
}

double Housing::mtaHT() {
    if(shortestPathToTrans.empty()) return infinite_time;
    double mtaHT(0);
    
    for(unsigned int i = 1; i < shortestPathToTrans.size(); ++i) {
        mtaHT += linkValue(shortestPathToTrans[i-1], shortestPathToTrans[i]);
    }

    //take into account original node
    mtaHT += linkValue(shortestPathToTrans.back(), this);

    return mtaHT;
}

//Transport methods:
string Transport::getType() {
    return "transport";
}

void Transport::draw() {
    cout << "transport draw called" << endl;
    setColor(BLACK);

    drawCircle(position);
    double x = position.centre.x;
    double y = position.centre.y;
    double r = position.rayon;

    //dessin des traits
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
    cout << "production draw called" << endl;
    setColor(BLACK);

    drawCircle(position);
    double x = position.centre.x;
    double y = position.centre.y;
    double d = 2 * position.rayon;

    //dessin des traits
    Coords p1{x - d*0.375, y - d*0.0625};
    Coords p2{x + d*0.375, y - d*0.0625};
    drawSegment(p1, p2);

    Coords p3{x - d*0.375, y + d*0.0625};
    Coords p4{x + d*0.375, y + d*0.0625};
    drawSegment(p3, p4);

    Coords p5{x - d*0.375, y - d*0.0625};
    Coords p6{x - d*0.375, y + d*0.0625};
    drawSegment(p5, p6);

    Coords p8{x + d*0.375, y - d*0.0625};
    Coords p7{x + d*0.375, y + d*0.0625};
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

    for(unsigned int i = 1; i < temp.size(); ++i) {  
        double key = temp[i]; 
        int key2 = ta[i];
        int j = i - 1;  

        while((j >= 0) and (key < temp[j])) {  //maybe key <= temp[j]?
            temp[j + 1] = temp[j];   
            ta[j + 1] = ta[j]; //sneakily sort ta at the same time as tmp
            --j;  
        }

        temp[j + 1] = key;  
        ta[j + 1] = key2;
    }
} //insertion sort but adapted to this project
//problem is in this sort

bool tnEmpty(const vector<Noeud*>& tn) {
    for(unsigned int i = 0; i < tn.size(); ++i) {
        if(tn[i]->getIn()) return false; //there exists an in that is true
        //=> TN is not "empty"
    }
    return true;
}

unsigned int findMinAccess(const vector<unsigned int>& ta, const vector<Noeud*>& tn) {
    for(unsigned int i = 0; i < ta.size(); ++i) {
        if(tn[ta[i]]->getIn() == true) {
            cout << i << " is my findminaccess" << endl;
            return ta[i];
        }
        //first element of TA that has in = true
    }
    cout << "if i can see this message it means i fucked up, findMinAccess() doesn't work" << endl;
    return 0;
} //its ok to not have a return here, this function is only called while AT LEAST ONE in = true
//this function counts on the fact that TA is sorted, it won't actually find a minimum
//so fix the fucking sort then !!!


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
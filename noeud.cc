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
#include "noeud.h"
#include "error.h"
#include "tools.h"
#include "constantes.h"
using namespace std;

//Constructors:
Noeud::Noeud(unsigned int uid, double x, double y, unsigned int size):
    uid(uid), size(size) {
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

//Housing methods:
string Housing::getType() {
    return "housing";
}

void Housing::draw() {
}

bool Housing::testMaxLink() {
    if(liens.empty() == true) return false;
    if(liens.size() > max_link) return true;
    return false;
}

//Transport methods:
string Transport::getType() {
    return "transport";
}

void Transport::draw() {
}

//Production methods:
string Production::getType() {
    return "production";
}

void Production::draw() {
}
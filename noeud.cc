#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "noeud.h"
#include "error.h"
#include "tools.h"
#include "constantes.h"
using namespace std;

//Noeud functions:
Noeud::Noeud(unsigned int uid, double x, double y, unsigned int size, string type):
    uid(uid), size(size), type(type) {
    position.centre.x = x; position.centre.y = y; position.rayon = sqrt(size);
}

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

string Noeud::getType() {
    return type;
}

void Noeud::setType(string valeur) {
    type = valeur;
}

vector<unsigned int> Noeud::getLiens() {
    return liens;
}

void Noeud::setLiens(unsigned int linkUid) {
    liens.push_back(linkUid);
}

//Error functions for creating nodes:
bool Noeud::testIdenticalUid(vector<Noeud> ensembleNoeuds) {
    if(ensembleNoeuds.empty() == true) {
        return false;
    }
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i].getUid()) {
            cout << error::identical_uid(uid) << endl;
            return true;
        }
    }
    return false;
}

bool Noeud::testNodeNodeSuperposition(std::vector<Noeud> ensembleNoeuds) {
    if(ensembleNoeuds.empty() == true) {
        return false;
    }
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(intersectionCC(getPosition(), ensembleNoeuds[i].getPosition())) {
            cout << error::node_node_superposition(getUid(), ensembleNoeuds[i].getUid()) << endl;
            return true;
        }
    }
    return false;
}

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

bool Noeud::testNodeValidity(vector<Noeud> ensembleNoeuds) {
    bool notValid = testIdenticalUid(ensembleNoeuds) or testReservedUid() or testCapacityProblem() 
        or testNodeNodeSuperposition(ensembleNoeuds);
    if(notValid == true) {
        return false; //noeud IS NOT valid
    }
    return true;
} //runs before creating a node
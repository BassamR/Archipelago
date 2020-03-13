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
    uid(uid), type(type) {
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
    return position.rayon * position.rayon;
}

void Noeud::setSize(unsigned int x) {
    position.rayon = sqrt(x);
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

//Error functions:
bool Noeud::testIdenticalUid(vector<Noeud> ensembleNoeuds) { //before creating 1 node
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

bool Noeud::testNodeLinkSuperposition(Noeud obj1, Noeud obj2, Noeud obj3) { //pb, when to run?
    if(intersectionCS(obj1.getPosition(), obj2.getPosition(), obj3.getPosition())) {
        cout << error::node_link_superposition(obj3.getUid()) << endl;
        return true;
    }
    return false;
}

bool Noeud::testNodeNodeSuperposition(Noeud obj1, Noeud obj2) { //pb, when to run?
    if(intersectionCC(obj1.getPosition(), obj2.getPosition())) {
        cout << error::node_node_superposition(obj1.getUid(), obj2.getUid()) << endl;
        return true;
    }
    return false;
}

bool Noeud::testReservedUid() { //before creating 1 node
    if(uid == no_link) {
        cout << error::reserved_uid() << endl;
        return true;
    }
    return false;
}

bool Noeud::testCapacityProblem() { //before creating 1 node
    if(getSize() <= min_capacity) {
        cout << error::too_little_capacity(getSize()) << endl;
        return true;
    }

    if(getSize() >= max_capacity) {
        cout << error::too_much_capacity(getSize()) << endl;
        return true;
    }

    return false;
}

bool Noeud::testNodeValidity(vector<Noeud> ensembleNoeuds) { //regroups all
    bool notValid = testIdenticalUid(ensembleNoeuds) or testReservedUid() or testCapacityProblem();
    if(notValid == true) {
        return false; //noeud IS NOT valid
    }
    return true;
}
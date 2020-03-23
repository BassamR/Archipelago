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

vector<Noeud*> Noeud::getLiens() {
    return liens;
}

void Noeud::setLiens(Noeud* linkUid) {
    liens.push_back(linkUid);
}

//Error functions unique to one node:
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
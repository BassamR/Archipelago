#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "noeud.h"
#include "error.h"
#include "tools.h"
#include "constantes.h"
using namespace std;

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
    return position.rayon;
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
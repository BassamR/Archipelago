#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "noeud.h"
#include "error.h"
#include "tools.h"
#include "constantes.h"
using namespace std;

Noeud::Noeud(unsigned int uid, Cercle coord, unsigned int size, string type):
    uid(uid), size(size), type(type) {
    coords.x = coord.x; coords.y = coord.y;
}

unsigned int Noeud::getUid() {
    return uid;
}

void Noeud::setUid(unsigned int x) {
    uid = x;
}

Cercle Noeud::getCoords() {
    return coords;
}

void Noeud::setCoords(Cercle cer) {
    coords.x = cer.x;
    coords.y = cer.y;
}

unsigned int Noeud::getSize() {
    return size;
}

void Noeud::setSize(unsigned int x) {
    size = sqrt(x);
}

string Noeud::getType() {
    return type;
}

void Noeud::setType(string valeur) {
    if(valeur != "Logement " or "Transport" or "Production") {
        cout << "Type incorrect" << endl;
    } else { //this control might be useless, depends on how ville is structured
        type = valeur;
    }
}

vector<unsigned int> Noeud::getLiens() {
    return liens;
}

void Noeud::setLiens(unsigned int linkUid) {
    liens.push_back(linkUid);
}
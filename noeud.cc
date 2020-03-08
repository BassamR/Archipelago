#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "noeud.h"
#include "error.h"
#include "tools.h"
#include "constantes.h"
using namespace std;

vector<Noeud> ensembleNoeuds;
vector<unsigned int> uidIndex;
vector<unsigned int> emptyVector; //used to initalize obj.liens

Noeud::Noeud(unsigned int uid, Cercle coord, unsigned int size, string type, vector<unsigned int> liens):
    uid(uid), type(type), size(size), liens(liens) {
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

void createNoeud(unsigned int uid, Cercle coord, unsigned int size, string type) {
    Noeud obj(uid, coord, size, type, emptyVector);
    ensembleNoeuds.push_back(obj);
    uidIndex.push_back(uid);
    //obj.~delete(); ?
}

//given a uid, find the node's index in ensembleNoeud:
unsigned int findNoeudIndex(unsigned int uid) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i].getUid()) {
            return i;
        }
    }
    cout << "no uid found" << endl;
    return 0;
}

void createLien(unsigned int uid1, unsigned int uid2) {
    ensembleNoeuds[findNoeudIndex(uid1)].setLiens(uid2); //adds uid2 to uid1's list of links
    ensembleNoeuds[findNoeudIndex(uid2)].setLiens(uid1); //adds uid1 to uid2's list of links
}
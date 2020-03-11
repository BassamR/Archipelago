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
    unsigned int size = position.rayon * position.rayon;
    return size;
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

bool errorFunctions::identicalUid(vector<Noeud> ensembleNoeuds, unsigned int uid) { //before creating 1 node
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i].getUid()) {
            cout << error::identical_uid(uid) << endl;
            return true;
        }
    }
    return false;
}

bool errorFunctions::linkVacuum(unsigned int uid1, unsigned int uid2, vector<Noeud> ensembleNoeuds) { //before creating 1 link
    bool uid1Existe(false), uid2Existe(false);
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid1 == ensembleNoeuds[i].getUid()) {
            uid1Existe = true;
        }
        if(uid2 == ensembleNoeuds[i].getUid()) {
            uid2Existe = true;
        }
    }

    if((uid1Existe and uid2Existe) == false) {
        if(uid1Existe == false) {
            cout << error::link_vacuum(uid1) << endl;
        } else {
            cout << error::link_vacuum(uid2) << endl;
        }
        return true;
    }
    return false;
}

bool errorFunctions::maxLink(Noeud obj) { //after creating all links
    if(obj.getLiens().size() > max_link) {
        cout << error::max_link(obj.getUid()) << endl;
        return true;
    }
    return false;
}

bool errorFunctions::multipleSameLink(unsigned int uid1, unsigned int uid2, vector<vector<unsigned int>> liens) { //before creating 1 link
    for(unsigned int i = 0; i < liens[0].size(); ++i) {
        if(liens[i][0] == uid1 and liens[i][1] == uid2) {
            cout << error::multiple_same_link(uid1, uid2) << endl;
            return true;
        }
    }
    return false;
}

bool errorFunctions::nodeLinkSuperposition(Noeud obj1, Noeud obj2, Noeud obj3) { //pb, when to run?
    if(intersectionCS(obj1.getPosition(), obj2.getPosition(), obj3.getPosition())) {
        cout << error::node_link_superposition(obj3.getUid()) << endl;
        return true;
    }
    return false;
}

bool errorFunctions::nodeNodeSuperposition(Noeud obj1, Noeud obj2) { //pb, when to run?
    if(intersectionCC(obj1.getPosition(), obj2.getPosition())) {
        cout << error::node_node_superposition(obj1.getUid(), obj2.getUid()) << endl;
        return true;
    }
    return false;
}

bool errorFunctions::reservedUid(Noeud obj) { //before creating 1 node
    if(obj.getUid() == no_link) {
        cout << error::reserved_uid() << endl;
        return true;
    }
    return false;
}

bool errorFunctions::selfLinkNode(unsigned int uid1, unsigned int uid2) { //before creating 1 link
    if(uid1 == uid2) {
        cout << error::self_link_node(uid1) << endl;
        return true;
    }
    return false;
}

bool errorFunctions::capacityProblem(Noeud obj) { //before creating 1 node
    if(obj.getSize() <= min_capacity) {
        cout << error::too_little_capacity(obj.getSize()) << endl;
        return true;
    }

    if(obj.getSize() >= max_capacity) {
        cout << error::too_much_capacity(obj.getSize()) << endl;
        return true;
    }

    return false;
}
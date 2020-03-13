#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "ville.h"
#include "noeud.h"
#include "tools.h"
#include "error.h"
#include "constantes.h"
using namespace std;

namespace {
    Ville ville; //main ville
}

//Reading functions
void lecture(char* nomFichier) {
}

//Ville functions
void Ville::createNoeud(unsigned int uid, double x, double y, unsigned int size, string type) {
    Noeud obj(uid, x, y, size, type);

    if(obj.testNodeValidity(ensembleNoeuds) == false) {
        cout << "temporary. the noeud is bad" << endl;
    } else {
        ensembleNoeuds.push_back(obj);
        uidIndex.push_back(uid);
    }
}

Noeud Ville::getNoeud(unsigned int index) {
    return ensembleNoeuds[index];
}

unsigned int Ville::findNoeudIndex(unsigned int uid) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i].getUid()) {
            return i;
        }
    }
    cout << "No uid found" << endl;
    return 0;
} //given a uid, find the node's index in ensembleNoeud:

void Ville::createLien(unsigned int uid1, unsigned int uid2) {
    if(testLinkValidity(uid1, uid2) == false) {
        cout << "temporary. link is bad" << endl;
    } else {
        liens.push_back(vector<unsigned int>{uid1, uid2}); //add both links to Ville's vector<vector>> of links
        ensembleNoeuds[findNoeudIndex(uid1)].setLiens(uid2); //adds uid2 to uid1's list of links
        ensembleNoeuds[findNoeudIndex(uid2)].setLiens(uid1); //adds uid1 to uid2's list of links
    }
}

//Error functions for creating links
bool Ville::testLinkVacuum(unsigned int uid1, unsigned int uid2) { //before creating 1 link
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

bool Ville::testMultipleSameLink(unsigned int uid1, unsigned int uid2) { //before creating 1 link
    for(unsigned int i = 0; i < liens[0].size(); ++i) {
        if(liens[i][0] == uid1 and liens[i][1] == uid2) {
            cout << error::multiple_same_link(uid1, uid2) << endl;
            return true;
        }
    }
    return false;
}

bool Ville::testSelfLinkNode(unsigned int uid1, unsigned int uid2) { //before creating 1 link
    if(uid1 == uid2) {
        cout << error::self_link_node(uid1) << endl;
        return true;
    }
    return false;
}

bool Ville::testMaxLink() { //after creating all links
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(ensembleNoeuds[i].getLiens().size() > max_link) {
            cout << error::max_link(ensembleNoeuds[i].getUid()) << endl;
            return true;
        }
    }
    return false;
}

bool Ville::testLinkValidity(unsigned int uid1, unsigned int uid2) {   
    bool notValid = testLinkVacuum(uid1, uid2) or testMultipleSameLink(uid1, uid2) or testSelfLinkNode(uid1, uid2);
    if(notValid == true) {
        return false;
    }
    return true;
}
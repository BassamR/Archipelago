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

void lecture(char* nomFichier) {
}

void Ville::createNoeud(unsigned int uid, double x, double y, unsigned int size, string type) {
    //add error functions to this. if no errors, create
    //might need to change parameters in that case
    //Noeud obj(uid, x, y, size, type);
    ensembleNoeuds.push_back({uid, x, y, size, type});
    uidIndex.push_back(uid);
}

Noeud Ville::getNoeud(unsigned int index) {
    return ensembleNoeuds[index];
}

//given a uid, find the node's index in ensembleNoeud:
unsigned int Ville::findNoeudIndex(unsigned int uid) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i].getUid()) {
            return i;
        }
    }
    cout << "No uid found" << endl;
    return 0;
}

void Ville::createLien(unsigned int uid1, unsigned int uid2) {
    //add error functions to this. if no error, create
    //might need to change parameters in that case
    liens.push_back(vector<unsigned int>{uid1, uid2}); //add both links to Ville's vector<vector>> of links
    ensembleNoeuds[findNoeudIndex(uid1)].setLiens(uid2); //adds uid2 to uid1's list of links
    ensembleNoeuds[findNoeudIndex(uid2)].setLiens(uid1); //adds uid1 to uid2's list of links
}
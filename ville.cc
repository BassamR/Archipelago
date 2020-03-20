//ville.cc
//Version 1.0
//Auteurs: Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
#include <iostream>
#include <fstream>
#include <sstream>
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
    Ville ville;
}

//Reading functions
void lecture(char* nomFichier) {
     string line;
     ifstream fichier(nomFichier);
     if(not fichier.fail()) {
        while(getline(fichier >> ws, line)) { //check documentation for max_line
            if(line[0] == '#') continue;
            decodageLigne(line);
        }
    } else {
        cout << "Lecture du fichier impossible" << endl; //temp
    }
}

void decodageLigne(string line) {
    istringstream data(line);

    enum Etat_lecture {NBH, HOUSING, NBT, TRANSPORT, NBP, PRODUCTION, NBL, LINK, FIN};
    static int etat(NBH); //etat initial
    static int i(0), total(0); //i: compteur, total: nbh, nbt...

    unsigned int uid, size, uid1, uid2;
    double x, y;

    switch(etat) {
        case NBH:
            data >> total; i = 0;
            if(total == 0) etat = NBT;
            else etat = HOUSING;
            break;

        case HOUSING:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = NBT;
            ville.createNoeud(uid, x, y, size, "housing");
            break;
        
        case NBT:
            data >> total; i = 0;
            if(total == 0) etat = NBP;
            else etat = TRANSPORT;
            break;

        case TRANSPORT:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = NBP;
            ville.createNoeud(uid, x, y, size, "transport");
            break;

        case NBP:
            data >> total; i = 0;
            if(total == 0) etat = NBL;
            else etat = PRODUCTION;
            break;

        case PRODUCTION:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = NBL;
            ville.createNoeud(uid, x, y, size, "production");
            break;

        case NBL:
            data >> total; i = 0;
            if(total == 0) etat = FIN;
            else etat = LINK;
            break;

        case LINK:
            data >> uid1 >> uid2; ++i;
            ville.createLien(uid1, uid2);
            if(i == total) {
                etat = FIN;
                if(ville.testMaxLink() == true) exit(EXIT_FAILURE);
                else cout << error::success() << endl;
            }
            break;

        case FIN: cout << "Erreur dans le format du fichier" << endl; break;
        default: cout << "Erreur dans la lecture de l'etat" << endl; break;
    }
}

//Ville functions:
void Ville::createNoeud(unsigned int uid, double x, double y, 
                            unsigned int size, string type) {
    Noeud obj(uid, x, y, size, type);

    if(obj.testNodeValidity(ensembleNoeuds) == false) {
        exit(EXIT_FAILURE);
    } else {
        ensembleNoeuds.push_back(obj);
        uidIndex.push_back(uid);
    }
}

Noeud Ville::getNoeud(unsigned int index) {
    return ensembleNoeuds[index];
}

void Ville::createLien(unsigned int uid1, unsigned int uid2) {
    if(testLinkValidity(uid1, uid2) == false) {
        exit(EXIT_FAILURE);
    } else {
        liens.push_back(vector<unsigned int>{uid1, uid2}); 
        ensembleNoeuds[findNoeudIndex(uid1)].setLiens(uid2); 
        ensembleNoeuds[findNoeudIndex(uid2)].setLiens(uid1); 
    }
}

unsigned int Ville::findNoeudIndex(unsigned int uid) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i].getUid()) {
            return i;
        }
    }
    cout << "No uid found" << endl;
    return 0;
} //given a uid, find the node's index in vector ensembleNoeud

//Error functions for creating links:
bool Ville::testLinkVacuum(unsigned int uid1, unsigned int uid2) {
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

bool Ville::testMultipleSameLink(unsigned int uid1, unsigned int uid2) {
    if(liens.empty() == true) {
        return false;
    }
    
    for(unsigned int i = 0; i < liens.size(); ++i) {
        if((liens[i][0] == uid1 and liens[i][1] == uid2) or 
            (liens[i][0] == uid2 and liens[i][1] == uid1)) {
            cout << error::multiple_same_link(uid1, uid2) << endl;
            return true;
        }
    }
    return false;
}

bool Ville::testSelfLinkNode(unsigned int uid1, unsigned int uid2) {
    if(uid1 == uid2) {
        cout << error::self_link_node(uid1) << endl;
        return true;
    }
    return false;
}

bool Ville::testNodeLinkSuperposition(unsigned int uid1, unsigned int uid2) {
    Cercle posNoeud1 = ensembleNoeuds[findNoeudIndex(uid1)].getPosition();
    Cercle posNoeud2 = ensembleNoeuds[findNoeudIndex(uid2)].getPosition();

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(intersectionCS(posNoeud1, posNoeud2, ensembleNoeuds[i].getPosition())) {
            unsigned int badUid = ensembleNoeuds[i].getUid();
            cout << error::node_link_superposition(badUid) << endl;
            return true;
        }
    }

    return false;
}

bool Ville::testLinkValidity(unsigned int uid1, unsigned int uid2) {   
    bool notValid = testLinkVacuum(uid1, uid2) or testMultipleSameLink(uid1, uid2) 
        or testSelfLinkNode(uid1, uid2) or testNodeLinkSuperposition(uid1, uid2);
    if(notValid == true) {
        return false;
    }
    return true;
} //runs before creating a link

bool Ville::testMaxLink() {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(ensembleNoeuds[i].getType() == "housing" and 
                ensembleNoeuds[i].getLiens().size() > max_link) {
            cout << error::max_link(ensembleNoeuds[i].getUid()) << endl;
            return true;
        }
    }
    return false;
} //after creating all links
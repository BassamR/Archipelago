/**
* \name ville.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date Mars 2020
* \version 1.0
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "ville.h"
#include "noeud.h"
#include "tools.h"
#include "error.h"
#include "constantes.h"
using namespace std;

//Declaration of local variables/functions:
static void decodageLigne(string line);
static Ville ville;

//Reading functions
void lecture(char* nomFichier) {
    string line;
    ifstream fichier(nomFichier);
    if(not fichier.fail()) {
        while(getline(fichier >> ws, line)) {
            if(line[0] == '#') continue;
            decodageLigne(line);
        }
    } else {
        cout << "Lecture du fichier impossible" << endl;
    }
}

static void decodageLigne(string line) {
    istringstream data(line);

    enum EtatLecture {NBH, HOUSING, NBT, TRANSPORT, NBP, PRODUCTION, NBL, LINK, FIN};
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
            ville.createHousing(uid, x, y, size);
            break;

        case NBT:
            data >> total; i = 0;
            if(total == 0) etat = NBP;
            else etat = TRANSPORT;
            break;

        case TRANSPORT:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = NBP;
            ville.createTransport(uid, x, y, size);
            break;

        case NBP:
            data >> total; i = 0;
            if(total == 0) etat = NBL;
            else etat = PRODUCTION;
            break;

        case PRODUCTION:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = NBL;
            ville.createProduction(uid, x, y, size);
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
void Ville::createHousing(unsigned int uid, double x, double y, unsigned int size) {
    Housing* newHouse = new Housing(uid, x, y, size);

    if(ville.testNodeValidity(newHouse) == false) {
        exit(EXIT_FAILURE);
    } else {
        ensembleNoeuds.push_back(newHouse);
    }
}

void Ville::createProduction(unsigned int uid, double x, double y, unsigned int size) {
    Production* newProd = new Production(uid, x, y, size);

    if(ville.testNodeValidity(newProd) == false) {
        exit(EXIT_FAILURE);
    } else {
        ensembleNoeuds.push_back(newProd);
    }
}

void Ville::createTransport(unsigned int uid, double x, double y, unsigned int size) {
    Transport* newTrans = new Transport(uid, x, y, size);

    if(ville.testNodeValidity(newTrans) == false) {
        exit(EXIT_FAILURE);
    } else {
        ensembleNoeuds.push_back(newTrans);
    }
}

void Ville::createLien(unsigned int uid1, unsigned int uid2) {
     if(testLinkValidity(uid1, uid2) == false) {
         exit(EXIT_FAILURE);
     } else {
        unsigned int index1 = findNoeudIndex(uid1);
        unsigned int index2 = findNoeudIndex(uid2);
        Noeud* noeud1 = ensembleNoeuds[index1];
        Noeud* noeud2 = ensembleNoeuds[index2];

        liens.push_back(vector<Noeud*>{noeud1, noeud2});
        ensembleNoeuds[index1]->setLiens(ensembleNoeuds[index2]);
        ensembleNoeuds[index2]->setLiens(ensembleNoeuds[index1]);
    }
}

unsigned int Ville::findNoeudIndex(unsigned int uid) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i]->getUid()) {
            return i;
        }
    }
    cout << "No uid found" << endl;
    return 0;
} //given a uid, find the node's index in vector ensembleNoeud

//Error functions (nodes):
bool Ville::testIdenticalUid(Noeud* testNoeud) {
    if(ensembleNoeuds.empty() == true) {
        return false;
    }

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(testNoeud->getUid() == ensembleNoeuds[i]->getUid()) {
            cout << error::identical_uid(testNoeud->getUid()) << endl;
            return true;
        }
    }

    return false;
}

bool Ville::testNodeNodeSuperposition(Noeud* testNoeud) {
    if(ensembleNoeuds.empty() == true) {
        return false;
    }

     for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
         if(intersectionCC(testNoeud->getPosition(), 
             ensembleNoeuds[i]->getPosition())) {
            cout << error::node_node_superposition(testNoeud->getUid(), 
                ensembleNoeuds[i]->getUid()) << endl;
            return true;
        }
    }

    return false;
}

bool Ville::testNodeValidity(Noeud* testNoeud) {
    bool notValid = testNoeud->testCapacityProblem() or testNoeud->testReservedUid()
            or testIdenticalUid(testNoeud) or testNodeNodeSuperposition(testNoeud);

    if(notValid == true) {
        return false; //noeud IS NOT valid
    }
    
    return true;
} //runs before creating a node

//Error functions (links):
bool Ville::testLinkVacuum(unsigned int uid1, unsigned int uid2) {
    bool uid1Existe(false), uid2Existe(false);

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid1 == ensembleNoeuds[i]->getUid()) uid1Existe = true;
        if(uid2 == ensembleNoeuds[i]->getUid()) uid2Existe = true;
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
        if((liens[i][0]->getUid() == uid1 and liens[i][1]->getUid() == uid2) or
            (liens[i][0]->getUid() == uid2 and liens[i][1]->getUid() == uid1)) {
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
    Cercle posNoeud1 = ensembleNoeuds[findNoeudIndex(uid1)]->getPosition();
    Cercle posNoeud2 = ensembleNoeuds[findNoeudIndex(uid2)]->getPosition();

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(intersectionCS(posNoeud1, posNoeud2, ensembleNoeuds[i]->getPosition())) {
            unsigned int badUid = ensembleNoeuds[i]->getUid();
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
        if(ensembleNoeuds[i]->testMaxLink() == true) {
            cout << error::max_link(ensembleNoeuds[i]->getUid()) << endl;
            return true;
        }
    }
    return false;
} //after creating all links
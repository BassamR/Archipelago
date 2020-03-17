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
    Ville ville; //main ville
}

//Reading functions
//TODO: max_line, ainsi que les # en milieu de ligne
void lecture(char* nomFichier) {
     string line;
     ifstream fichier(nomFichier);
     if(not fichier.fail()) {
        // l’appel de getline filtre aussi les séparateurs
        while(getline(fichier >> ws, line)) {
            if(line[0] == '#') continue; // ligne de commentaire à ignorer, on passe à la suivante
            decodageLigne(line);
        }
        cout << error::success() << endl;
    } else {
        cout << "lecture du fichier impossible" << endl; //temp
    }
}

void decodageLigne(string line) {
    istringstream data(line);

    enum Etat_lecture {nbh, housing, nbt, transport, nbp, production, nbl, link, fin};
    static int etat(nbh); // état initial
    static int i(0), total(0); //i: compteur, total: represente nbh, nbt...

    unsigned int uid, size;
    double x, y;
    unsigned int uid1, uid2;

    switch(etat) {
        case nbh:
            data >> total; i = 0;
            if(total == 0) etat = nbt;
            else etat = housing;
            cout << "nb housing: " << total << endl; //temp, just to keep track of stuff
            break;

        case housing:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = nbt;
            cout << "house: " << uid << " " << x << " " << y << " " << size << endl; //temp
            ville.createNoeud(uid, x, y, size, "housing");
            break;
        
        case nbt:
            data >> total; i = 0;
            if(total == 0) etat = nbp;
            else etat = transport;
            cout << "nb transport: " << total << endl; //temp
            break;

        case transport:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = nbp;
            cout << "transport: " << uid << " " << x << " " << y << " " << size << endl; //temp
            ville.createNoeud(uid, x, y, size, "transport");
            break;

        case nbp:
            data >> total; i = 0;
            if(total == 0) etat = nbl;
            else etat = production;
            cout << "nb production: " << total << endl; //temp
            break;

        case production:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = nbl;
            cout << "production: " << uid << " " << x << " " << y << " " << size << endl; //temp
            ville.createNoeud(uid, x, y, size, "production");
            break;

        case nbl:
            data >> total; i = 0;
            if(total == 0) etat = fin;
            else etat = link;
            cout << "nb de liens: " << total << endl; //temp
            break;

        case link:
            data >> uid1 >> uid2; ++i;
            if(i == total) etat = fin;
            cout << "lien: " << uid1 << " " << uid2 << endl; //temp
            ville.createLien(uid1, uid2);
            break;

        case fin:
            break;
        
        default: cout << "erreur dans la lecture de l'etat" << endl;
    }
}

//Ville functions
void Ville::createNoeud(unsigned int uid, double x, double y, unsigned int size, string type) {
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

unsigned int Ville::findNoeudIndex(unsigned int uid) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i].getUid()) {
            return i;
        }
    }
    cout << "No uid found" << endl;
    return 0;
} //given a uid, find the node's index in vector ensembleNoeud

void Ville::createLien(unsigned int uid1, unsigned int uid2) {
    if(testLinkValidity(uid1, uid2) == false) {
        exit(EXIT_FAILURE);
    } else {
        liens.push_back(vector<unsigned int>{uid1, uid2}); //add both links to Ville's vector<vector> of links
        ensembleNoeuds[findNoeudIndex(uid1)].setLiens(uid2); //adds uid2 to uid1's list of links
        ensembleNoeuds[findNoeudIndex(uid2)].setLiens(uid1); //adds uid1 to uid2's list of links
    }
}

//Error functions for creating links
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
        if((liens[i][0] == uid1 and liens[i][1] == uid2) or (liens[i][0] == uid2 and liens[i][1] == uid1)) {
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
    unsigned int index1 = findNoeudIndex(uid1);
    unsigned int index2 = findNoeudIndex(uid2);
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(intersectionCS(ensembleNoeuds[index1].getPosition(), ensembleNoeuds[index2].getPosition(), ensembleNoeuds[i].getPosition())) {
            cout << error::node_link_superposition(ensembleNoeuds[i].getUid()) << endl;
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

bool Ville::testMaxLink() { //after creating all links
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(ensembleNoeuds[i].getLiens().size() > max_link) {
            cout << error::max_link(ensembleNoeuds[i].getUid()) << endl;
            return true;
        }
    }
    return false;
}
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
static void initVille(string line);
static Ville ville;
static void outputNode(ofstream& str, vector<Noeud*> nodes);

//Functions used by gui:
void drawCity() {
    ville.drawLinks();
    ville.drawNodes();
}

void deleteCity() {
    ville.resetVille();
}

void saveCity() {
    ville.saveVille();
}

//Reading/Outputting functions:
void lecture(char* nomFichier) {
    string line;
    ifstream fichier(nomFichier);
    if(not fichier.fail()) {
        while(getline(fichier >> ws, line)) {
            if(line[0] == '#') continue;
            initVille(line);
        }
    } else {
        cout << "Lecture du fichier impossible" << endl;
    }
}

static void initVille(string line) {
    istringstream data(line);

    enum EtatLecture {NBH, HOUSING, NBT, TRANSPORT, NBP, PRODUCTION, NBL, LINK, END};
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
            if(not ville.createHousing(uid, x, y, size)) etat = END;
            break;

        case NBT:
            data >> total; i = 0;
            if(total == 0) etat = NBP;
            else etat = TRANSPORT;
            break;

        case TRANSPORT:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = NBP;
            if(not ville.createTransport(uid, x, y, size)) etat = END;
            break;

        case NBP:
            data >> total; i = 0;
            if(total == 0) etat = NBL;
            else etat = PRODUCTION;
            break;

        case PRODUCTION:
            data >> uid >> x >> y >> size; ++i;
            if(i == total) etat = NBL;
            if(not ville.createProduction(uid, x, y, size)) etat = END;
            break;

        case NBL:
            data >> total; i = 0;
            if(total == 0) etat = END;
            else etat = LINK;
            break;

        case LINK:
            data >> uid1 >> uid2; ++i;
            if(not ville.createLien(uid1, uid2)) etat = END;
            if(i == total) {
                etat = END;
                if(ville.testMaxLink() == true) ville.resetVille();
                else cout << error::success() << endl;
            }
            break;

        case END: break;
        default: cout << "Erreur dans la lecture de l'etat" << endl; break;
    }
}

static void outputNode(ofstream& str, vector<Noeud*> nodes) {
    for(unsigned int i = 0; i < nodes.size(); ++i) {
        str << "\t" << nodes[i]->getUid() << " " << nodes[i]->getCoords().x << " ";
        str << nodes[i]->getCoords().y << " " << nodes[i]->getSize() << "\n";
    }
    str << "\n" << "\n";
}

//Ville functions:
bool Ville::createHousing(unsigned int uid, double x, double y, unsigned int size) {
    Housing* newHouse = new Housing(uid, x, y, size);

    if(newHouse->testNodeValidity(ensembleNoeuds) == false) {
        resetVille();
        return false;
    } else {
        ensembleNoeuds.push_back(newHouse);
        return true;
    }
}

bool Ville::createProduction(unsigned int uid, double x, double y, unsigned int size) {
    Production* newProd = new Production(uid, x, y, size);

    if(newProd->testNodeValidity(ensembleNoeuds) == false) {
        resetVille();
        return false;    
    } else {
        ensembleNoeuds.push_back(newProd);
        return true;
    }
}

bool Ville::createTransport(unsigned int uid, double x, double y, unsigned int size) {
    Transport* newTrans = new Transport(uid, x, y, size);

    if(newTrans->testNodeValidity(ensembleNoeuds) == false) {
        resetVille();
        return false;   
    } else {
        ensembleNoeuds.push_back(newTrans);
        return true;
    }
}

bool Ville::createLien(unsigned int uid1, unsigned int uid2) {
     if(testLinkValidity(uid1, uid2) == false) {
        resetVille();
        return false;
     } else {
        unsigned int index1 = findNoeudIndex(uid1);
        unsigned int index2 = findNoeudIndex(uid2);
        Noeud* noeud1 = ensembleNoeuds[index1];
        Noeud* noeud2 = ensembleNoeuds[index2];

        liens.push_back(vector<Noeud*>{noeud1, noeud2});
        ensembleNoeuds[index1]->setLiens(ensembleNoeuds[index2]);
        ensembleNoeuds[index2]->setLiens(ensembleNoeuds[index1]);
        return true;
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
        return false; //link is NOT valid
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

void Ville::drawNodes() {
    if(ensembleNoeuds.empty() == true) {
        cout << "cannot draw empty city" << endl;
        return;
    }

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        ensembleNoeuds[i]->draw();
    }
}

void Ville::drawLinks() {
    if(liens.empty() == true) {
        cout << "cannot draw empty links" << endl;
        return;
    }

    for(unsigned int i = 0; i < liens.size(); ++i) {
        drawSegment(liens[i][0]->getCoords(), liens[i][1]->getCoords());
    }
}

void Ville::saveVille() {
    unsigned int nbH(0), nbT(0), nbP(0), nbL(liens.size());
    vector<Noeud*> housingNodes, transportNodes, productionNodes;

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(ensembleNoeuds[i]->getType() == "housing") {
            ++nbH;
            housingNodes.push_back(ensembleNoeuds[i]);
        } else if(ensembleNoeuds[i]->getType() == "transport") {
            ++nbT;
            transportNodes.push_back(ensembleNoeuds[i]);
        } else {
            ++nbP;
            productionNodes.push_back(ensembleNoeuds[i]);
        }
    }

    ofstream fichier("savedCity.txt");

    if(fichier.is_open()) {
        fichier << "#Saved city" << "\n" << "\n";

        fichier << nbH << " #nb housing" << "\n";
        outputNode(fichier, housingNodes);

        fichier << nbT << " #nb transport" << "\n";
        outputNode(fichier, transportNodes);

        fichier << nbP << " #nb production" << "\n";
        outputNode(fichier, productionNodes);

        fichier << nbL << " #nb links" << "\n";
        for(unsigned int i = 0; i < liens.size(); ++i) {
            fichier << liens[i][0]->getUid() << " " << liens[i][1]->getUid() << "\n";
        }
    }
    fichier.close();
}

void Ville::resetVille() {
    for(unsigned int i = 0; i < liens.size(); ++i) {
        liens[i].clear();
    }
    liens.clear();

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        delete ensembleNoeuds[i];
    }
    ensembleNoeuds.clear();
}
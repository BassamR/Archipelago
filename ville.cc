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
static Ville ville;
static void initVille(string line, bool& continueLecture);
static void outputNode(ofstream& str, vector<Noeud*> nodes);

//Reading/Outputting functions:
void lecture(char* nomFichier) {
    ville.resetVille();
    cout << "lecture called" << endl;

    bool continueLecture = true;
    string line;
    ifstream fichier(nomFichier);
    
    if(not fichier.fail()) {
        while(getline(fichier >> ws, line) and continueLecture) {
            if(line[0] == '#') continue;
            initVille(line, continueLecture);
        }
    } else {
        cout << "Lecture du fichier impossible" << endl;
    }

    fichier.close();
}

static void initVille(string line, bool& continueLecture) {
    istringstream data(line);
    cout << "initville really called" << endl;

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
            if(total == 0) {
                etat = END;
                cout << error::success() << endl;
            }
            else etat = LINK;
            break;

        case LINK:
            data >> uid1 >> uid2; ++i;
            if(not ville.createLien(uid1, uid2)) etat = END;
            if(i == total) {
                etat = NBH;
                i = 0;
                total = 0; //resets the static stuff back to original
                if(ville.testMaxLink() == true) ville.resetVille();
                else cout << error::success() << endl;
            }
            break;

        case END: //when we want to finish with initializing
            etat = NBH;
            i = 0;
            total = 0; //resets static stuff back to original
            continueLecture = false; //stops lecture from calling initville
            break;
        
        default: cout << "Erreur dans la lecture de l'etat" << endl; break;
    }
}

static void outputNode(ofstream& str, vector<Noeud*> nodes) {
    if(nodes.empty()) return;

    for(unsigned int i = 0; i < nodes.size(); ++i) {
        nodes[i]->outputInfo(str);
    }
    
    str << "\n" << "\n";
}

//To be able to call ville methods outside of an instance
Ville* Ville::getVilleInstance() {
    return &ville;
}

//Ville methods:
bool Ville::createHousing(unsigned int uid, double x, double y, unsigned int size) {
    Housing* newHouse = new Housing(uid, x, y, size);

    if(newHouse->testNodeValidity(ensembleNoeuds) == false) {
        resetVille();
        return false;
    } else {
        ensembleNoeuds.push_back(newHouse);
        cout << "pushback node" << endl;
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
                cout << "pushback node" << endl;

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
                cout << "pushback node" << endl;

        return true;
    }
}

bool Ville::createLien(unsigned int uid1, unsigned int uid2) {
     if(testLinkValidity(uid1, uid2) == false) {
        resetVille();
        return false;
     } else {
                 cout << "pushback link" << endl;

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

//Draw, save, reset methods:
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

    setColor(BLACK);
    for(unsigned int i = 0; i < liens.size(); ++i) {
        drawSegment(liens[i][0]->getCoords(), liens[i][1]->getCoords());
    }
}

void Ville::saveVille(string nomFichier) {
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

    ofstream fichier(nomFichier);

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
            fichier << "\t";
            fichier << liens[i][0]->getUid() << " " << liens[i][1]->getUid() << "\n";
        }
    }

    fichier.close();
}

void Ville::resetVille() {
    if(ensembleNoeuds.empty()) {
        cout << "cannot delete empty city" << endl;
        return;
    }

    if(not liens.empty()){
        for(unsigned int i = 0; i < liens.size(); ++i) {
            liens[i].clear();
        }
        liens.clear();
        cout << "deleted links" << endl;
    }
    cout << "no links to delete" << endl;

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        delete ensembleNoeuds[i];
    }
    ensembleNoeuds.clear();
    cout << "deleted nodes" << endl;
}

//Link info methods:
double Ville::getLinkDistance(unsigned int index) {    
    Vecteur distance;
    creeVecteur(liens[index][0]->getCoords(), liens[index][1]->getCoords(), distance);

    return norme(distance);
} //surcharger cette fonction ? getLinkDistance(int uid1, int uid2);

double Ville::getLinkCapacity(unsigned int index) {
    int size1 = liens[index][0]->getSize();
    int size2 = liens[index][1]->getSize();

    if(size1 >= size2) return size2;
    else return size1;
}

double Ville::getLinkSpeed(unsigned int index) {
    string type1 = liens[index][0]->getType();
    string type2 = liens[index][1]->getType();

    if(type1 == "transport" and type2 == "transport") return fast_speed;
    
    return default_speed;
}

//Ville criterias:
double Ville::critereENJ() {
    if(ensembleNoeuds.empty()) return 0;

    double nbpH(0), nbpT(0), nbpP(0); //population de chaque type de quartier

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(ensembleNoeuds[i]->getType() == "housing") {
            nbpH += ensembleNoeuds[i]->getSize();
        } else if(ensembleNoeuds[i]->getType() == "transport") {
            nbpT += ensembleNoeuds[i]->getSize();
        } else { 
            nbpP += ensembleNoeuds[i]->getSize();
        }
    }

    return (nbpH - (nbpT + nbpP)) / (nbpH + nbpT + nbpP); //return value of ENJ
}

double Ville::critereCI() {
    if(liens.size() == 0) return 0;

    double ci = 0;

    for(unsigned int i = 0; i < liens.size(); ++i) {
        ci += getLinkDistance(i)*getLinkCapacity(i)*getLinkSpeed(i);
    }

    return ci;
}

double Ville::critereMTA() {
    return 0;
}
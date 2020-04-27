/**
* \name ville.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 2.0
*/

//Architecture: Fig 11 b1

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

//Constructor, destructor, getters/setters:
Ville::Ville(): activeNode(noActiveNode) {}
Ville::~Ville() {}

Ville* Ville::getVilleInstance() {
    return &ville;
}

int Ville::getActiveNode() {
    return activeNode;
}

Noeud* Ville::getNode(unsigned int index) {
    return ensembleNoeuds[index];
}

//Reading/Outputting functions:
void lecture(char* nomFichier) {
    ville.resetVille();

    bool continueLecture(true);
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
            if(total == 0) { //no links -> success because all nodes so far are ok
                etat = NBH;
                i = 0;
                total = 0; //reset static stuff back to original
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
                total = 0; //reset static stuff back to original
                if(ville.testMaxLink() == true) ville.resetVille();
                else cout << error::success() << endl;
            }
            break;

        case END: //stops initVille because of an error in the file
            ville.resetVille();
            etat = NBH;
            i = 0;
            total = 0; //reset static stuff back to original
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

//Ville methods:
bool Ville::createHousing(unsigned int uid, double x, double y, unsigned int size) {
    Housing* newHouse = new Housing(uid, x, y, size);

    if(newHouse->testNodeValidity(ensembleNoeuds) == false) {
        return false;
    } else {
        ensembleNoeuds.push_back(newHouse);
        return true;
    }
}

bool Ville::createProduction(unsigned int uid, double x, double y, unsigned int size){
    Production* newProd = new Production(uid, x, y, size);

    if(newProd->testNodeValidity(ensembleNoeuds) == false) {
        return false;    
    } else {
        ensembleNoeuds.push_back(newProd);
        return true;
    }
}

bool Ville::createTransport(unsigned int uid, double x, double y, unsigned int size) {
    Transport* newTrans = new Transport(uid, x, y, size);

    if(newTrans->testNodeValidity(ensembleNoeuds) == false) {
        return false;   
    } else {
        ensembleNoeuds.push_back(newTrans);
        return true;
    }
}

bool Ville::createLien(unsigned int uid1, unsigned int uid2) {
     if(testLinkValidity(uid1, uid2) == false) {
        return false;
     } else {
        unsigned int index1 = findNoeudIndex(uid1);
        unsigned int index2 = findNoeudIndex(uid2);
        Noeud* noeud1 = ensembleNoeuds[index1];
        Noeud* noeud2 = ensembleNoeuds[index2];

        liens.push_back(vector<Noeud*>{noeud1, noeud2});
        noeud1->setLiens(noeud2);
        noeud2->setLiens(noeud1);
        return true;
    }
}

bool Ville::createLien(Coords coords) {
    if(activeNode == noActiveNode) return false;
    unsigned int nodeIndex = findNoeudIndex(coords);

    unsigned int uid1 = ensembleNoeuds[activeNode]->getUid();
    unsigned int uid2 = ensembleNoeuds[nodeIndex]->getUid();

    if(testLinkValidity(uid1, uid2) == false) {
        return false;
     } else {
        Noeud* noeud1 = ensembleNoeuds[activeNode];
        Noeud* noeud2 = ensembleNoeuds[nodeIndex];

        liens.push_back(vector<Noeud*>{noeud1, noeud2});
        noeud1->setLiens(noeud2);
        noeud2->setLiens(noeud1);
        return true;
    }
}

void Ville::deleteNode(Coords coord) {
    if(activeNode == noActiveNode) return;
    
    cout << "ville deleting a node" << endl;

    // if(appartientCercle(ensembleNoeuds[activeNode]->getPosition(), coord)) {
    //     for(unsigned int i = 0; i < liens.size(); ++i) {
    //         if(liens[i][0] == ensembleNoeuds[activeNode]) {
    //             // swap(liens[i], liens.back());
    //             // liens.pop_back();
    //             liens.erase(liens.begin() + i);
    //             --i;
    //         }
    //         if(liens[i][1] == ensembleNoeuds[activeNode]) {
    //             // std::swap(liens[i], liens.back());
    //             // liens.pop_back();
    //             liens.erase(liens.begin() + i);
    //             --i;
    //         }
    //     }

    


    //     swap(ensembleNoeuds[activeNode], ensembleNoeuds.back());
    //     delete ensembleNoeuds.back();
    //     ensembleNoeuds.back() = nullptr;
    //     ensembleNoeuds.pop_back();
    //     activeNode = noActiveNode;
    //     return;
    // }

    // activeNode = noActiveNode;
    return; //stub for rendu 3
}

void Ville::deleteLink() {
    return; //stub for rendu 3
}

unsigned int Ville::findNoeudIndex(unsigned int uid) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(uid == ensembleNoeuds[i]->getUid()) {
            return i;
        }
    }
    cout << "No uid found from findNoeudIndex uid" << endl;
    return 0;
} //given a uid, find the node's index in vector ensembleNoeud

unsigned int Ville::findNoeudIndex(Coords coords) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(appartientCercle(ensembleNoeuds[i]->getPosition(), coords)) {
            return i;
        }
    }
    cout << "No uid found from findNoeudIndex coords" << endl;
    return 0;
}

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
    if(ensembleNoeuds.empty()) return;

    setColor(BLACK);
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        ensembleNoeuds[i]->draw();
    }

    if(activeNode != noActiveNode) {
        setColor(RED);
        ensembleNoeuds[activeNode]->draw();
    }

}

void Ville::drawLinks() {
    if(liens.empty()) return;

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
    if(ensembleNoeuds.empty()) return;

    if(not liens.empty()){
        for(unsigned int i = 0; i < liens.size(); ++i) {
            liens[i].clear();
        }
        liens.clear();
    }

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        delete ensembleNoeuds[i]; //also calls Noeud's destructor
    }
    ensembleNoeuds.clear();

    activeNode = noActiveNode;
}

//Link info methods:
double Ville::linkDistance(unsigned int index) {    
    Vecteur distance;
    creeVecteur(liens[index][0]->getCoords(), liens[index][1]->getCoords(), distance);

    return norme(distance);
}

double Ville::linkCapacity(unsigned int index) {
    int size1 = liens[index][0]->getSize();
    int size2 = liens[index][1]->getSize();

    if(size1 >= size2) return size2;
    else return size1;
}

double Ville::linkSpeed(unsigned int index) {
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
        ci += linkDistance(i)*linkCapacity(i)*linkSpeed(i);
    }

    return ci;
}

double Ville::critereMTA() {
    double mta(0), mtaCount(0), nbH(0);

    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(ensembleNoeuds[i]->getType() == "housing") {
            ++nbH;

            ensembleNoeuds[i]->dijkstra(ensembleNoeuds, "production");
            ensembleNoeuds[i]->dijkstra(ensembleNoeuds, "transport");

            mtaCount += ensembleNoeuds[i]->mtaHP() + ensembleNoeuds[i]->mtaHT();
        }
    }

    if(nbH == 0) return 0; //cannot make an average on 0 nodes

    mta = mtaCount/nbH;

    return mta;
}

//Handling of user events
void Ville::setActiveNode(Coords clickLocation) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(appartientCercle(ensembleNoeuds[i]->getPosition(), clickLocation)) {
            activeNode = i;
            return;
        }
    }

    activeNode = noActiveNode;
    return;
}

void Ville::resetActiveNode() {
    activeNode = noActiveNode;
}


bool Ville::clickOnNode(Coords clickLocation) {
    for(unsigned int i = 0; i < ensembleNoeuds.size(); ++i) {
        if(appartientCercle(ensembleNoeuds[i]->getPosition(), clickLocation)) {
            return true;
        }
    }
    return false;
}

bool Ville::clickOnActiveNode(Coords clickLocation) {
    if(activeNode == noActiveNode) return false;

    if(appartientCercle(ensembleNoeuds[activeNode]->getPosition(), clickLocation)) {
        return true;
    }
    return false;
}

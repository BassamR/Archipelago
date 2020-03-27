/**
* \name ville.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date Mars 2020
* \version 1.0
*/

#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H

#include "noeud.h"

void lecture(char* nomFichier);

class Ville {
private:
    std::vector<Noeud*> ensembleNoeuds;
    std::vector<std::vector<Noeud*>> liens;

public:
    Ville() {}

    void createHousing(unsigned int uid, double x, double y, unsigned int size);
    void createProduction(unsigned int uid, double x, double y, unsigned int size);
    void createTransport(unsigned int uid, double x, double y, unsigned int size);

    void createLien(unsigned int uid1, unsigned int uid2);

    unsigned int findNoeudIndex(unsigned int uid);
    
    bool testIdenticalUid(Noeud* testNoeud);
    bool testNodeNodeSuperposition(Noeud* testNoeud);
    bool testNodeValidity(Noeud* testNoeud);

    bool testLinkVacuum(unsigned int uid1, unsigned int uid2);
    bool testMultipleSameLink(unsigned int uid1, unsigned int uid2);
    bool testSelfLinkNode(unsigned int uid1, unsigned int uid2);
    bool testNodeLinkSuperposition(unsigned int uid1, unsigned int uid2);
    bool testLinkValidity(unsigned int uid1, unsigned int uid2);

    bool testMaxLink();
};

#endif
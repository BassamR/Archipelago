#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H

/**
* \name ville.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date Mars 2020
* \version 1.0
*/

#include <vector>
#include "noeud.h"

void lecture(char* nomFichier);

class Ville {
private:
    std::vector<Noeud*> ensembleNoeuds;
    std::vector<std::vector<Noeud*>> liens;

public:
    Ville() {}

    void createNoeud(unsigned int uid, double x, double y, unsigned int size, 
        std::string type);
    void createLien(unsigned int uid1, unsigned int uid2);

    unsigned int findNoeudIndex(unsigned int uid);

    bool testLinkVacuum(unsigned int uid1, unsigned int uid2);
    bool testMultipleSameLink(unsigned int uid1, unsigned int uid2);
    bool testSelfLinkNode(unsigned int uid1, unsigned int uid2);
    bool testNodeLinkSuperposition(unsigned int uid1, unsigned int uid2);
    //regroups all the above:
    bool testLinkValidity(unsigned int uid1, unsigned int uid2);
    //test after creating all links:
    bool testMaxLink();
};

#endif
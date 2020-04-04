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

    bool createHousing(unsigned int uid, double x, double y, unsigned int size);
    bool createProduction(unsigned int uid, double x, double y, unsigned int size);
    bool createTransport(unsigned int uid, double x, double y, unsigned int size);

    bool createLien(unsigned int uid1, unsigned int uid2);

    unsigned int findNoeudIndex(unsigned int uid);
    
    bool testLinkVacuum(unsigned int uid1, unsigned int uid2);
    bool testMultipleSameLink(unsigned int uid1, unsigned int uid2);
    bool testSelfLinkNode(unsigned int uid1, unsigned int uid2);
    bool testNodeLinkSuperposition(unsigned int uid1, unsigned int uid2);
    bool testLinkValidity(unsigned int uid1, unsigned int uid2);

    bool testMaxLink();

    void drawNodes();
    void drawLinks();
    void saveVille();
    void resetVille();
};

void drawCity();
void deleteCity();
void saveCity();

#endif
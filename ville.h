#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H

//ville.h
//Version 1.0
//Auteurs: Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)

#include <vector>
#include "noeud.h"

void lecture(char* nomFichier);

class Ville {
private:
    std::vector<Noeud> ensembleNoeuds;
    std::vector<unsigned int> uidIndex;
    std::vector<std::vector<unsigned int>> liens;

public:
    void createNoeud(unsigned int uid, double x, double y, unsigned int size, std::string type);
    Noeud getNoeud(unsigned int index); //returns the noeud object at wanted index

    void createLien(unsigned int uid1, unsigned int uid2);

    unsigned int findNoeudIndex(unsigned int neededUid);

    bool testLinkVacuum(unsigned int uid1, unsigned int uid2);
    bool testMultipleSameLink(unsigned int uid1, unsigned int uid2); 
    bool testSelfLinkNode(unsigned int uid1, unsigned int uid2); 
    bool testNodeLinkSuperposition(unsigned int uid1, unsigned int uid2);
    bool testLinkValidity(unsigned int uid1, unsigned int uid2); //regroups all the above
    bool testMaxLink(); //test after creating all links
};

#endif
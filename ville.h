#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H

#include <vector>
#include "noeud.h"

class Ville {
private:
    std::vector<Noeud> ensembleNoeuds;
    std::vector<unsigned int> uidIndex;

public:
    Ville() {}
    
    void createNoeud(unsigned int uid, Cercle coord, unsigned int size, std::string type);
    Noeud getNoeud(unsigned int index); //returns the noeud object at wanted index

    void createLien(unsigned int uid1, unsigned int uid2);

    unsigned int findNoeudIndex(unsigned int neededUid);
};

void lecture(char* nomFichier);

#endif
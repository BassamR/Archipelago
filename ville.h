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

    int activeNode; //=noActiveNode when no node selected

public:
    Ville();
    virtual ~Ville();

    static Ville* getVilleInstance();

    int getActiveNode();
    Noeud* getNode(unsigned int index);

    bool createHousing(unsigned int uid, double x, double y, unsigned int size);
    bool createProduction(unsigned int uid, double x, double y, unsigned int size);
    bool createTransport(unsigned int uid, double x, double y, unsigned int size);

    bool createLien(unsigned int uid1, unsigned int uid2);

    void deleteNode();
    void deleteLink();

    unsigned int findNoeudIndex(unsigned int uid);
    
    bool testLinkVacuum(unsigned int uid1, unsigned int uid2);
    bool testMultipleSameLink(unsigned int uid1, unsigned int uid2);
    bool testSelfLinkNode(unsigned int uid1, unsigned int uid2);
    bool testNodeLinkSuperposition(unsigned int uid1, unsigned int uid2);
    bool testLinkValidity(unsigned int uid1, unsigned int uid2);

    bool testMaxLink();

    void drawNodes();
    void drawLinks();
    void saveVille(std::string nomFichier);
    void resetVille();

    void tempDrawColor();

    double linkDistance(unsigned int index);
    double linkCapacity(unsigned int index);
    double linkSpeed(unsigned int index);

    double critereENJ();
    double critereCI();
    double critereMTA();

    void setActiveNode(Coords clickLocation);
};

#endif
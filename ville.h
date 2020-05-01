/**
* \name ville.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 2.0
*/

//Architecture: Fig 11 b1

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

    bool createHousing(unsigned int uid, double x, double y, unsigned int size,
        double safeDistance = 0);
    bool createProduction(unsigned int uid, double x, double y, unsigned int size, 
        double safeDistance = 0);
    bool createTransport(unsigned int uid, double x, double y, unsigned int size,
        double safeDistance = 0);

    bool createLien(unsigned int uid1, unsigned int uid2, double safeDistance = 0);
    void handleLink(Coords coords, double safeDistance = 0);

    void deleteNode(Coords coord);
    void deleteLink(Noeud* noeud1, Noeud* noeud2);

    unsigned int findNoeudIndex(unsigned int uid);
    unsigned int findNoeudIndex(Coords coords);
    
    bool testLinkVacuum(unsigned int uid1, unsigned int uid2);
    bool testMultipleSameLink(unsigned int uid1, unsigned int uid2);
    bool testSelfLinkNode(unsigned int uid1, unsigned int uid2);
    bool testNodeLinkSuperposition(unsigned int uid1, unsigned int uid2, 
        double safeDistance = 0);
    bool testLinkValidity(unsigned int uid1, unsigned int uid2, 
        double safeDistance = 0);

    bool testMaxLink();

    void drawNodes();
    void drawLinks();
    void saveVille(std::string nomFichier);
    void resetVille();
    void drawShortestPath();

    double linkDistance(unsigned int index);
    double linkCapacity(unsigned int index);
    double linkSpeed(unsigned int index);

    double critereENJ();
    double critereCI();
    double critereMTA();

    void setActiveNode(Coords clickLocation);
    void resetActiveNode();

    bool clickOnNode(Coords clickLocation);
    bool clickOnActiveNode(Coords clickLocation);

    void changeNodeCoords(Coords clickLocation);
    void changeNodeSize(Coords click1, Coords click2);

    int findBiggestUid();
};

#endif
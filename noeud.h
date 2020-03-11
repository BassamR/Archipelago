#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H

#include <vector>
#include "tools.h"

class Noeud {
private:
    unsigned int uid;
    Cercle position; //contient les coords du centre + le rayon (rayon = sqrt(size))
    std::string type;
    std::vector<unsigned int> liens; //each Noeud has a list of nodes its connected to

public:
    Noeud(unsigned int uid, double x, double y, unsigned int size, std::string type);

    unsigned int getUid();
    void setUid(unsigned int x);

    Cercle getPosition();

    Coords getCoords();
    void setCoords(double x, double y);

    unsigned int getSize();
    void setSize(unsigned int x);

    std::string getType();
    void setType(std::string valeur);

    std::vector<unsigned int> getLiens();
    void setLiens(unsigned int linkUid);
};

class errorFunctions { //will actually test for error, unlike error module which just outputs message
private:
public:
    bool identicalUid(std::vector<Noeud> ensembleNoeuds, unsigned int uid); //befpre creating 1 node
    bool linkVacuum(unsigned int uid1, unsigned int uid2, std::vector<Noeud> ensembleNoeuds); //before creating 1 link
    bool maxLink(Noeud obj); //after creating all links
    bool multipleSameLink(unsigned int uid1, unsigned int uid2, std::vector<std::vector<unsigned int>> liens); //before creating 1 link
    bool nodeLinkSuperposition(Noeud obj1, Noeud obj2, Noeud obj3); //pb: when to run this? 
    bool nodeNodeSuperposition(Noeud obj1, Noeud obj2); //pb: when to run this? efficiency
    bool reservedUid(Noeud obj); //before creating 1 node
    bool selfLinkNode(unsigned int uid1, unsigned int uid2); //before creating 1 link
    bool capacityProblem(Noeud obj); //before creating 1 obj
};

#endif
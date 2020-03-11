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
    void identicalUid(unsigned int uid);
    bool linkVacuum(unsigned int uid1, unsigned int uid2, std::vector<Noeud> ensembleNoeuds); //after creating 1 link
    bool maxLink(Noeud obj); //after creating all links
    void multipleSameLink(unsigned int uid1, unsigned int uid2);
    void nodeLinkSuperposition(unsigned int uid);
    void nodeNodeSuperposition(unsigned int uid1, unsigned int uid2);
    bool reservedUid(Noeud obj); //after creating 1 obj
    void success();
    bool selfLinkNode(unsigned int uid1, unsigned int uid2); //after creating 1 link
    bool tooLittleCapacity(Noeud obj); //after creating an obj
    bool tooMuchCapacity(Noeud obj); //after creating an obj
    
    //if(error), the cout is automatic, just exit. else: do nth, continue
};

#endif
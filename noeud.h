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
    void linkVacuum(unsigned int uid);
    void maxLink(unsigned int uid);
    void multipleSameLink(unsigned int uid1, unsigned int uid2);
    void nodeLinkSuperposition(unsigned int uid);
    void nodeNodeSuperposition(unsigned int uid1, unsigned int uid2);
    void reservedUid();
    void success();
    void selfLinkNode(unsigned int uid);
    void tooLittleCapacity(unsigned int capacity);
    void tooMuchCapacity(unsigned int capacity);
    //all void functions: if error -> calls function from error module and exits. else: continue
};

#endif
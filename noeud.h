/**
* \name noeud.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date Mars 2020
* \version 1.0
*/

#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H

#include <vector>
#include "tools.h"
#include <string>

class Noeud {
private:
    unsigned int uid;
    unsigned int size;
    Cercle position; //contient les coords du centre + le rayon (rayon= sqrt(size))
    std::string type;
    std::vector<Noeud*> liens; //node has a list of address of nodes its connected to

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

    std::vector<Noeud*> getLiens();
    void setLiens(Noeud* linkUid);

    bool testReservedUid();
    bool testCapacityProblem();
};

#endif
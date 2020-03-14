#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H

#include <vector>
#include "tools.h"

class Noeud {
private:
    unsigned int uid;
    unsigned int size;
    Cercle position; //contient les coords du centre + le rayon (rayon = sqrt(size))
    std::string type;
    std::vector<unsigned int> liens; //each node has a list of nodes its connected to

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

    //Error Functions for creating Nodes
    bool testIdenticalUid(std::vector<Noeud> ensembleNoeuds); //before creating 1 node
    bool testNodeNodeSuperposition(std::vector<Noeud> ensembleNoeuds); //before creating 1 node
    bool testReservedUid(); //before creating 1 node
    bool testCapacityProblem(); //before creating 1 node
    bool testNodeValidity(std::vector<Noeud> ensembleNoeuds); //regroups all the above
};

#endif
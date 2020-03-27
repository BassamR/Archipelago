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

class Noeud {
protected:
    unsigned int uid;
    unsigned int size;
    Cercle position; //contient les coords du centre + le rayon
    std::vector<Noeud*> liens; //liste des adresses des noeuds voisins

public:
    Noeud(unsigned int uid, double x, double y, unsigned int size);

    unsigned int getUid();
    void setUid(unsigned int x);

    virtual std::string getType();

    Cercle getPosition();

    Coords getCoords();
    void setCoords(double x, double y);

    unsigned int getSize();
    void setSize(unsigned int x);

    std::vector<Noeud*> getLiens();
    void setLiens(Noeud* linkUid);

    bool testReservedUid();
    bool testCapacityProblem();
    virtual bool testMaxLink();

    virtual void draw();
};

class Housing: public Noeud {
public:
    Housing(unsigned int uid, double x, double y, unsigned int size);

    std::string getType();
    void draw();

    bool testMaxLink();
};

class Transport: public Noeud {
public:
    Transport(unsigned int uid, double x, double y, unsigned int size);

    std::string getType();
    void draw();
};

class Production: public Noeud {
public:
    Production(unsigned int uid, double x, double y, unsigned int size);

    std::string getType();
    void draw();
};

#endif
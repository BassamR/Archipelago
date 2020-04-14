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

    bool in;
    unsigned int parent;
    double access;

public:
    Noeud(unsigned int uid, double x, double y, unsigned int size);
    virtual ~Noeud();

    unsigned int getUid();
    void setUid(unsigned int x);

    virtual std::string getType() = 0;

    Cercle getPosition();

    Coords getCoords();
    void setCoords(double x, double y);

    unsigned int getSize();
    void setSize(unsigned int x);

    std::vector<Noeud*> getLiens() const;
    void setLiens(Noeud* linkUid);

    bool getIn();
    void setIn(bool value);

    unsigned int getParent();
    void setParent(unsigned int value);

    double getAccess();
    void setAccess(double value);

    void outputInfo(std::ofstream& str);

    bool testReservedUid();
    bool testCapacityProblem();
    virtual bool testMaxLink();
    bool testIdenticalUid(const std::vector<Noeud*> ensemble);
    bool testNodeNodeSuperposition(const std::vector<Noeud*> ensemble);
    bool testNodeValidity(const std::vector<Noeud*> ensemble);

    virtual void draw() = 0;

    virtual void dijkstra(std::vector<Noeud*>& tn, std::string nodeType); //check conventions if i can keep it majuscule

    virtual void updateShortestPath(std::vector<Noeud*> ensemble, unsigned int goal, std::string type);

    virtual double mtaHP();
    virtual double mtaHT();
};

class Housing: public Noeud {
protected:
    std::vector<Noeud*> shortestPathToProd;
    std::vector<Noeud*> shortestPathToTrans;

public:
    Housing(unsigned int uid, double x, double y, unsigned int size);

    std::string getType();
    void draw();

    bool testMaxLink();

    void dijkstra(std::vector<Noeud*>& tn, std::string nodeType); //check conventions if i can keep it majuscule

    void updateShortestPath(const std::vector<Noeud*>& ensemble, unsigned int goal, std::string type);

    double mtaHP();
    double mtaHT();
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
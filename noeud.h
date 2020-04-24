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
    Noeud* parent;
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

    std::vector<Noeud*> getLiens();
    void setLiens(Noeud* linkUid);

    bool getIn();
    void setIn(bool value);

    Noeud* getParent();
    void setParent(Noeud* value);

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

    virtual void dijkstra(std::vector<Noeud*>& tn, std::string nodeType);

    virtual void updateShortestPathToProd(std::vector<Noeud*>& ensemble, Noeud* goal);
    virtual void updateShortestPathToTrans(std::vector<Noeud*>& ensemble, Noeud* goal);

    virtual double mtaHP();
    virtual double mtaHT();

    virtual std::vector<Noeud*> getShortestProd();
    virtual std::vector<Noeud*> getShortestTrans();
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

    std::vector<Noeud*> getShortestProd();
    std::vector<Noeud*> getShortestTrans();

    void dijkstra(std::vector<Noeud*>& tn, std::string nodeType) override;

    void updateShortestPathToProd(std::vector<Noeud*>& ensemble, Noeud* goal) override;
    void updateShortestPathToTrans(std::vector<Noeud*>& ensemble, Noeud* goal) override;

    double mtaHP() override;
    double mtaHT() override;
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
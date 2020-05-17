/**
* \name noeud.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 3.0
*/

//Architecture: Fig 11 b1

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

    virtual std::string getType() = 0;

    unsigned int getUid();
    void setUid(unsigned int x);

    Cercle getPosition();

    Coords getCoords();
    void setCoords(double x, double y);

    unsigned int getSize();
    void setSize(unsigned int x);

    std::vector<Noeud*> getLiens();
    void setLiens(Noeud* linkUid);
    void removeLien(Noeud* node);

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
    bool testNodeNodeSuperposition(const std::vector<Noeud*> ensemble, 
        double safeDistance = 0);
    bool testNodeValidity(const std::vector<Noeud*> ensemble, 
        double safeDistance = 0);

    virtual void draw() = 0;

    virtual void dijkstra(std::vector<Noeud*>& tn, std::string nodeType);

    virtual void updateShortestPathToProd(Noeud* goal);
    virtual void updateShortestPathToTrans(Noeud* goal);

    virtual double mtaHP();
    virtual double mtaHT();

    virtual std::vector<Noeud*> getShortestProd();
    virtual std::vector<Noeud*> getShortestTrans();

    virtual void drawShortestPath();
};

class Housing: public Noeud {
protected:
    std::vector<Noeud*> shortestPathToProd;
    std::vector<Noeud*> shortestPathToTrans;

public:
    Housing(unsigned int uid, double x, double y, unsigned int size);

    std::string getType();
    void draw();

    bool testMaxLink() override;

    std::vector<Noeud*> getShortestProd() override;
    std::vector<Noeud*> getShortestTrans() override;

    void dijkstra(std::vector<Noeud*>& tn, std::string nodeType) override;

    void updateShortestPathToProd(Noeud* goal) override;
    void updateShortestPathToTrans(Noeud* goal) override;

    double mtaHP() override;
    double mtaHT() override;

    void drawShortestPath() override;
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
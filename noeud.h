#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H

#include <vector>
#include "tools.h"

class Noeud {
private:
    unsigned int uid;
    Cercle coords;
    unsigned int size; //rayon du cercle
    std::string type;
    std::vector<unsigned int> liens; //each Noeud has a list of nodes its connected to

public:
    Noeud(unsigned int uid, Cercle coord, unsigned int size, std::string type, std::vector<unsigned int> liens);

    unsigned int getUid();
    void setUid(unsigned int x);

    Cercle getCoords();
    void setCoords(Cercle cer);

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


void createNoeud(unsigned int uid, Cercle coord, unsigned int size, std::string type);
//Ville va lire une ligne, et appelera cette fonction

//la creation + le controle des liens est fait separement, apres creation/controle des noeuds

unsigned int findNoeudIndex(unsigned int neededUid);
//vu que tout les noeuds sont dans un vector, on doit pouvoir lier leur position et uid
//createNoeud va donc ajouter tt les uids dans un vector<int> Indexes

void createLien(unsigned int uid1, unsigned int uid2);
//adds uid1 to uid2's list of links, and adds uid2 to uid1's list of links

#endif
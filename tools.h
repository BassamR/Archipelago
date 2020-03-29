/**
* \name tools.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date Mars 2020
* \version 1.0
*/

#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

struct Coords {
     double x;
     double y;
};

struct Vecteur {
     double a;
     double b;
};

struct Cercle {
     Coords centre;
     double rayon;
};

struct Segment {
     Coords p;
     Vecteur v;
};

void creeVecteur(Coords p1, Coords p2, Vecteur& v);
double norme(Vecteur v);
bool intersectionCC(Cercle c1, Cercle c2, double safeDistance = 0);
bool intersectionCS(Cercle c, Cercle s, Cercle t, double safeDistance = 0);
//safedistance=0 pour la lecture, =dist_min pour l'ajout d'un noeud manuellement

#endif
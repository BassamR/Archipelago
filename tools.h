/**
* \name tools.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 3.0
*/

//Architecture: Fig 11 b1

#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

#include "graphic_color.h"

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

void creeVecteur(const Coords& p1, const Coords& p2, Vecteur& v);
double norme(const Vecteur& v);
bool intersectionCC(Cercle c1, Cercle c2, double safeDistance = 0);
bool intersectionCS(Cercle c, Cercle s, Cercle t, double safeDistance = 0);
//safedistance=0 pour la lecture, =dist_min pour l'ajout d'un noeud manuellement

bool appartientCercle(Cercle cercle, Coords coords);

void setColor(Color couleur);

void drawCircle(Cercle c);
void drawSegment(Coords v1, Coords v2);
void drawRectangle(Coords point, double width, double height);

#endif
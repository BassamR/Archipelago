#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

#include "constantes.h"

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

void creeVecteur(Coords a, Coords b, Vecteur v);
double norme(Vecteur v);
bool intersectionCC(Cercle p, Cercle l);
bool intersectionCS(Cercle c, Segment s);

#endif
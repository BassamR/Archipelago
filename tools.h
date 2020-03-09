#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

#include "constantes.h"

double distancePoints();
double distancePointVecteur();
double norme();

struct Cercle {
    unsigned int x;
    unsigned int y;
};
//above is temporary, just to be able to use the type Cercle in other modules

struct coordo {
     double x;
     double y;
};

struct vecteur {
     double a;
     double b;
};

struct cercle {
     coordo centre;
     double rayon;
};

struct segment {
     coordo p;
     vecteur v;
};

void cvecteur(coordo a, coordo b, vecteur v);
double norme(vecteur v);
bool intersectioncc(cercle p, cercle l);
bool intersectioncs(cercle c, segment s);

#endif
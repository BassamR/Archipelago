#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

double distancePoints();
double distancePointVecteur();
double norme();

struct Cercle {
    unsigned int x;
    unsigned int y;
}; //temporary, just to be able to use the type Cercle in other modules

#endif
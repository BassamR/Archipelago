#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

double distancePoints();
double distancePointVecteur();
double norme();

struct Cercle {
    int x;
    int y;
    int r;
}; //temporary, just to be able to use the type Cercle in other modules

#endif
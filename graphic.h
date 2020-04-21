//interface partielle que va utiliser tools pour le dessin

#ifndef GRAPHIC_HEADER_H
#define GRAPHIC_HEADER_H

#include "graphic_color.h"

//uniquement les fonctions de dessin
void makeBgWhite();

void setColorG(Color couleur);

void drawCircleG(double x, double y, double r);
void drawSegmentG(double x1, double y1, double x2, double y2);

#endif
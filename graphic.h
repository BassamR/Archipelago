/**
* \name graphic.h
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 1.0
*/

#ifndef GRAPHIC_HEADER_H
#define GRAPHIC_HEADER_H

#include "graphic_color.h"

void makeBgWhite();

void setColorGraphic(Color couleur);

void drawCircleGraphic(double x, double y, double r);
void drawSegmentGraphic(double x1, double y1, double x2, double y2);
void drawRectangleGraphic(double x, double y, double width, double height);

#endif
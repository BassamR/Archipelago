/**
* \name graphic.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 1.0
*/

//Architecture: Fig 11 b1

#include <cmath>
#include "graphic_gui.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);
static const double lineWidth = 6.0;

void graphicSetContext(const Cairo::RefPtr<Cairo::Context>& cr) {
    static bool init(false);
    if(!init) {
        ptcr = &cr;
        init = true;
    }
}

void makeBgWhite() {
    (*ptcr)->set_source_rgb(1, 1, 1);
    (*ptcr)->paint();
}

//Draw functions definition
void setColorGraphic(Color couleur) {
    switch(couleur) {
        case BLACK:
            (*ptcr)->set_source_rgb(0.0, 0.0, 0.0);
            break;
        case RED:
            (*ptcr)->set_source_rgb(1.0, 0.0, 0.0);
            break;
        case GREEN:
            (*ptcr)->set_source_rgb(0.0, 1.0, 0.0);
            break;
        default: break;
    }
}

void drawCircleGraphic(double x, double y, double r) {
    (*ptcr)->set_line_width(lineWidth);

    (*ptcr)->save();
    (*ptcr)->arc(x, y, r, 0.0, 2 * M_PI);
    (*ptcr)->set_source_rgb(1.0, 1.0, 1.0);
    (*ptcr)->fill_preserve();

    (*ptcr)->restore();
    (*ptcr)->stroke();
}

void drawSegmentGraphic(double x1, double y1, double x2, double y2) {
    (*ptcr)->set_line_width(lineWidth);

    (*ptcr)->save();
    (*ptcr)->move_to(x1, y1);
    (*ptcr)->line_to(x2, y2);

    (*ptcr)->restore();
    (*ptcr)->stroke();
}

void drawRectangleGraphic(double x, double y, double width, double height) {
    (*ptcr)->set_line_width(lineWidth);

    (*ptcr)->save();
    (*ptcr)->rectangle(x, y, width, height);

    (*ptcr)->restore();
    (*ptcr)->stroke();
}
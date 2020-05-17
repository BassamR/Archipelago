/**
* \name graphic.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 2.0
*/

//Architecture: Fig 11 b1

#include <cmath>
#include "graphic_gui.h"

#define WHITE_RGB 1.0, 1.0, 1.0
#define BLACK_RGB 0.0, 0.0, 0.0
#define RED_RGB 1.0, 0.0, 0.0
#define GREEN_RGB 0.0, 1.0, 0.0
#define DEFAULT_LINE_WIDTH 6.5

static constexpr double zoomIncrement(1.2);
static double lineWidth(DEFAULT_LINE_WIDTH);
static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphicSetContext(const Cairo::RefPtr<Cairo::Context>& cr) {
    static bool init(false);
    if(!init) {
        ptcr = &cr;
        init = true;
    }
}

void incrementLineWidth() {
    lineWidth *= zoomIncrement;
}

void decrementLineWidth() {
    lineWidth /= zoomIncrement;
}

void resetLineWidth() {
    lineWidth = DEFAULT_LINE_WIDTH;
}

void makeBgWhite() {
    (*ptcr)->set_source_rgb(WHITE_RGB);
    (*ptcr)->paint();
}

//Draw functions definition
void setColorGraphic(Color couleur) {
    switch(couleur) {
        case BLACK:
            (*ptcr)->set_source_rgb(BLACK_RGB);
            break;
        case RED:
            (*ptcr)->set_source_rgb(RED_RGB);
            break;
        case GREEN:
            (*ptcr)->set_source_rgb(GREEN_RGB);
            break;
        default: break;
    }
}

void drawCircleGraphic(double x, double y, double r) {
    (*ptcr)->set_line_width(lineWidth);

    (*ptcr)->save();
    (*ptcr)->arc(x, y, r, 0.0, 2 * M_PI);
    (*ptcr)->set_source_rgb(WHITE_RGB);
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
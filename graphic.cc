#include <iostream>
#include <cmath>
#include "graphic_gui.h"
#include "constantes.h"
using namespace std;

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);
static double changeCoordsX(double x); //not used elsewhere
static double changeCoordsY(double y);
static double changeCoordsR(double r);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr) {
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

static double changeCoordsX(double x) {
    double newX = default_drawing_size * (x + dim_max) / (dim_max + dim_max);
    return newX;
}

static double changeCoordsY(double y) {
    double newY = default_drawing_size * (dim_max - y) / (dim_max + dim_max);
    return newY;
}

static double changeCoordsR(double r) {
    double newR = r * default_drawing_size / (dim_max + dim_max);
    return newR;
}

void drawCircleG(double a, double b, double c) {
    double x = changeCoordsX(a);
    double y = changeCoordsY(b);
    double r = changeCoordsR(c);

    (*ptcr)->set_line_width(2.0);
    (*ptcr)->set_source_rgb(0.0, 0.0, 0.0);

    (*ptcr)->save();
    (*ptcr)->arc(x, y, r, 0.0, 2 * M_PI);
    (*ptcr)->set_source_rgb(1.0, 1.0, 1.0);
    (*ptcr)->fill_preserve();

    (*ptcr)->restore();
    (*ptcr)->stroke();
}

void drawSegmentG(double a1, double b1, double a2, double b2) {
    double x1 = changeCoordsX(a1);
    double y1 = changeCoordsY(b1);
    double x2 = changeCoordsX(a2);
    double y2 = changeCoordsY(b2);

    (*ptcr)->set_line_width(2.0);
    (*ptcr)->set_source_rgb(0.0 , 0.0, 0.0);
    (*ptcr)->move_to(x1, y1);
    (*ptcr)->line_to(x2, y2);
    (*ptcr)->stroke();
}
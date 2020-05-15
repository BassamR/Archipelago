/**
* \name tools.cc
* \author Hugo Masson, Bassam El Rawas (Sciper 314886, 310635)
* \date May 2020
* \version 2.0
*/

//Architecture: Fig 11 b1

#include <iostream>
#include <cmath>
#include "tools.h"
#include "graphic.h"
using namespace std;

void creeVecteur(const Coords& p1, const Coords& p2, Vecteur& v) {
     v.a = p2.x - p1.x;
     v.b = p2.y - p1.y;
}

double norme(const Vecteur& v) {
    return sqrt(v.a * v.a + v.b * v.b);
}

bool intersectionCC(Cercle c1, Cercle c2, double safeDistance) { //CC = cercle-cercle
    Vecteur c1c2;
    creeVecteur(c1.centre, c2.centre, c1c2);
    
    if (c1.rayon + c2.rayon + safeDistance >= norme(c1c2)) {
        return true;
    } else {
        return false;
    }
}

//intersection entre lien c-s et le noeud t. CS = cercle-segment
bool intersectionCS(Cercle c, Cercle s, Cercle t, double safeDistance) {
    if(t.centre.x == c.centre.x and t.centre.y == c.centre.y) {
        return false;
    }
    if(t.centre.x == s.centre.x and t.centre.y == s.centre.y) {
        return false;
    } //pas le droit de tester avec un 3e noeud egal aux 2 premiers
    
    //creation des vecteurs c-s, c-t...
    Vecteur cs, ct, st, sc;
    creeVecteur(c.centre, s.centre, cs);
    creeVecteur(c.centre, t.centre, ct);
    creeVecteur(s.centre, t.centre, st);
    creeVecteur(s.centre, c.centre, sc);

    //produit scalaire: ct.cs = |ct||cs|cos(ct;cs)
    double scalar1 = (ct.a * cs.a + ct.b * cs.b)/(norme(cs)*norme(ct));
    double scalar2 = (st.a * sc.a + st.b * sc.b)/(norme(st)*norme(sc));

    //arccos defined for x in [-1, 1], deal with precision issues for double
    if(scalar1 < -1) scalar1 = -1.;
    if(scalar1 > 1) scalar1 = 1.;
    if(scalar2 < -1) scalar2 = -1.;
    if(scalar2 > 1) scalar2 = 1.;

    double angleCTCS = acos(scalar1);
    double angleSTSC = acos(scalar2);

    if(angleCTCS >= M_PI/2.0 or angleCTCS <= -M_PI/2.0 or angleSTSC >= M_PI/2.0 or 
        angleSTSC <= -M_PI/2.0) {
        return false;
    }

    //distance entre le lien c-s et le centre du noeud t
    double dist = norme(ct)*sin(angleCTCS);
    
    if(dist <= t.rayon + safeDistance) {
        return true;
    } else {
        return false;
    }
}

bool appartientCercle(Cercle cercle, Coords coords) {
    double xc = cercle.centre.x;
    double yc = cercle.centre.y;
    double r = cercle.rayon;
    double x = coords.x;
    double y = coords.y;

    if(pow(x-xc, 2) + pow(y-yc, 2) <= pow(r, 2)) return true;
    else return false;
}

//Draw functinons
void setColor(Color couleur) {
    switch(couleur) {
        case BLACK:
            setColorGraphic(BLACK);
            break;
        case RED:
            setColorGraphic(RED);
            break;
        case GREEN:
            setColorGraphic(GREEN);
            break;
        default: break;
    }
}

void drawCircle(Cercle c) {
    drawCircleGraphic(c.centre.x, c.centre.y, c.rayon);
}

void drawSegment(Coords v1, Coords v2) {
    drawSegmentGraphic(v1.x, v1.y, v2.x, v2.y);
}

void drawRectangle(Coords point, double width, double height) {
    drawRectangleGraphic(point.x, point.y, width, height);
}
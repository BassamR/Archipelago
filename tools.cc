#include <iostream>
#include <cmath>
#include "tools.h"
#include "constantes.h"
using namespace std;

const double PI = 3.14159265358979323846;

void creeVecteur(Coords p1, Coords p2, Vecteur& v) {
     v.a = p2.x - p1.x;
     v.b = p2.y - p1.y;
}

double norme(Vecteur v) {
    return sqrt(v.a * v.a + v.b * v.b);
}

bool intersectionCC(Cercle c1, Cercle c2) { //cc = cercle-cercle
    Vecteur c1c2;
    creeVecteur(c1.centre, c2.centre, c1c2);
    
    if (c1.rayon + c2.rayon + dist_min >= norme(c1c2)) {
        return true;
    } else {
        return false;
    }
}

//intersection entre lien c-s, et noeud t
bool intersectionCS(Cercle c, Cercle s, Cercle t) { //cs = cercle-segment
    Vecteur cs; //creation du vecteur allant du centre de c au centre de s
    creeVecteur(c.centre, s.centre, cs);
    Vecteur ct;
    creeVecteur(c.centre, t.centre, ct);

    Vecteur st, sc;
    creeVecteur(s.centre, t.centre, st);
    creeVecteur(s.centre, t.centre, sc);

    //ct.cs = |ct||cs|cos(ct;cs), produit scalaire
    double angleCTCS = acos((ct.a * cs.a + ct.b * cs.b)/(norme(cs)*norme(ct))); 
    double angleSTSC = acos((st.a * sc.a + st.b * sc.b)/(norme(st)*norme(sc)));

    if(angleCTCS >= PI/2 or angleCTCS <= -PI/2 or angleSTSC >= PI/2 or angleSTSC <= -PI/2) {
        return false;
    } //impossible to have link-node superposition in this case

    double dist = norme(ct)*sin(angleCTCS); //distance entre le lien c-s et le noeud t
    if(dist <= dist_min) { //la distance prend en compte le rayon de t
        return true;
    } else {
        return false;
    }
}
#include <iostream>
#include <cmath>
#include "tools.h"
#include "constantes.h"
using namespace std;

void creeVecteur(Coords p1, Coords p2, Vecteur v) {
     v.a = p2.x - p1.x;
     v.b = p2.y - p1.y;
}

double norme(Vecteur v) {
    return sqrt(v.a*v.a + v.b*v.b);
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

bool intersectionCS(Cercle c, Cercle s, Cercle t) { //cs = cercle-segment
    Vecteur cs; //creation du vecteur allant du centre de c au centre de s
    creeVecteur(c.centre, s.centre, cs);
    Vecteur ct;
    creeVecteur(c.centre, s.centre, ct);

    //ct.cs = |ct||cs|cos(ct;cs), produit scalaire
    double angleCTCS = acos((ct.a*cs.a + ct.b*cs.b)/(norme(cs)*norme(ct))); 
    double dist = norme(ct)*sin(angleCTCS); //distance entre le lien c-s et le noeud t

    if(dist <= dist_min) { //la distance prend en compte le rayon de t
        return true;
    } else {
        return false;
    }
}
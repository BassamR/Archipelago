//outils mathematiques, independants de la notion de noeud, lien, ville...
#include <iostream>
#include <cmath>
#include "tools.h"
#include "constantes.h"
using namespace std;

void cvecteur(coordo a, coordo b, vecteur v) {
     v.a = b.x-a.x;
     v.b = b.y-a.y;
}

double norme(vecteur v) {
    return sqrt(v.a*v.a+v.b*v.b);
}

bool intersectioncc(cercle p, cercle l) {
    vecteur cc;
    cvecteur(p.centre, l.centre, cc);
    double dist = norme(cc);
    if (p.rayon+l.rayon+dist_min >= dist) {
        return true;
    } else {
        return false;
    }
}

bool intersectioncs(cercle c, cercle s, cercle t) {
    vecteur cs; //creation du vecteur allant du centre de c au centre de s
    cvecteur(c.centre, s.centre, cs);

    vecteur ct;
    cvecteur(c.centre, s.centre, ct);

    vecteur cthort; //vecteur orthogonal au segment cs
    double produit = (ct.a*cs.a+ct.b*cs.b)/(cs.a*cs.a+cs.b*cs.b); //produit scalaire/norme
    cthort.a = ct.a-produit*cs.a; // orthogonalisation
    cthort.b = ct.b-produit*cs.b; //ortho
    double dist = norme(cthort); //distance minimale entre le centre de t et le segment cs

    if(dist <= t.rayon+dist_min) {
        return true;
    } else {
        return false;
    }
}
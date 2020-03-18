#include <iostream>
#include <cmath>
#include "tools.h"
#include "constantes.h"
using namespace std;

const double pi = 3.14159265358979323846;

void creeVecteur(Coords p1, Coords p2, Vecteur& v) {
     v.a = p2.x - p1.x;
     v.b = p2.y - p1.y;
}

double norme(Vecteur v) {
    return sqrt(v.a * v.a + v.b * v.b);
}

bool intersectionCC(Cercle c1, Cercle c2, double safeDistance) { //cc = cercle-cercle
    Vecteur c1c2;
    creeVecteur(c1.centre, c2.centre, c1c2);
    
    if (c1.rayon + c2.rayon + safeDistance >= norme(c1c2)) {
        return true;
    } else {
        return false;
    }
}

//intersection entre lien c-s et noeud t
bool intersectionCS(Cercle c, Cercle s, Cercle t, double safeDistance) { //cs = cercle-segment
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

    //ct.cs = |ct||cs|cos(ct;cs), produit scalaire
    double angleCTCS = acos((ct.a * cs.a + ct.b * cs.b)/(norme(cs)*norme(ct))); 
    double angleSTSC = acos((st.a * sc.a + st.b * sc.b)/(norme(st)*norme(sc)));

    if(angleCTCS >= pi/2 or angleCTCS <= -pi/2 or angleSTSC >= pi/2 or angleSTSC <= -pi/2) {
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
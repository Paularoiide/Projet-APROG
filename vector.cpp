
#include "vector.h"
#include <cmath>


Vector operator+(Vector a, Vector b){
    Vector result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;

}
Vector operator-(Vector a, Vector b){
    Vector result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}
double norm2(Vector a){
    return a.x*a.x + a.y*a.y;

}
Vector operator*(Vector a, double lambda){
    Vector result;
    result.x = a.x * lambda;
    result.y = a.y * lambda;
    return result;
}
Vector operator*(double lambda, Vector a){
    return a * lambda;
}

Vector operator/(Vector a, double lambda){
    Vector result;
    result.x = a.x / lambda;
    result.y = a.y / lambda;
    return result;
}

// produit scalaire
double ps(Vector a, Vector b) {
    return a.x*b.x + a.y*b.y;
}

Vector rotate(Vector a, double angle) {
    angle *= M_PI / 180;
    Vector c;
    double co = cos(angle);
    double si = sin(angle);
    c.x = co * a.x + si * a.y;
    c.x = - si * a.x + co * a.y;
    return c;
}

// renvoie le projete d'un point a à la droite formee en reliant
// les points PointA et PointB
Vector projection(Vector a, Vector PointA, Vector PointB) {
    double pente=90.;
    if (PointA.x=PointB.x) {
        if (PointA.y>PointB.y) {
            pente=270;
        } else { pente=90;}
    } else {
        pente = (PointB.y-PointA.y)/(PointB.x-PointA.x);
    }
    double ordALOrigine = PointB.y-pente*PointB.x;
    // on calcule le projete de l'origine
    Vector POrigine;
    POrigine.x = -pente*ordALOrigine/(2*(1+pente*pente));
    POrigine.y = pente*POrigine.x+ordALOrigine;

    // le projete P s'ecrit : P = a . e/||e|| + POrigine, où e est un vecteur directeur de la droite
    Vector e;
    e = PointB-PointA;
    return(ps(a,e)/norm2(e)*e + POrigine);



}
// idem au projete pour la distance
double distance(Vector a, Vector PointA, Vector PointB) {
    // la projection minimisant la distance étant orthogonale en dimension finie
    return norm2(a-projection(a,PointA,PointB));
}
double distance(Vector a, Vector b) {
    return norm2(a-b);
}

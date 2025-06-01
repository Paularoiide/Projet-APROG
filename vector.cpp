
#include "vector.h"
#include <cmath>


Vector operator+(Vector a, Vector b){ // Opération +
    Vector result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;

}
Vector operator-(Vector a, Vector b){ // Opération -
    Vector result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}
double norm2(Vector a){ // Norme au carré d'un vecteur
    return a.x*a.x + a.y*a.y;
}
Vector operator*(Vector a, double lambda){ // Opération *
    Vector result;
    result.x = a.x * lambda;
    result.y = a.y * lambda;
    return result;
}
Vector operator*(double lambda, Vector a){  // Opération * dans l'autre sens
    return a * lambda;
}

Vector operator/(Vector a, double lambda){ // Opération /
    Vector result;
    result.x = a.x / lambda;
    result.y = a.y / lambda;
    return result;
}

Vector operator/(double lambda, Vector a){  // Opération / dans l'autre sens
    return a / lambda;
}

double ps(Vector a, Vector b) { // produit scalaire entre 2 vecteurs
    return a.x*b.x + a.y*b.y;
}

Vector rotate(Vector a, double angle) { // Tourne un vecteur d'un certain degré
    angle *= M_PI / 180;
    Vector c;
    double co = cos(angle);
    double si = sin(angle);
    c.x = co * a.x + si * a.y;
    c.x = - si * a.x + co * a.y;
    return c;
}



Vector projection(Vector a, Vector PointA, Vector PointB) {// Renvoie le projeté d'un point a à la droite formee en reliant les points PointA et PointB
    Vector e = PointB - PointA;       // vecteur directeur de la droite
    Vector ap = a - PointA;           // vecteur de PointA à a
    double t = ps(ap, e) / norm2(e);  // projection scalaire
    return PointA + t * e;            // projection vectorielle sur la droite
}



double distance(Vector a, Vector PointA, Vector PointB) {// Renvoie le projeté d'un point a à la droite formee en reliant les points PointA et PointB
    return norm2(a-projection(a,PointA,PointB));
}
double distance(Vector a, Vector b) {//Renvoie la distance entre deux points
    return sqrt(norm2(a-b));
}

double angle_entre(Vector a, Vector b){ // Angle entre a et b dans le sens horaire en degres
    return -asin(a.x*b.y-a.y*b.x)/(norm2(a)*norm2(b))*180/M_PI;
}


double rad(double deg) { // Converti de degré en radiant
    return(M_PI/180.*deg);
}
double deg(double rad) { // Converti de radiant en degré
    return(180.*rad/M_PI);
}

Vector reflect(Vector v, Vector normal) { // Donne la reflection d'un vecteur par rapport à une normale
    return v - 2 * ps(v, normal) * normal;
}

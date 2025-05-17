
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

// prend l'angle en degres
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

double angle_entre(Vector a, Vector b){ // Angle entre a et b dans le sens horaire en degres
    return -asin(a.x*b.y-a.y*b.x)/(norm2(a)*norm2(b))*180/M_PI;
}
double angle_horiz(Vector a) {
    Vector horiz = {1.,0.};
    return angle_entre(horiz,a);
}

double rad(double deg) {
    return(M_PI/180.*deg);
}
double deg(double rad) {
    return(180.*rad/M_PI);
}

// renvoie l'intersection d'une demi-droite avec un des bords
// le point est supposé dans l'écran
// angle : en degres, mesure principale [0;360[

Vector intersection_bords(Vector point, double angle) {
    if(angle==0.) {
    Vector v={static_cast<double>(WIDTH),point.y};
    return(v);
    } else if(angle==90.) {
    Vector v={point.x,0.};
    return(v);
    } else if(angle==180.) {
    Vector v={0.,point.y};
    return(v);
    } else if(angle==270.) {
    Vector v={point.x,static_cast<double>(HEIGHT)};
    return(v);}

    double dmin;//distance minimale
    // y = (x-point.x)*tan(angle)+point.y
    double a = rad(angle);
    Vector cibleDroite = {static_cast<double>(WIDTH),(static_cast<double>(WIDTH)-point.x)*tan(a)+point.y};
    Vector cibleGauche = {0.,-1.*point.x*tan(a)+point.y};
    Vector cibleHaute = {point.x-point.y/tan(a),0.};
    Vector cibleBasse = {point.x+(static_cast<double>(HEIGHT)-point.y)/tan(a),static_cast<double>(HEIGHT)};
    Vector candidats[2];
    if(0.<angle && angle < 90.) {
        candidats[0] = cibleHaute;
        candidats[1] = cibleDroite;
    } else if(90.<angle && angle<180.) {
        candidats[0] = cibleHaute;
        candidats[1] = cibleGauche;
    } else if(180.<angle && angle<270.) {
        candidats[0] = cibleBasse;
        candidats[1] = cibleGauche;
    } else {
        candidats[0] = cibleBasse;
        candidats[1] = cibleDroite;
    }
    if(distance(point,candidats[0])<distance(point,candidats[1])){return(candidats[0]);}
    else{return(candidats[1]);}
}

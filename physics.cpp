#include "physics.h"


Vector Acceleration(Vector v, double friction_coeff, double friction_coeff2){ // Calcul l'acceleration d'un corps à vitesse v en prenant en compte des frottements
    Vector acc = {0.,0.};

    // ajout de la friction
    Vector friction = -friction_coeff * v;
    acc =acc + friction;

    // ajout de la friction constante
    Vector friction2 = -friction_coeff2*v/sqrt(norm2(v));
    acc = acc + friction2;

    return acc;
}

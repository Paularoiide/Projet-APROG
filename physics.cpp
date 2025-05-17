#include "physics.h"


Vector Acceleration(Vector v, double friction_coeff){
    Vector acc = {0.,0.};

    // ajout de la friction
    Vector friction = -friction_coeff * v;
    acc =acc + friction;

    return acc;
}

#include "physics.h"


Vector Acceleration(Vector v, double friction_coeff){
    Vector acc = {0.,0.};
    Vector friction = -friction_coeff * v;
    acc =acc + friction;
    return acc;
}


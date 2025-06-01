#pragma once
#include "vector.h"

#include <Imagine/Graphics.h>

using namespace Imagine;

Vector Acceleration(Vector v, double friction_coeff = 0.003, double friction_coeff2 = 0.003); // Calcul l'acceleration d'un corps à vitesse v en prenant en compte des frottements


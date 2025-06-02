#pragma once
#include "vector.h"

#include <Imagine/Graphics.h>

using namespace Imagine;

// Calcul une accélération en prenant en compte différents types de frottements
Vector Acceleration(Vector v,bool frein, double friction_coeff = 0.003, double friction_coeff2 = 0.003);

// Detecte les inputs au clavier
int keyboard();

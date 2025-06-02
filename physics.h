#pragma once
#include "vector.h"

#include <Imagine/Graphics.h>

using namespace Imagine;

Vector Acceleration(Vector v,bool frein, double friction_coeff = 0.003, double friction_coeff2 = 0.003);

int keyboard();

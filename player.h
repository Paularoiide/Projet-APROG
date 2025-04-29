#pragma once
#include <Imagine/Graphics.h>
#include "physics.h"
#include "vector.h"
#include "affichage.h"
#include "niveaux.h"

using namespace Imagine;

#include <typeinfo>
using namespace std;

enum class role_Slime {
    JOUEUR, SLIME_ENEMY, PNJ
};

class Slime{

public:
    Vector pos;
    Vector speed = Vector{0.,0.};
    Idx sprite = {0,0};
    void Lancer();
    void Move();
    void Accelerate(Vector a);
    void Display(); // Affiche le sprite en cours
    void Collision(Element Obstacle);
    Vector Launch();
    Slime( role_Slime givenRole, Vector givenPosition);
};


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
    int radius = 8;
    void Lancer();
    void Move();
    void Accelerate(Vector a);
    void Display(); // Affiche le sprite en cours
    void Shock(Collisionable *Obstacle);
    bool Collision(Collisionable *Obstacle);
    void Die();
    Vector Launch();
    Vector Launch2();
    Slime( role_Slime givenRole, Vector givenPosition);
};


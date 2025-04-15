#pragma once
#include <Imagine/Graphics.h>
#include "physics.h"
#include "vector.h"
#include "affichage.h"

using namespace Imagine;

enum class role_Slime {
    JOUEUR, SLIME_ENEMY, PNJ
};

class Slime{

public:
    Color color;
    Vector pos;
    Vector speed = Vector{0.,0.};
    int sprite_pos = 0;
    int rayon = 10;
    void Lancer(Vector cible);
    void Move();
    void Accelerate(Vector a);
    void Afficher(); // Affiche le sprite en cours
    Slime(Color givenColor, role_Slime givenRole, Vector givenPosition);
};


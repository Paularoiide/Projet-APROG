#pragma once

#include "physics.h"
#include "vector.h"
#include "affichage.h"
#include "element.h"

#include <Imagine/Graphics.h>
using namespace Imagine;

#include <typeinfo>
using namespace std;

enum class role_Slime {
    JOUEUR, SLIME_ENEMY,KILLER, PNJ
};
role_Slime roleFromStr(string s);

class Slime : public Element{

public:
    Vector pos;
    role_Slime role;
    Vector speed = Vector{0.,0.};
    Idx sprite = {0,0};
    int radius = 4;
    //void Lancer(vector<unique_ptr<Element>>& obstacles);
    void Lancer(Vector pulse, vector<unique_ptr<Element>>& obstacles,Background background);
    void Move();
    void Accelerate(Vector a);
    void Display(); // Affiche le sprite en cours
    void Shock(Collisionable *Obstacle);
    bool Collision(Collisionable *Obstacle);
    void Check(Slime slime, vector<unique_ptr<Element>>& obstacles);
    void KILL(Slime slime, vector<unique_ptr<Element>>& obstacles);
    void Die();
    void Win();
    Vector Launch();
    Vector Launch2();
    Slime( role_Slime givenRole, Vector givenPosition);
    void afficher() override {
        Display();
    }
};


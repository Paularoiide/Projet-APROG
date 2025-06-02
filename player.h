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
    Vector pos_initial;
    role_Slime role;
    Vector speed = Vector{0.,0.};
    Idx sprite = {0,0};
    int patternNumber;
    Vector *pattern;
    int radius = 4;
    int frein=0;
    //void Lancer(vector<unique_ptr<Element>>& obstacles);
    void Lancer(Vector pulse, vector<unique_ptr<Element>>& obstacles,Background background);
    void Move();
    void Accelerate(Vector a);
    void Display(); // Affiche le sprite en cours
    void Shock(Collisionable *Obstacle);
    bool Collision(Collisionable *Obstacle);
    bool CollisionSlime(const Slime& other);
    void Check(Slime& slime, vector<unique_ptr<Element>>& obstacles);
    void KILL(Slime slime, vector<unique_ptr<Element>>& obstacles);
    void Die();
    void Win();
    void idleMove();
    Vector Launch();
    Vector Launch2();
    Slime( role_Slime givenRole, Vector givenPosition);
    Slime(role_Slime givenRole, Vector givenPos, Vector *movePattern, int n);
    void afficher() override {
        Display();
    }
};


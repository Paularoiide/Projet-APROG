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
    Vector speed;
    Idx sprite;
    int radius;
    //void Lancer(vector<unique_ptr<Element>>& obstacles);
    void Lancer(Vector pulse, vector<unique_ptr<Element>>& obstacles,Background background);
    void Move();
    void Accelerate(Vector a);
    void Display(); // Affiche le sprite en cours
    void Shock(Collisionable *Obstacle);
    bool Collision(Collisionable *Obstacle);
    bool CollisionSlime(const Slime& other);
    void Check(Slime& slime, vector<unique_ptr<Element>>& obstacles);
    void idleMove();
    Vector Launch();
    Slime( role_Slime givenRole, Vector givenPosition);
    void afficher() override {
        Display();
    }
};


#pragma once

#include "physics.h"
#include "vector.h"
#include "affichage.h"
#include "element.h"

#include <Imagine/Graphics.h>
using namespace Imagine;

#include <typeinfo>
using namespace std;


class Joueur;


class Slime : public Element {
protected:
    Idx sprite = {0, 0};
    int patternNumber;
    Vector* pattern;

public:
    Vector pos;
    Vector pos_initial;
    Vector speed = Vector{0., 0.};
    int radius = 4;

    // Méthodes générales
    void Accelerate(Vector a);
    void Shock(Collisionable* Obstacle);
    bool Collision(Collisionable* Obstacle) const;

    // Méthodes abstraites à redéfinir
    virtual void Display() = 0;             // méthode virtuelle pure
    virtual void Move() = 0;
    virtual Vector Launch() const {

    }
    virtual void Check(Joueur& joueur, vector<unique_ptr<Element>>& obstacles) {

    }
    virtual bool CollisionSlime(const Slime& other){

    }

    // Surcharge d'affichage pour Element
    void afficher() override {
        Display();
    }

    virtual ~Slime() = default; // Destructeur virtuel
};


class Joueur : public Slime {
public:
    Joueur(Vector givenPosition);
    void Display() override;
    void Move() override;
    Vector Launch() const;
};



class Ennemi : public Slime {
public:
    bool kill;
    Ennemi(Vector givenPosition);
    Ennemi(Vector givenPos, Vector *movePattern, int n);
    void Display() override;
    void Move() override;
    void Check(Joueur& joueur, vector<unique_ptr<Element>>& obstacles) override;
    bool CollisionSlime(const Joueur &other);
};


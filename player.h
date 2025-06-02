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
class Ennemi;

// Classe de base abstraite représentant un Slime (joueur ou ennemi)
class Slime : public Element {
protected:
    Idx sprite = {0, 0};       // Indices pour l'animation du sprite
    Vector pos;                // Position actuelle
    Vector pos_initial;        // Position initiale
    Vector speed = Vector{0., 0.};  // Vitesse actuelle
    int radius = 4;            // Rayon pour les collisions
public:
    // Méthodes générales
    void Accelerate(Vector a); // Applique une accélération au Slime
    void Shock(Collisionable* Obstacle); // Gère le rebond sur un obstacle
    bool Collision(Collisionable* Obstacle) const; // Détecte les collisions avec un obstacle

    // Méthodes abstraites à redéfinir
    virtual void Display() = 0; // Affiche le Slime (méthode virtuelle pure)
    virtual void Move() = 0;    // Gère le déplacement (méthode virtuelle pure)

    // Méthodes optionnelles (implémentation par défaut vide)
    virtual Vector Launch() const {return Vector{0.,0.};} // Renvoie un vecteur de lancement (pour le joueur)
    virtual void Check(Joueur& joueur, vector<unique_ptr<Element>>& obstacles) {} // Vérifie les interactions
    virtual bool CollisionSlime(const Slime& other) {return false;} // Détecte les collisions entre Slimes

    Vector getPosition() const {
        return pos;
    }
    Vector getSpeed() const {
        return speed;
    }
    int getRadius() const {
        return radius;
    }
    void setSpeed(const Vector& v) {
        speed = v;
    }

    // Surcharge d'affichage pour Element
    void afficher() override {
        Display(); // Utilise Display() pour l'affichage
    }

    virtual ~Slime() = default; // Destructeur virtuel
};


// Classe représentant le joueur (hérite de Slime)
class Joueur : public Slime {
    int frein;                   // indique pour combiend e temps le frein est activé

public:
    Joueur(Vector givenPosition); // Constructeur avec position initiale
    void Display() override;      // Affiche le joueur avec animations
    void Move() override;         // Gère le déplacement du joueur
    Vector Launch() const override;        // Calcule le vecteur de lancement (basé sur la souris)
    bool CollisionSlime(const Ennemi &other); // Détecte les collisions avec le joueur
    int getFrein() const{
        return frein;
    }
    void setFrein(const int f){
        frein = f;
    }
};


// Classe représentant un ennemi (hérite de Slime)
class Ennemi : public Slime {
    bool kill; // Si true, l'ennemi est en mode attaque
public:

    Ennemi(Vector givenPosition); // Constructeur simple
    void Display() override;      // Affiche l'ennemi avec animations différenciées (marche/course/attaque)
    void Move() override;         // Gère le déplacement automatique (va-et-vient)
    void Check(Joueur& joueur, vector<unique_ptr<Element>>& obstacles) override; // Détecte le joueur et déclenche l'attaque
    bool getKill() const{
        return kill;
    }
    void setKill(const bool f){
        kill = f;
    }
};

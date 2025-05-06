#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <regex>
using namespace std;
#include "vector.h"
#include "globals.h"

class Element {// polymorphisme pour les différents objets
public:
    virtual void afficher() = 0; // méthode virtuelle pure
    virtual ~Element() {}       // destructeur virtuel pour éviter les fuites
};

class Collisionable : public Element{
public :
    Vector Point1;
    Vector Point2;
};


class Mur : public Collisionable {
public:
    int epaisseur;
    Mur(Vector PointA, Vector PointB, int epais);
    void afficher() override;
};

// mur, tel que la partie derrière le mur (sens trigo depuis le point 1 vers le point 2)
// est inaccessible et noircie
class Bordure : public Collisionable {
public:
    Bordure(Vector PointA, Vector PointB);
    void afficher();
};

class Boite : public Element {
public:
    Vector Point1;// coin inférieur Gauche
    Vector Point2;// coins supérieur droit
    void afficher() override {
        cout << "Boite" << endl;
    }
};


class Slime;

class Pique : public Element {
public:
    Vector Base;
    Vector Sommet;
    int largeur;
    Vector Sommets[3];
    Pique(Vector Base1, Vector Sommet1, int largeur1);
    void afficher() override {
        cout << "Pique" << endl;
    }

    bool Collision(Slime *slime);
};


class NiveauTextuel {
public:
    int numero = 0;
    std::vector<std::string> lignes;
    int nbElem;
    NiveauTextuel(int nbElem1);
    NiveauTextuel();
    ~NiveauTextuel();
    void detruire();
};
NiveauTextuel ouvrir_niveau(string nom_fichier);



class Niveau{
public:
    vector<Element*> elements;
    int nbElem;

    ~Niveau(); // Pour supprimer les pointeurs

    void ajouterElement(Element* obj);
    void remplir_niveau(NiveauTextuel texte);

    void afficher();

};



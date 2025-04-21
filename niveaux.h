#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#include "vector.h"

class Element {// polymorphisme pour les différents objets
public:
    virtual void afficher() = 0; // méthode virtuelle pure
    virtual ~Element() {}       // destructeur virtuel pour éviter les fuites
};

class Mur : public Element {
public:
    Vector Point1;
    Vector Point2;
    int epaisseur;
    Mur(Vector PointA, Vector PointB, int epais);
    void afficher() override;
};

// mur, tel que la partie derrière le mur (sens trigo depuis le point 1 vers le point 2)
// est inaccessible et noircie
class Bordure : public Element {
public:
    Vector Point1;
    Vector Point2;
    Bordure(Vector PointA, Vector PointB);
    void afficher(int WIDTH, int HEIGHT);
};

class Boite : public Element {
public:
    Vector Point1;// coin inférieur Gauche
    Vector Point2;// coins supérieur droit
    void afficher() override {
        cout << "Boite" << endl;
    }
};

class Pique : public Element {
public:
    Vector Base;
    Vector Sommet;
    int largeur;
    Pique(Vector Base1, Vector Sommet1, int largeur1);
    void afficher() override {
        cout << "Pique" << endl;
    }
};


class NiveauTextuel {
public:
    int numero = 0;
    string* lignes;
    int nbElem;
    NiveauTextuel(int nbElem1);
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

    void afficher();

};

#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "vector.h"
using namespace std;

class Element {// polymorphisme pour les différents objets
public:
    virtual void afficher() = 0; // méthode virtuelle pure
    virtual ~Element() {}       // destructeur virtuel pour éviter les fuites
};

class Mur : public Element {
public:
    Vector Point1;
    Vector Point2;
    Mur(Vector PointA, Vector PointB);
    void afficher() override {
        cout << "Mur" << endl;
    }
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

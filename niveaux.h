#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "vector.h"
using namespace std;

class Objet {// polymorphisme pour les différents objets
public:
    virtual void afficher() = 0; // méthode virtuelle pure
    virtual ~Objet() {}       // destructeur virtuel pour éviter les fuites
};

class Mur : public ObjetJeu {
public:
    Vector Point1;
    Vector Point2;
    Mur(Vector PointA, Vector PointB);
    void afficher() override {
        cout << "Mur" << endl;
    }
};

class Boite : public ObjetJeu {
public:
    void afficher() override {
        cout << "Boite" << endl;
    }
};

class Pique : public ObjetJeu {
public:
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
    vector<Objet*> objets;
    int nbElem;

    ~Niveau(); // Pour supprimer les pointeurs

    void ajouterObjet(ObjetJeu* obj) {
        objets.push_back(obj);
    }

    void afficher() {
        for (ObjetJeu* obj : objets) {
            obj->afficher();
        }
    }

};

#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <regex>
#include <memory>
using namespace std;

#include "vector.h"
#include "globals.h"
#include "player.h"
#include "element.h"

class Mur : public Collisionable {
public:
    int epaisseur;
    Mur(Vector PointA, Vector PointB, int epais);
    void afficher() override;
    bool is_in(Vector v) override;
};

class Porte : public Collisionable {
public:
    int epaisseur;
    Porte(Vector PointA, Vector PointB, int epais);
    void afficher() override;
};

class Bordure : public Collisionable {
public:
    Bordure(Vector PointA, Vector PointB);
    void afficher() override;
    bool is_in(Vector v) override;
};

class Boite : public Element {
public:
    Vector Point1;
    Vector Point2;
    void afficher() override {
        cout << "Boite" << endl;
    }
};


//class Slime : public Element;

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

class Niveau {
public:
    std::vector<std::unique_ptr<Element>> elements;
    std::vector<std::unique_ptr<Collisionable>> collisionables;
    int nbElem;

    // Constructeur par défaut
    Niveau() : nbElem(0) {}

    // Supprimer le constructeur de copie et l'opérateur d'affectation
    Niveau(const Niveau&) = delete;
    Niveau& operator=(const Niveau&) = delete;

    // Déplacer le constructeur et l'opérateur d'affectation
    Niveau(Niveau&&) = default;
    Niveau& operator=(Niveau&&) = default;

    ~Niveau() = default;

    void ajouterElement(std::unique_ptr<Element> obj);
    void remplir_niveau(NiveauTextuel texte);
    void afficher();
};


template<typename Base, typename T>
inline bool instanceof(const T *ptr);

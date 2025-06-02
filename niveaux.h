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
#include "player.h"
#include "element.h"

// élément de base : un mur invisible sur lequel rebondissent les slimes
class Mur : public Collisionable {
public:
    int epaisseur;
    Mur(Vector PointA, Vector PointB, int epais);
    void afficher() override;
    bool is_in(Vector v) override; //
};

// élément linéaire marquant la fin d'un niveau
class Porte : public Collisionable {
public:
    int epaisseur;
    Porte(Vector PointA, Vector PointB, int epais);
    void afficher() override;
    bool is_in(Vector v) override;
};

// objet qui extrait d'un fichier texte (.txt) une liste de strings (ce sont les lignes du fichier texte)
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

// objet contenant tous les propriétés d'un niveau :
// ses éléments physiques et ennemis, son numéro, le nombre d'objets présents
class Niveau {
public:
    std::vector<std::unique_ptr<Element>> elements;
    std::vector<std::unique_ptr<Ennemi>> ennemis;
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

// fonction créant un NiveauTextuel puis un Niveau à partir du premier
Niveau generer_niveau(std::string chemin_niveau);

// utile pour les tests et la description d'un niveau pour déboggage.
template<typename Base, typename T>
inline bool instanceof(const T *ptr);

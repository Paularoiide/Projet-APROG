#pragma once

// Structure représentant un vecteur 2D
struct Vector {
    double x, y;  // Coordonnées x et y
};

// Opérateurs et fonctions vectorielles

// Opérateur d'addition entre deux vecteurs
Vector operator+(Vector a, Vector b);

// Opérateur de soustraction entre deux vecteurs
Vector operator-(Vector a, Vector b);

// Calcule la norme au carré d'un vecteur (optimisation pour éviter sqrt)
double norm2(Vector a);

// Opérateur de multiplication d'un vecteur par un scalaire
Vector operator*(Vector a, double lambda);

// Opérateur de division d'un vecteur par un scalaire
Vector operator/(Vector a, double lambda);

// Opérateur de multiplication commutative (scalaire * vecteur)
Vector operator*(double lambda, Vector a);

// Produit scalaire entre deux vecteurs
double ps(Vector a, Vector b);

// Rotation d'un vecteur d'un angle donné (en degrés)
Vector rotate(Vector a, double angle);

// Distance entre un point et une droite définie par deux points
double distance(Vector a, Vector PointA, Vector PointB);

// Projection orthogonale d'un point sur une droite définie par deux points
Vector projection(Vector a, Vector PointA, Vector PointB);

// Distance euclidienne entre deux points
double distance(Vector a, Vector b);

// Angle entre deux vecteurs (en degrés, sens horaire)
double angle_entre(Vector a, Vector b);


// Conversion de radians en degrés
double deg(double rad);

// Conversion de degrés en radians
double rad(double deg);

// Réflexion d'un vecteur par rapport à une normale unitaire
Vector reflect(Vector v, Vector normal);


// Structure pour représenter des indices (utile pour les sprites)
struct Idx {
    int i;  // Index de ligne
    int j;  // Index de colonne
};

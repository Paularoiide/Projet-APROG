#pragma once
#include "globals.h"

struct Vector{double x,y;};

Vector operator+(Vector a, Vector b); // Sum
Vector operator-(Vector a, Vector b); // Difference
double norm2(Vector a);               // Euclidean norm
Vector operator*(Vector a, double lambda); // Mult. scalar
Vector operator/(Vector a, double lambda); // Div. scalar
Vector operator*(double lambda, Vector a); // Mult. scalar

double ps(Vector a, Vector b);
Vector rotate(Vector a, double angle);
double distance(Vector a, Vector PointA, Vector PointB);
Vector projection(Vector a, Vector PointA, Vector PointB);
double distance(Vector a, Vector b);
double angle_entre(Vector a,Vector b);
double angle_horiz(Vector a);
double deg(double rad);
double rad(double deg);

Vector intersection_bords(Vector point, double angle);

struct Idx{int i; int j;};

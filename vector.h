#pragma once

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
double angle(Vector a,Vector b);

struct Idx{int i; int j;};

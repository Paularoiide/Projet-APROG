
#include "vector.h"
#include <cmath>


Vector operator+(Vector a, Vector b){
    Vector result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;

}
Vector operator-(Vector a, Vector b){
    Vector result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}
double norm2(Vector a){
    return a.x*a.x + a.y*a.y;

}
Vector operator*(Vector a, double lambda){
    Vector result;
    result.x = a.x * lambda;
    result.y = a.y * lambda;
    return result;
}
Vector operator*(double lambda, Vector a){
    return a * lambda;
}

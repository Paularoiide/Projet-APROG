// Slime, jeu de l'atelier APROG, 2025

#include <iostream>
#include <cassert>
using namespace std;
#include <Imagine/Graphics.h>
using namespace Imagine;

#include "physics.h"
#include "vector.h"

struct Slime{
    Color color;
    int rayon = 10;
    void lancer(Vector cible);

};

int main()
{
    cout << "Slime !" << endl;
    return 0;
}

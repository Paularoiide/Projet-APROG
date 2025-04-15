#pragma once

#include <fstream>
#include <string>
#include <iostream>
using namespace std;



class Niveau {
public:
    int numero = 0;
    Niveau(int nbElem);
};
Niveau::Niveau(int nbElem1) {
    int nbElem = nbElem1;
    string* lignes = new string[nbElem];
}

Niveau ouvrir_niveau(string nom_fichier);

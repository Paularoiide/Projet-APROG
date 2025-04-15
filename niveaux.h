#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;



class Niveau {
public:
    int numero = 0;
    string* lignes;
    int nbElem;
    Niveau(int nbElem1);
    ~Niveau();
    void detruire();
};



Niveau ouvrir_niveau(string nom_fichier);


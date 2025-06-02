#pragma once
#include <string>
using namespace std;

extern int WIDTH;
extern int HEIGHT;
extern double dt;
extern int decalage_x; // Décalages pour avoir de la place en haut et à gauche du niveau
extern int decalage_y;
extern string strAssets;

extern bool modeFrein;
extern int sleepTime;

bool fichierExiste(const std::string& cheminComplet);

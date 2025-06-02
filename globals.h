#pragma once
#include <string>
using namespace std;

// définition des variables globales nécessaires dans de nombreux fichiers

extern int WIDTH;      // largeur de la fenêtre
extern int HEIGHT;     // hauteur
extern int decalage_x; // Décalages pour avoir de la place en haut et à gauche du niveau
extern int decalage_y;
extern string strAssets;// emplacement des fichiers relativement au main
extern bool modeFrein; // à désctiver si on souhaite enlever la possibilité de freiner
extern int sleepTime;  // pas de temps entre deux affichages et mouvements
extern double dt;      // permet de faire varier proportionnelement la vitesse du joueur
                       // (si l'on souhaite un jeu calme ou excitant/rapide)

// détermine si un fichier existe dans le chemin relatif donné
bool fichierExiste(const std::string& cheminComplet);

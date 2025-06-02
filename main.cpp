// Slime, jeu de l'atelier APROG, 2025
// Paul Andreis & Charles de Bourgoing

#include <iostream>
#include <cassert>
#include <string>
using namespace std;

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
using namespace Imagine;

#include "physics.h"
#include "vector.h"
#include "affichage.h"
#include "player.h"
#include "niveaux.h"
#include "globals.h"

Imagine::Image<Color> getSlimeSprite(const Imagine::Image<Color>& spriteSheet, int x, int y, int width, int height);

// pour la définition de ces variables globales : voir globals.h

int decalage_x = 250; // Décalage pour avoir de la place en haut et à gauche du niveau
int decalage_y = 12;
int WIDTH = 1472;
int HEIGHT = 832;
double dt=0.1;
string strAssets = "build/assets/";
bool modeFrein = true;
int sleepTime = 50;//ms

// structure regroupant tout ce qui caractérise un niveau : l'ensebmel de ses éléments (Niveau),
// le joueur (permet de fixer sa position initiale),
// le fond d'écran.
struct LevelData {
    Joueur joueur;
    Background background;
    std::shared_ptr<Niveau> niveau;

};

// génère un StartLevel à partir de toutes les informations nécessaires :
// la fenêtre principale, le chemin du fond d'écran, le nom du niveau,
// les coordonnées de départ du joueur pos_x et pos_y ainsi que si c'st le premier niveau ou non
LevelData StartLevel(Window& principale,string background_string, string nom_niv, double pos_x, double pos_y, bool first_level) {
    int width, height;
    Color *C;
    loadColorImage(background_string, C, width, height);
    if (width == 0 || height == 0 || C == nullptr) {
        throw std::runtime_error("Erreur de chargement de l'image : " + background_string);
    }
    Background background = {C, width, height};
    string repert_nivs = "../Projet-APROG/build/assets/Niveaux/";
    if (first_level){ //Si c'est le premier niveau on affiche le menu
        menu(principale, WIDTH, HEIGHT, repert_nivs);
    }
    clearWindow();

    std::shared_ptr<Niveau> niveau1 = std::make_shared<Niveau>(generer_niveau(repert_nivs + nom_niv));


    Vector pos_init = {pos_x, pos_y};
    Joueur joueur = Joueur(pos_init);

    // affichage de tous les éléments :
    Resetscreen(niveau1->elements, background);
    joueur.Display();
    for (auto& ennemi : niveau1->ennemis) {
        ennemi->Display();
    }

    return {joueur, background, niveau1};
}

// après avoir lancé la boucle principale pour un niveau
// cette fonction renvoie le nombre de tirs utilisés par le joueur
// ou -1 si le joueur perd
int PlayLevel(Window& principale,const string& background_string, const string& nom_niv, double pos_x, double pos_y,bool first_level) {
    LevelData data = StartLevel(principale,background_string, nom_niv, pos_x, pos_y,first_level );
    Joueur& joueur = data.joueur;
    Background& background = data.background;
    Niveau& niveau1 = *data.niveau;
    int nb_tir = 0;
    while (true) {
        joueur.speed = joueur.Launch();
        bool porteTouchee = false;
        nb_tir +=1;

        for (int timeStep = 0; timeStep <= 250 * freqDisplay; timeStep++) {
            if ((timeStep % freqDisplay) == 0) {
                noRefreshBegin();
                Resetscreen(niveau1.elements, background);

                // Affichage des ennemis (slimes)
                for (auto& ennemi : niveau1.ennemis) {
                    ennemi->Display();
                }

                joueur.Display();
                noRefreshEnd();
                milliSleep(sleepTime);
            }

            // === Collisions avec les éléments du niveau ===
            for (auto& elem : niveau1.elements) {
                Collisionable* d = dynamic_cast<Collisionable*>(elem.get());
                if (!d) continue;

                if (joueur.Collision(d)) {
                    if (dynamic_cast<Porte*>(elem.get())) {
                        porteTouchee = true;
                        break;
                    } else {
                        joueur.Shock(d);
                    }
                }
                for (auto& ennemi : niveau1.ennemis) {
                    if (ennemi->Collision(d)){
                        ennemi->Shock(d);
                    }
                }

            }

            if (porteTouchee) break;

            // === Mouvement et détection des ennemis ===
            for (auto& ennemi : niveau1.ennemis) {
                ennemi->Check(joueur,niveau1.elements);
                if (ennemi->kill){
                    Vector dif = joueur.pos - ennemi->pos;
                    ennemi->speed = 0.5 * (dif)/sqrt(norm2(dif));
                }
                ennemi->Move();

            }

            // === Mouvement du joueur ===
            int key = keyboard();
            if(key=='w') {
                joueur.frein=1000/sleepTime;
            } else {
                if(joueur.frein>0) {
                    joueur.frein--;
                }
            }

            joueur.Move();
            Vector acc = Acceleration(joueur.speed,(joueur.frein>0));
            joueur.Accelerate(acc);

            if (norm2(joueur.speed) <= 0.0005) break;
        }

        for (auto& ennemi : niveau1.ennemis) {
            if (joueur.CollisionSlime(*ennemi)) {
                        closeWindow(principale);
                return -1; // Code si échec de la fuite
            }
        }

        if (porteTouchee) {
            closeWindow(principale);
            return nb_tir;
        }
    }
    flushEvents();
    return nb_tir;
}


int main() {
    srand(time(0)); // Initialisation de l'aléatoire

    string nomstxts[3] = {"Lab0.txt","Lab1.txt","Lab2.txt"};
    string path0 = stringSrcPath(strAssets + "Niveaux/lab0.png");
    string path1 = stringSrcPath(strAssets + "Niveaux/lab1.png");
    string path2 = stringSrcPath(strAssets + "Niveaux/lab2.png");

    int nb_tir = 0;// nombre total de tirs
    int score_niveau;// nombre de tirs pour le niveau actuel
    string paths[3] = {path0,path1,path2};

    //parcourt des niveaux
    for(int i=0;i<2;i++) {
        Window principale = openWindow(WIDTH, HEIGHT, "Jeu APROJ - Slime");
        score_niveau = PlayLevel(principale, paths[i], nomstxts[i], 591 + decalage_x, 180 + decalage_y, (i==0));

        if (score_niveau == -1){ // Dans ce cas c'est Game Over
            GameOver(false,0);
            return 0;
        }
        nb_tir += score_niveau;
    }

    GameOver(true,nb_tir);
    endGraphics();
    return 0;
}




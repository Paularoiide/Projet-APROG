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

int decalage_x = 250; // Décalage pour avoir de la place en haut et à gauche du niveau
int decalage_y = 12;
int WIDTH = 1472;
int HEIGHT = 832;
double dt=0.1;
string strAssets = "build/assets/";
bool modeFrein = true;
int sleepTime = 50;//ms

bool fichierExiste(const std::string& cheminComplet) {
    std::ifstream fichier(cheminComplet.c_str());
    return fichier.good();
}



struct LevelData {
    Joueur joueur;
    Background background;
    std::shared_ptr<Niveau> niveau;

};

LevelData StartLevel(Window& principale,string background_string, string nom_niv, double pos_x, double pos_y, bool first_level) {
    int width, height;
    Color *C;
    loadColorImage(background_string, C, width, height);
    Background background = {C, width, height};
    string repert_nivs = "../Projet-APROG/build/assets/Niveaux/";
    if (first_level){ //Si c'est le premier niveau on affiche le menu
        menu(principale, WIDTH, HEIGHT, repert_nivs);
    }
    clearWindow();

    std::shared_ptr<Niveau> niveau1 = std::make_shared<Niveau>(generer_niveau(repert_nivs + nom_niv));
    cout << "niveau_genere" << endl;
    cout << "niveau_affiche" << endl;

    Vector pos_init = {pos_x, pos_y};
    Joueur joueur = Joueur(pos_init);

    Resetscreen(niveau1->elements, background);
    joueur.Display();
    for (auto& ennemi : niveau1->ennemis) {
        ennemi->Display();
    }

    return {joueur, background, niveau1};
}


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
            //cout <<"touche : "<<key<<endl;
            if(key=='w') {
                joueur.frein=1000/sleepTime;
                //cout << "frein active"<<endl;
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
                cout << "Slime attrapé ! Game Over." << endl;
                        closeWindow(principale);
                return -1; // Code si échec de la fuite
            }
        }

        if (porteTouchee) {
            cout << "Porte touchee ! Fin du niveau." << endl;
            closeWindow(principale);
            return nb_tir;
        }

        cout << "Waiting for a new pulse" << endl;
    }
    flushEvents();
    return nb_tir;
}


int main() {
    srand(time(0)); // Initialisation de l'aléatoire
    string path0 = stringSrcPath(strAssets + "Niveaux/lab0.png");
    string path1 = stringSrcPath(strAssets + "Niveaux/lab1.png");

    int nb_tir = 0;
    int score_niveau;
    Window principale = openWindow(WIDTH, HEIGHT, "Jeu APROJ - Slime");
    score_niveau = PlayLevel(principale, path0, "Lab0.txt", 591 + decalage_x, 180 + decalage_y, true);

    if (score_niveau == -1){ // Dans ce cas c'est Game Over
        GameOver(false,0);
        return 0;
    }
    nb_tir += score_niveau;

    principale = openWindow(WIDTH, HEIGHT, "Jeu APROJ - Slime");

    score_niveau = PlayLevel(principale, path1, "Lab1.txt", 50 + decalage_x, 180 + decalage_y, false);

    if (score_niveau == -1){ // Dans ce cas c'est Game Over
        GameOver(false,0);
        return 0;
    }
    nb_tir += score_niveau;

    GameOver(true,nb_tir);
    endGraphics();
    return 0;
}




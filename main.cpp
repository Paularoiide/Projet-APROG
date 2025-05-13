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

int WIDTH = 512;
int HEIGHT = 512;
double dt=0.01;

void generer_niveau(string chemin_niveau) {
    NiveauTextuel texte;
    texte = ouvrir_niveau(chemin_niveau);

}

void menu(const int width,const int height) {
    const int BUTTON_WIDTH = 100;
    const int BUTTON_HEIGHT = 50;
    int x_button = width/2-BUTTON_WIDTH/2;
    int y_button = height/2-BUTTON_HEIGHT/2;
    drawButton(x_button,y_button,BUTTON_WIDTH,BUTTON_HEIGHT,BLUE, "Commencer");
    while (true) {
        int x, y;
        if (getMouse(x, y)) {
            if (x >= x_button && x <= x_button + BUTTON_WIDTH &&
                y >= y_button && y <= y_button + BUTTON_HEIGHT) {
                drawButton(x_button,y_button,BUTTON_WIDTH,BUTTON_HEIGHT,GREEN, "Cliqué !");
                    //showMessage("Bouton cliqué !");
                    break; // Quitter après clic
            }
        }
        milliSleep(20);
    }
}

int main()
{
    openWindow(WIDTH, HEIGHT,"Jeu APROJ - Slime");
    //Affichage du menu
    //menu(WIDTH,HEIGHT);
    //Afichage Test sprite
    int j = 0;
    // Test du Slime
    Vector pos_init = {200,200};
    Slime slime = Slime(role_Slime::JOUEUR, pos_init);
    slime.Display();
    while (true){
      slime.Lancer();
    }


    cout << "Slime !" << endl;
    endGraphics();
    return 0;
}



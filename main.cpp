// Slime, jeu de l'atelier APROG, 2025
// Paul Andreis & Charles de Bourgoing

#include <iostream>
#include <cassert>
using namespace std;
#include <Imagine/Graphics.h>
using namespace Imagine;

#include "physics.h"
#include "vector.h"
#include "affichage.h"

enum class role_Slime {
    JOUEUR, SLIME_ENEMY, PNJ
};

class Slime{

public:
    Color color;
    int rayon = 10;
    void lancer(Vector cible);
    Slime(Color givenColor, role_Slime givenRole);

};
Slime::Slime(Color givenColor, role_Slime givenRole) {// constructeur
    color = givenColor;
    role_Slime role = givenRole;
}



void menu(const int WIDTH,const int HEIGHT) {
    const int BUTTON_WIDTH = 100;
    const int BUTTON_HEIGHT = 50;
    int x_button = WIDTH/2-BUTTON_WIDTH/2;
    int y_button = HEIGHT/2-BUTTON_HEIGHT/2;
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
    const int WIDTH=512;
    const int HEIGHT=512;
    openWindow(WIDTH, HEIGHT,"Jeu APROJ - Slime");
    cout << "Slime !" << endl;
    menu(WIDTH,HEIGHT);
    endGraphics();
    return 0;
}

// Slime, jeu de l'atelier APROG, 2025
// Paul Andreis & Charles de Bourgoing

#include <iostream>
#include <cassert>
using namespace std;

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
using namespace Imagine;

#include "physics.h"
#include "vector.h"
#include "affichage.h"
#include "player.h"
#include "niveaux.h"

Imagine::Image<Color> getSlimeSprite(const Imagine::Image<Color>& spriteSheet, int x, int y, int width, int height);




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
    Imagine::Image<RGBA<unsigned char>> slimebuddy;
    Imagine::Image<RGBA<unsigned char>> slimeSheet;
    load(slimebuddy, srcPath("slimebuddy.png"));
    RGBA<unsigned char> background_color = slimebuddy(1,1);
    cout << "Pixel (0,0) = ("
         << int(background_color.r()) << ", "
         << int(background_color.g()) << ", "
         << int(background_color.b()) << ", "
         << int(background_color.a()) << ")" << endl;

    createMaskFromColor(slimebuddy,background_color);
    openWindow(WIDTH, HEIGHT,"Jeu APROJ - Slime");

    // Découper un sprite
    Imagine::Image<RGBA<unsigned char>> slime = getSlimeSprite(slimebuddy, 0, 0, 32, 32);
    cout << "Slime !" << endl;

    display(slime, 100, 100); // Affiche le sprite découpé à l'écran
    menu(WIDTH,HEIGHT);
    endGraphics();
    return 0;
}



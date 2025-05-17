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

Niveau generer_niveau(std::string chemin_niveau) {
    NiveauTextuel texte = ouvrir_niveau(chemin_niveau);
    Niveau niveauActuel;
    niveauActuel.remplir_niveau(texte);
    //cout << "niveau rempli" << endl;
    //cout << "quelque test à l'interieur de generer_niveau" << endl;
            cout << "analyse du niveau :"<< endl;
    for(int i=0;i<niveauActuel.elements.size();i++) {
        cout << "Type de l'objet : " << typeid(*niveauActuel.elements[i]).name() << endl;
    }
    //cout<<"fin de la fonction generer_niveau"<<endl;
    return niveauActuel;
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
    // creation du niveau
    //Niveau niveau1;
    //cout << "Répertoire de travail courant : " << std::filesystem::current_path() << endl;
    Niveau niveau1 = generer_niveau("../Projet-APROG/build/assets/Niveaux/Intro.txt");
    cout << "niveau_genere" << endl;
    //cout << "réalisations de tests sur le niveau." << endl;
    //cout << "Test avec typeid(*elements[i]).name(). Type de l'objet niveau1.elements[0] : " << typeid(*niveau1.elements[0]).name() << endl;
    niveau1.afficher();
    cout << "niveau_affiche" << endl;
    // Test du Slime
    Vector pos_init = {200,200};
    Slime slime = Slime(role_Slime::KILLER, pos_init);

    slime.Display();
    while (true){
      Vector dir = slime.Launch();
        // désormais inclus dans Lancer
      slime.Lancer(dir,niveau1.elements);
    }

    cout << "Slime !" << endl;
    endGraphics();
    return 0;
}



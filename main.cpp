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

int WIDTH = 1500;
int HEIGHT = 1000;
double dt=0.01;

bool fichierExiste(const std::string& cheminComplet) {
    std::ifstream fichier(cheminComplet.c_str());
    return fichier.good();
}

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
int entrer_code(const int width, const int height,const int BUTTON_WIDTH, const int BUTTON_HEIGHT) {

    Window fenCode = openWindow(300,200,"code");
    showWindow(fenCode);
    setActiveWindow(fenCode);
    setBackGround(Color(145,30,170));

    string instruction = "entrez le code :";
    string code = "";
    int font=10;
    // Boucle principale
    bool fin = false;
    while (!fin) {
        // Effacer la fenêtre
        clearWindow();

        // Afficher l'instruction
        drawString(BUTTON_WIDTH/2, BUTTON_WIDTH/2, instruction.c_str(), BLACK, font);

        // Afficher le code en cours de saisie
        drawString(BUTTON_WIDTH/2, BUTTON_WIDTH, code.c_str(), BLACK, 20);

        // Rectangle autour de la zone de texte
        drawRect(BUTTON_WIDTH/2-5, BUTTON_WIDTH/2-20, 2*BUTTON_WIDTH, BUTTON_WIDTH/2, BLACK);

        // Gérer les événements
        Event ev;
        getEvent(0, ev);

        switch (ev.type) {
            case EVT_KEY_ON:
                if (ev.key == KEY_BACK && !code.empty()) {
                    // Supprimer le dernier caractère
                    code.pop_back();
                } else if (ev.key == KEY_RETURN) {
                    // Valider avec Entrée
                    fin = true;
                    closeWindow(fenCode);
                    cout << "Entree. code selectionne : " << code << endl;
                    return stoi(code);
                } else if (isdigit(ev.key)) {
                    // Ajouter un chiffre
                    code += char(ev.key);
                }
                break;
            case EVT_BUT_ON:
                // Valider avec un clic
                closeWindow(fenCode);
                cout << "code selectionne : " << code << endl;
                return stoi(code);
            default:
                break;
        }
        milliSleep(20);
    }
    return -1;
    cout << "erreur" << endl;
}

int fen_niveaux(const int width, const int height,const int BUTTON_WIDTH, const int BUTTON_HEIGHT, string repertoire) {

    Window fenNiv = openWindow(300,200,"code");
    showWindow(fenNiv);
    setActiveWindow(fenNiv);
    setBackGround(Color(145,30,170));
    std::vector<std::string> fichiers = {"Intro.txt"};
    int x_button = width/2-BUTTON_WIDTH/2;
    int i=1;
    while (fichierExiste(repertoire +"Niveau" + std::to_string(i) + ".txt")) {
        drawButton(x_button,BUTTON_HEIGHT*(1.5*i+1/2),BUTTON_WIDTH,BUTTON_HEIGHT,WHITE,"Niveau "+to_string(i));
    }
    while(true) {
        int x, y;
        if (getMouse(x, y)) {
            for(int j=0;j<i;j++) {
                if (x >= x_button && x <= x_button + BUTTON_WIDTH &&
                        y >= BUTTON_HEIGHT*(1.5*i+1/2) && y <= BUTTON_HEIGHT*(1.5*i+3/2)) {
                        closeWindow(fenNiv);
                        return j;
                }
            }
        }
    }
}

int menu(Window fenMenu, const int width,const int height, const string repertoire) {
    const int BUTTON_WIDTH = 100;
    const int BUTTON_HEIGHT = 50;
    int x_button_begin = width/2-BUTTON_WIDTH/2;
    int y_button_begin = height/2-BUTTON_HEIGHT/2;
    int x_button_admin = width-3/2*BUTTON_WIDTH;
    int y_button_admin = height-3/2*BUTTON_HEIGHT;
    drawButton(x_button_begin,y_button_begin,BUTTON_WIDTH,BUTTON_HEIGHT,BLUE, "Commencer");
    drawButton(x_button_admin,y_button_admin,BUTTON_WIDTH,BUTTON_HEIGHT,BLUE, "?");
    while (true) {
        int x, y;
        if (getMouse(x, y)) {
            if (x >= x_button_begin && x <= x_button_begin + BUTTON_WIDTH &&
                y >= y_button_begin && y <= y_button_begin + BUTTON_HEIGHT) {
                drawButton(x_button_begin,y_button_begin,BUTTON_WIDTH,BUTTON_HEIGHT,GREEN, "Cliqué !");
                    //showMessage("Bouton cliqué !");
                return 0;
                break; // Quitter après clic
            }
            if (x >= x_button_admin && x <= x_button_admin + BUTTON_WIDTH &&
                y >= y_button_admin && y <= y_button_admin + BUTTON_HEIGHT) {
                if(entrer_code(width, height, BUTTON_WIDTH,BUTTON_HEIGHT)==511) {
                    cout << "code_entre" << endl;
                    int select = fen_niveaux(width,height,BUTTON_WIDTH,BUTTON_HEIGHT,repertoire);
                    cout << "niveau selectionne : " << select << endl;
                }
                setActiveWindow(fenMenu);
                return(-1);
            }
        }
        milliSleep(20);
    }
}

int main()
{
    int width,height;
    Color *C;                                           // load color image (Color array)
    loadColorImage(srcPath("lab1.png"),C,width,height);
    Background background = {C,width,height};
    Window principale = openWindow(WIDTH, HEIGHT,"Jeu APROJ - Slime");
    //Affichage du menu
    // creation du niveau

    string repert_niv = "../Projet-APROG/build/assets/Niveaux/";
    menu(principale,WIDTH,HEIGHT,repert_niv);
    clearWindow();
    Niveau niveau1 = generer_niveau(repert_niv + "Lab1.txt");
    cout << "niveau_genere" << endl;
    cout << "niveau_affiche" << endl;
    Vector pos_init = {50,300};
    Slime slime = Slime(role_Slime::JOUEUR, pos_init);
    Resetscreen(niveau1.elements, background);
    slime.Display();

    while (true){
      Vector dir = slime.Launch();
      slime.Lancer(dir,niveau1.elements,background);
    }

    cout << "Slime !" << endl;
    endGraphics();
    return 0;
}



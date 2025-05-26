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
double dt=1;
string strAssets = "build/assets/";

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
    /*cout << "analyse du niveau :"<< endl;
    for(int i=0;i<niveauActuel.elements.size();i++) {
        cout << "Type de l'objet : " << typeid(*niveauActuel.elements[i]).name() << endl;
    }*/
    //cout<<"fin de la fonction generer_niveau"<<endl;
    return niveauActuel;
}
int entrer_code(const int width, const int height, const int BUTTON_WIDTH, const int BUTTON_HEIGHT) {

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
    Color *C;
    int w;
    int h;
    string Image_menu = stringSrcPath(strAssets + "Slimescape.png");
    loadColorImage(Image_menu, C, w, h);
    putColorImage(0,0,C,w,h,false,1.);
    const int BUTTON_WIDTH = 100;
    const int BUTTON_HEIGHT = 50;
    int x_button_begin = width/2-BUTTON_WIDTH/2;
    int y_button_begin = height/1.2-BUTTON_HEIGHT/2;
    int x_button_admin = width-3/2*BUTTON_WIDTH;
    int y_button_admin = height-3/2*BUTTON_HEIGHT;
    drawButton(x_button_begin,y_button_begin,BUTTON_WIDTH,BUTTON_HEIGHT,WHITE, "Commencer");
    drawButton(x_button_admin,y_button_admin,BUTTON_WIDTH,BUTTON_HEIGHT,WHITE, "?");
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

struct LevelData {
    Slime slime;
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
    Slime slime = Slime(role_Slime::JOUEUR, pos_init);

    Resetscreen(niveau1->elements, background);
    slime.Display();

    return {slime, background, niveau1};
}


int PlayLevel(Window& principale,const string& background_string, const string& nom_niv, double pos_x, double pos_y,bool first_level) {
    LevelData data = StartLevel(principale,background_string, nom_niv, pos_x, pos_y,first_level );
    Slime& slime = data.slime;
    Background& background = data.background;
    Niveau& niveau1 = *data.niveau;
    int nb_tir = 0;
    while (true) {
        slime.speed = slime.Launch();
        bool porteTouchee = false;
        nb_tir +=1;

        for (int timeStep = 0; timeStep <= 250 * freqDisplay; timeStep++) {
            if ((timeStep % freqDisplay) == 0) {
                noRefreshBegin();
                Resetscreen(niveau1.elements, background);

                // Affichage des ennemis (slimes)
                for (auto& ennemi : niveau1.ennemis) {
                    ennemi->Display(); // plus besoin de cast
                    cout << ennemi->pos.x;
                }

                slime.Display();
                noRefreshEnd();
                milliSleep(50);
            }

            // === Collisions avec les éléments du niveau ===
            for (auto& elem : niveau1.elements) {
                Collisionable* d = dynamic_cast<Collisionable*>(elem.get());
                if (!d) continue;

                if (slime.Collision(d)) {
                    if (dynamic_cast<Porte*>(elem.get())) {
                        porteTouchee = true;
                        break;
                    } else {
                        slime.Shock(d);
                    }
                }
                for (auto& ennemi : niveau1.ennemis) {
                    if (ennemi->Collision(d)){
                        ennemi->Shock(d);
                    }
                }

            }

            if (porteTouchee) break;

            // === Collisions avec les ennemis ===
            for (auto& ennemi : niveau1.ennemis) {
                Collisionable* d = dynamic_cast<Collisionable*>(ennemi.get());
                if (!d) continue;

                if (slime.Collision(d)) {
                    slime.Die(); // ou une logique spéciale contre les ennemis
                }
            }

            // === Mouvement des ennemis ===
            for (auto& ennemi : niveau1.ennemis) {
                //ennemi->Move();
                //Vector acc = Acceleration(ennemi->speed);
                //ennemi->Accelerate(acc);
            }

            // === Mouvement du joueur ===
            slime.Move();
            Vector acc = Acceleration(slime.speed);
            slime.Accelerate(acc);

            if (norm2(slime.speed) <= 0.0005) break;
        }

        if (porteTouchee) {
            cout << "Porte touchee ! Fin du niveau." << endl;
            closeWindow(principale);
            return nb_tir;
        }

        cout << "Waiting for a new pulse" << endl;
    }
    return nb_tir;
}


int main() {
    srand(time(0)); // Initialisation de l'aléatoire
    string path0 = stringSrcPath(strAssets + "Niveaux/lab0.png");
    string path1 = stringSrcPath(strAssets + "Niveaux/lab1.png");

    int nb_tir = 0;

    Window principale = openWindow(WIDTH, HEIGHT, "Jeu APROJ - Slime");

    nb_tir += PlayLevel(principale, path0, "Lab0.txt", 591 + decalage_x, 180 + decalage_y, true);

    principale = openWindow(WIDTH, HEIGHT, "Jeu APROJ - Slime");

    nb_tir += PlayLevel(principale, path1, "Lab1.txt", 50 + decalage_x, 180 + decalage_y, false);

    // Fenêtre finale pour afficher le score
    openWindow(WIDTH, HEIGHT, "Fin du jeu");


    // Affichage du texte
    string message = "Bravo ! Score final : " + to_string(nb_tir) + " tirs";
    // Création de 5 Slimes à des positions aléatoires
    std::vector<Slime> slimes;

    for (int i = 0; i < 70; ++i) {
        Vector pos = Vector{
            static_cast<double>(rand() % WIDTH),
            static_cast<double>(rand() % HEIGHT)
        };
        slimes.push_back(Slime(role_Slime::JOUEUR, pos));
    }


    // Affichage continu des 5 Slimes jusqu'au clic
    while (true) {
        noRefreshBegin();
        clearWindow();
        drawString(WIDTH / 2 - 100, HEIGHT/2, message, BLACK, 20);
        for (Slime& s : slimes) {
            s.Display();
        }
        noRefreshEnd();
        milliSleep(80);

    }

    endGraphics();
    return 0;
}




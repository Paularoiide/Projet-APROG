#include "vector.h"
#include "affichage.h"
#include "player.h"
#include "globals.h"


using namespace std;

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
using namespace Imagine;


// Dessine un bouton rectangulaire avec texte centré
void drawButton(int x, int y, int width, int height, Color rectColor, const string& label) {
    fillRect(x,y, width, height, rectColor);
    drawString(x + width/10, y + height*3/5, label, BLACK, width/10);
}

// Effectue un fondu au noir progressif de l'écran
void fonduNoir(const int WIDTH, const int HEIGHT, int duree, int nb_pas) {
    for(int i=0;i<nb_pas;i++) {
        int niv = 255*i/nb_pas;
        RGBA gris(255,255,255,niv);
        fillRect(0,0,WIDTH,HEIGHT,gris);
        milliSleep(duree/nb_pas);

    }
}

// Charge et extrait une sous-image (sprite) depuis une image source
AlphaColor* getSprite(const std::string& path, int i, int j, int w1, int h1) {
    AlphaColor* C;
    int w, h;
    loadAlphaColorImage(path, C, w, h);
    if (w == 0 || h == 0 || C == nullptr) {
        throw std::runtime_error("Erreur de chargement de l'image : " + path);
    }

    AlphaColor* D = new AlphaColor[w1 * h1]; // Allocation pour le sprite extrait

    for (int y = 0; y < h1; y++) {
        for (int x = 0; x < w1; x++) {
            int srcX = i * w1 + x;
            int srcY = j * h1 + y;
            D[y * w1 + x] = C[srcY * w + srcX]; // Copie du pixel
        }
    }

    return D; // Pointeur vers la zone extraite (sprite)
}

// Affiche un sprite à une position donnée en centrant l'image
void putSprite(const std::string& path, double x, double y, int i, int j, int w1, int h1) {
    AlphaColor* D = getSprite(path, i, j, w1, h1);
    putAlphaColorImage(x - w1 / 2.0, y - h1 / 2.0, D, w1, h1,false,1.2);
}

// Réinitialise l'écran avec le fond et les obstacles
void Resetscreen(vector<unique_ptr<Element>>& obstacles,Background background){
    clearWindow();
    for (auto& obj : obstacles) {
        Element* rawPtr = obj.get();
        if (rawPtr) { // Vérifiez que le pointeur n'est pas nul
        } else {
            cerr << "objet non construit au moment de l'affichage" << endl;
        }
    }
    putColorImage(decalage_x,decalage_y,background.Texture,background.w,background.h);
}

// Affiche l'écran de fin de jeu avec message et animations
void GameOver(bool win, int nb_tir) {
    // Fenêtre finale pour afficher le score
    openWindow(WIDTH, HEIGHT, "Fin du jeu");
    string message_game_over;
    if (win){
        message_game_over = "Bravo ! Score final : " + to_string(nb_tir) + " tirs";
    } else {
        message_game_over = "Tu as été attrapé, Game Over";
    }

    const int NB_SLIMES = 70; // Constante pour le nombre de slimes
    // Tableau fixe de pointeurs uniques vers des Slimes (pour ne pas avoir à gérer de deletes) :
    std::array<std::unique_ptr<Slime>, NB_SLIMES> slimes;
    for (int i = 0; i < NB_SLIMES; ++i) { // Création des slimes aux positions aléatoires
        Vector pos = Vector{
            static_cast<double>(rand() % WIDTH),
            static_cast<double>(rand() % HEIGHT)
        };

        if (win) {
            slimes[i] = std::make_unique<Joueur>(pos);
        } else {
            slimes[i] = std::make_unique<Ennemi>(pos);
        }
    }
    while (true) {
        noRefreshBegin();
        clearWindow();

        for (const auto& slime : slimes) {
            slime->Display();
        }

        drawString(WIDTH / 2 - 100, HEIGHT/2, message_game_over, BLACK, 20);
        noRefreshEnd();
        milliSleep(80);
    }
}

// Ouvre une fenêtre expliquant comment jouer
void afficherTexteInfo(int width, int height) {
    Window fenInfo = openWindow(width, height,"À propos du jeu");
        setActiveWindow(fenInfo);
    Color *C;
    int w;
    int h;
    string Image_aide = stringSrcPath(strAssets + "aide.png");
    loadColorImage(Image_aide, C, w, h);
    if (w == 0 || h == 0 || C == nullptr) {
        throw std::runtime_error("Erreur de chargement de l'image : " + Image_aide);
    }
    putColorImage(0,0,C,w,h,false,1.);

    // Attente clic pour quitter
    int clic_x, clic_y;
    while (!getMouse(clic_x, clic_y)) {
        milliSleep(20);
    }

    closeWindow(fenInfo);
}



// Affiche le menu principal avec boutons de démarrage et aide
int menu(Window fenMenu, const int width, const int height) {
    Color *C;
    int w;
    int h;
    string Image_menu = stringSrcPath(strAssets + "Slimescape.png");
    loadColorImage(Image_menu, C, w, h);
    if (w == 0 || h == 0 || C == nullptr) {
        throw std::runtime_error("Erreur de chargement de l'image : " + Image_menu);
    }

    putColorImage(0,0,C,w,h,false,1.);
    const int BUTTON_WIDTH = 100;
    const int BUTTON_HEIGHT = 50;
    int x_button_begin = width/2-BUTTON_WIDTH/2;
    int y_button_begin = height/1.2-BUTTON_HEIGHT/2;
    int x_button_admin = width-3/2*BUTTON_WIDTH;
    int y_button_admin = height-3/2*BUTTON_HEIGHT;
    drawButton(x_button_begin,y_button_begin,BUTTON_WIDTH,BUTTON_HEIGHT,WHITE, "Commencer");
    drawButton(x_button_admin,y_button_admin,BUTTON_WIDTH,BUTTON_HEIGHT,WHITE, "Règles");
        while (true) {
        int x, y;
        if (getMouse(x, y)) {
            if (x >= x_button_begin && x <= x_button_begin + BUTTON_WIDTH &&
                y >= y_button_begin && y <= y_button_begin + BUTTON_HEIGHT) {
                milliSleep(200);
                return 0;
                break; // Quitter après clic
            }
            if (x >= x_button_admin && x <= x_button_admin + BUTTON_WIDTH &&
                y >= y_button_admin && y <= y_button_admin + BUTTON_HEIGHT) {
                afficherTexteInfo(width, height);
                setActiveWindow(fenMenu);
            }
        }
        milliSleep(20);
    }
}

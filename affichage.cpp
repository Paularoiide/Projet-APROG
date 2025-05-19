#include "affichage.h"
using namespace std;

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
using namespace Imagine;

void drawButton(int x, int y, int width, int height, Color rectColor, const string& label) {
    fillRect(x,y, width, height, rectColor);
    drawString(x + width/10, y + height*3/5, label, BLACK, width/10);
}

void fonduNoir(const int WIDTH, const int HEIGHT, int duree, int nb_pas) {
    for(int i=0;i<nb_pas;i++) {
        int niv = 255*i/nb_pas;
        RGBA gris(255,255,255,niv);
        fillRect(0,0,WIDTH,HEIGHT,gris);
        milliSleep(duree/nb_pas);

    }
}

AlphaColor* getSprite(const std::string& path, int i, int j, int w1, int h1) {
    AlphaColor* C; // Image complète
    int w, h;      // Largeur et hauteur de l'image
    loadAlphaColorImage(path, C, w, h);

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

void putSprite(const std::string& path, double x, double y, int i, int j, int w1, int h1) {
    AlphaColor* D = getSprite(path, i, j, w1, h1);
    putAlphaColorImage(x - w1 / 2.0, y - h1 / 2.0, D, w1, h1);
}


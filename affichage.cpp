#include "affichage.h"

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

Imagine::Image<Color> getSlimeSprite(const Imagine::Image<Color>& spriteSheet, int x, int y, int width, int height) {
    Imagine::Image<Color> sprite(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            sprite(i, j) = spriteSheet(x + i, y + j);
        }
    }
    return sprite;
}



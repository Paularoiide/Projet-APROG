#pragma once

#include <Imagine/Graphics.h>
using namespace Imagine;
#include <string>
using namespace std;

const int WIDTH=512;
const int HEIGHT=512;
const double dt = 0.01;
const int freqDisplay = 10 * (1/dt);


void drawButton(int x, int y, int width, int height, Color rectColor, const string& label);
void fonduNoir(const int WIDTH, const int HEIGHT, int duree, int nb_pas);

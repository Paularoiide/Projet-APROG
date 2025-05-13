#pragma once

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

using namespace Imagine;
#include <string>
using namespace std;

#include "globals.h"

//const double dt = 0.01;
const int freqDisplay = 10 * (1/dt);


void drawButton(int x, int y, int width, int height, Color rectColor, const string& label);
void fonduNoir(const int WIDTH, const int HEIGHT, int duree, int nb_pas);


AlphaColor* getSprite(const std::string& path,int i, int j, int w1 = 32, int h1 = 32);
void putSprite(const std::string& path,double x, double y,int i, int j, int w1 = 32, int h1 = 32);



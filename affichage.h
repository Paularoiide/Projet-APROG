#include "niveaux.h"
#include "globals.h"
#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <regex>
#include <memory>
using namespace std;

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

using namespace Imagine;



//const double dt = 0.01;
const int freqDisplay = 10 * (1/dt);


void drawButton(int x, int y, int width, int height, Color rectColor, const string& label);
void fonduNoir(const int WIDTH, const int HEIGHT, int duree, int nb_pas);


AlphaColor* getSprite(const std::string& path,int i, int j, int w1 = 32, int h1 = 32);
void putSprite(const std::string& path,double x, double y,int i, int j, int w1 = 32, int h1 = 32);
void Resetscreen(vector<unique_ptr<Element>>& obstacles);



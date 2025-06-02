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

#include "globals.h"
#include "element.h"

// Fréquence d'affichage basée sur le pas de temps
const int freqDisplay = 10 * (1/dt);

// Dessine un bouton avec texte centré
void drawButton(int x, int y, int width, int height, Color rectColor, const string& label);

// Effectue un fondu au noir progressif
void fonduNoir(const int WIDTH, const int HEIGHT, int duree, int nb_pas);

// Charge et extrait un sprite depuis une image
AlphaColor* getSprite(const std::string& path, int i, int j, int w1 = 32, int h1 = 32);

// Affiche un sprite centré à une position
void putSprite(const std::string& path, double x, double y, int i, int j, int w1 = 32, int h1 = 32);

// Réinitialise l'affichage avec fond et obstacles
void Resetscreen(vector<unique_ptr<Element>>& obstacles, Background background);

// Affiche l'écran de fin de partie
void GameOver(bool win, int nb_tir);

// Affiche le menu principal et gère les interactions
int menu(Window fenMenu, const int width, const int height, const string repertoire);

// Affiche la sélection de niveaux
int fen_niveaux(const int width, const int height, const int BUTTON_WIDTH, const int BUTTON_HEIGHT, string repertoire);

// Demande la saisie d'un code numérique
int entrer_code(const int width, const int height, const int BUTTON_WIDTH, const int BUTTON_HEIGHT);

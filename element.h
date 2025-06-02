#pragma once
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
using namespace Imagine;

#include "vector.h"

//Classe représentant les différents éléments d'un niveau
class Element {
public:
    virtual void afficher() = 0;
    virtual ~Element() {}
};

//Ensemble des éléments avec lesquels il est possible d'avoir une collision
class Collisionable : public Element {
public:
    Vector Point1;
    Vector Point2;
    virtual void afficher() override = 0;
};

struct Background{Color* Texture; int w,h;};

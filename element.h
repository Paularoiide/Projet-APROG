#pragma once
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
using namespace Imagine;

#include "vector.h"

class Element {
public:
    virtual void afficher() = 0;
    virtual ~Element() {}
};

class Collisionable : public Element {
public:
    Vector Point1;
    Vector Point2;
    virtual void afficher() override = 0;
    virtual bool is_in(Vector v) = 0;
};

struct Background{Color* Texture; int w,h;};

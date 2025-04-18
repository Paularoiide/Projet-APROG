#include "player.h"
#include "physics.h"
#include "affichage.h"

Slime::Slime(role_Slime givenRole, Vector givenPos) {// constructeur
    pos = givenPos;
    role_Slime role = givenRole;
}

void Slime::Display(){
    putSprite(pos.x,pos.y,sprite.i,sprite.j);
    sprite.i += 1;
    sprite.i = sprite.i%6;
    double angle = atan2(speed.y, speed.x); // y, x
    double deg = angle * 180.0 / M_PI; // Conversion en degr
    if (deg < 0)
        deg += 360; // Pour avoir un angle entre 0 et 360
    if (deg >= 337.5 || deg < 22.5)
        sprite.j = 2;
    else if (deg >= 22.5 && deg < 67.5)
        sprite.j = 8;
    else if (deg >= 67.5 && deg < 112.5)
        sprite.j = 4;
    else if (deg >= 112.5 && deg < 157.5)
        sprite.j = 7;
    else if (deg >= 157.5 && deg < 202.5)
        sprite.j = 1;
    else if (deg >= 202.5 && deg < 247.5)
        sprite.j = 5;
    else if (deg >= 247.5 && deg < 292.5)
        sprite.j = 3;
    else
        sprite.j = 6;
}
void Slime::Move(){
    pos = pos +speed * dt;
}

void Slime::Accelerate(Vector a){
    speed = speed + a * dt;
}


Vector Slime::Launch(){
    cout << "Proceed to launch" << endl;
    int mouse_x = 0, mouse_y = 0;
    getMouse(mouse_x, mouse_y);
    Vector mouse = {static_cast<double>(mouse_x), static_cast<double>(mouse_y)};
    Vector delta = mouse - pos;
    Vector dir = delta / norm2(delta);
    double pulse = norm2(delta) * 5 / (static_cast<double>(WIDTH)); // Arbitrary formula
    Vector launch_vector = pulse * dir;
    return launch_vector;
}

void Slime::Lancer(){
    speed = Launch();
    for(int timeStep=0; timeStep<=250*freqDisplay; timeStep++) {

        //******** Display ************

        if ((timeStep%freqDisplay)==0){
            noRefreshBegin();
            clearWindow();
            Display();
            noRefreshEnd();
            milliSleep(75);
        }
        Move();
        Vector acc = Acceleration(speed);
        Accelerate(acc);
        if ((abs(speed.x) < 0.05) && (abs(speed.y) < 0.05)){
                break;
            }
    }
    cout << "End" << endl;
}

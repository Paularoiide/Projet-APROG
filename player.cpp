#include "player.h"
#include "physics.h"
#include "affichage.h"

Slime::Slime(Color givenColor, role_Slime givenRole, Vector givenPos) {// constructeur
    color = givenColor;
    pos = givenPos;
    role_Slime role = givenRole;
}

void Slime::Display(){

}
void Slime::Move(){
    pos = pos +speed * dt;
}

void Slime::Accelerate(Vector a){
    speed = speed + a * dt;
}


Vector Slime::Launch(){
    int mouse_x = 0, mouse_y = 0;
    getMouse(mouse_x, mouse_y);
    Vector mouse = {static_cast<double>(mouse_x), static_cast<double>(mouse_y)};
    Vector delta = mouse - pos;
    Vector dir = delta / norm2(delta);
    double pulse = norm2(delta) * 1.5 / (static_cast<double>(WIDTH)); // Arbitrary formula
    Vector launch_vector = pulse * dir;
    return launch_vector;
}

void Slime::Lancer(Vector cible){
    speed = cible;
    for(int timeStep=0; timeStep<=250*freqDisplay; timeStep++) {

        //******** Display ************

        if ((timeStep%freqDisplay)==0){
            noRefreshBegin();
            noRefreshEnd();
            milliSleep(50);
        }

        Move();
        Vector acc = Acceleration(speed);
        Accelerate(acc);
    }
}

#include "player.h"
#include "physics.h"
#include "affichage.h"

Slime::Slime(Color givenColor, role_Slime givenRole, Vector givenPos) {// constructeur
    color = givenColor;
    pos = givenPos;
    role_Slime role = givenRole;
}
void Slime::Move(){
    pos = pos +speed * dt;
}

void Slime::Accelerate(Vector a){
    speed = speed + a * dt;
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

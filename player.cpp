#include "player.h"
#include "physics.h"
#include "affichage.h"
#include "vector.h"

role_Slime roleFromStr(string s){
    if(s=="Joueur") {return role_Slime::JOUEUR;}
    if(s=="Ennemi") {return role_Slime::SLIME_ENEMY;}
    if(s=="PNJ") {return role_Slime::PNJ;}
    if(s=="Killer") {return role_Slime::KILLER;}
}

Slime::Slime(role_Slime givenRole, Vector givenPos) {// constructeur
    pos = givenPos;
    role = givenRole;
}
NiveauTextuel::~NiveauTextuel() =default;

struct DirectionRange {
    double minAngle;
    double maxAngle;
    int spriteJ;
};


void Slime::Display() {
    sprite.i = (sprite.i + 1) % 6; // Animation commune

    double angle = atan2(speed.y, speed.x);
    double deg = angle * 180.0 / M_PI;
    if (deg < 0)
        deg += 360;

    if (role == role_Slime::JOUEUR) {

        DirectionRange directions[9] = {
            {337.5, 360.0, 2},
            {0.0,   22.5,  2},
            {22.5,  67.5,  8},
            {67.5,  112.5, 4},
            {112.5, 157.5, 7},
            {157.5, 202.5, 1},
            {202.5, 247.5, 5},
            {247.5, 292.5, 3},
            {292.5, 337.5, 6}
        };
        // Mise à jour sprite.j selon direction exacte (8 directions)
        for (const auto& dir : directions) {
            if (deg >= dir.minAngle && deg < dir.maxAngle) {
                sprite.j = dir.spriteJ;
                break;
            }
        }
        putSprite(srcPath("slimebuddy.png"), pos.x, pos.y, sprite.i, sprite.j);
    }

    else if (role == role_Slime::SLIME_ENEMY) {
        // Droite, Haut, Gauche, Bas
        DirectionRange directions[5] = {
            {315.0, 360.0, 3}, // Droite
            {0.0,   45.0,  3}, // Droite
            {45.0,  135.0, 0}, // Haut
            {135.0, 225.0, 2}, // Gauche
            {225.0, 315.0, 1}  // Bas
        };

        for (const auto& dir : directions) {
            if (deg >= dir.minAngle && deg < dir.maxAngle) {
                sprite.j = dir.spriteJ;
                break;
            }
        }
        if (norm2(speed) > 0.5){
            putSprite(srcPath("Slime2_Run_full.png"), pos.x, pos.y, sprite.i, sprite.j, 64, 64);
        }
        else {
            putSprite(srcPath("Slime2_Walk_full.png"), pos.x, pos.y, sprite.i, sprite.j, 64, 64);
        }
    }
    else if (role == role_Slime::KILLER) {
        // Droite, Haut, Gauche, Bas
        DirectionRange directions[5] = {
            {315.0, 360.0, 3}, // Droite
            {0.0,   45.0,  3}, // Droite
            {45.0,  135.0, 0}, // Haut
            {135.0, 225.0, 2}, // Gauche
            {225.0, 315.0, 1}  // Bas
        };

        for (const auto& dir : directions) {
            if (deg >= dir.minAngle && deg < dir.maxAngle) {
                sprite.j = dir.spriteJ;
                break;
            }
        }
        putSprite(srcPath("Slime2_Attack_full.png"), pos.x, pos.y, sprite.i, sprite.j, 64, 64);
    }
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
    Vector dir = delta / sqrt(norm2(delta));
    double pulse = sqrt(norm2(delta)) * 8 / (static_cast<double>(WIDTH)); // Arbitrary formula
    Vector launch_vector = pulse * dir;
    return launch_vector;
}

void Slime::Shock(Collisionable *obstacle) {
    Vector origin = projection(pos, obstacle->Point1, obstacle->Point2);
    Vector d = pos - origin;
    Vector normal = d / sqrt(norm2(d)); // Vecteur normal unitaire

    // Réflexion de la vitesse
    speed = reflect(speed, normal);
}

bool Slime::Collision(Collisionable *obstacle) {
    Vector AB = obstacle->Point2 - obstacle->Point1;
    Vector AP = pos - obstacle->Point1;
    double t = ps(AP, AB) / norm2(AB); // t ∈ R

    // Si la projection tombe hors du segment, pas de collision
    if (t< 0 || t > 1) return false;

    // Point projeté sur le segment
    Vector origin = obstacle->Point1 + t * AB;
    Vector d = pos - origin;
    Vector normal = d / sqrt(norm2(d));

    if (norm2(speed) != 0) {
        double t_min = -ps(speed, normal) / norm2(speed);
        double t_true;
        if (t_min < 0)
            t_true = 0;
        else if (t_min > dt)
            t_true = dt;
        else
            t_true = t_min;
        d = d + speed * t_true;
    }

    return (sqrt(norm2(d)) <= radius);
}





Vector Slime::Launch2(){
    Vector center = {50.,50.}; // Centre de l'interface de direction
    Vector arrow = {50.,40.}; // Position initiale de la flèche

    do {drawArrow(center.x,center.y,arrow.x,arrow.y,BLUE);
        arrow = rotate(arrow - center,1) + center;
    } while (getKey() != KEY_PAUSE);
    Vector dir = arrow - center / norm2(arrow - center); // We now have the launch direction
    Vector top_left = {50.,50.};
    arrow.x = 50.;
    arrow.y = 50.; // Position initiale de la flèche
    int w = 50,
        h = 5,
        alpha = 1;
    fillRect(top_left.x,top_left.y,w,h,BLACK); // Rectangle
    do {drawArrow(arrow.x,arrow.y,arrow.x,arrow.y+5,BLUE);
        arrow.x+= alpha;
        if ((arrow.x == top_left.x + w) || (arrow.x == top_left.x)){
            alpha *= -1;
        }

    } while (getKey() != KEY_PAUSE);
    return {0,0};

}

void Slime::Die(){
    speed = {0,0};
    cout<<"Je suis mort"<< endl;
}

void Slime::Win(){
    cout<<"I won"<< endl;
}

void Slime::Lancer(Vector pulse, vector<unique_ptr<Element>>& obstacles, Background background){
    speed = pulse;
    for(int timeStep=0; timeStep<=250*freqDisplay; timeStep++) {
        //******** Display ************

        if ((timeStep%freqDisplay)==0){
            noRefreshBegin();
            Resetscreen(obstacles,background);
            Display();
            noRefreshEnd();
            milliSleep(75);
        }
        for (int i = 0; i < obstacles.size(); i++) {
            if (Collisionable* d = dynamic_cast<Collisionable*>(obstacles[i].get())) {
                if (Collision(d)) {
                    if (Porte* p = dynamic_cast<Porte*>(obstacles[i].get())) {
                        Win();
                    }
                    else {
                        Shock(d);
                    }
                }
            }
        }
        Move();
        Vector acc = Acceleration(speed);// variation de vitesse à l'instant t
        Accelerate(acc);// mise à joue de la vitesse avec l'acceleration
        if (norm2(speed)<= 0.005){
            break;
        }
    }
    cout << "Waiting for a new pulse" << endl;
}

void Slime::Check(Slime slime, vector<unique_ptr<Element>>& obstacles){
    DirectionRange directions[4] = {
        {315.0, 45.0, 3}, // Droite
        {45.0,  135.0, 0}, // Haut
        {135.0, 225.0, 2}, // Gauche
        {225.0, 315.0, 1}  // Bas
    };
    for (const auto& dir : directions) {
        if (sprite.j == dir.spriteJ) {
            Vector dif = slime.pos - pos;
            double angle = atan2(dif.y, dif.x);
            double deg = angle * 180.0 / M_PI;
            if (deg < 0)
                deg += 360;
            if (deg <= dir.maxAngle && deg >= dir.minAngle){
                bool kill = true;
                /*Vector v;
                for (int i =0; i<100; i++){
                    v = pos + dif * i/100;
                    for (int i = 0; i < obstacles.size(); i++) {
                        if (Collisionable* d = dynamic_cast<Collisionable*>(obstacles[i].get())) { // Vérification si collisionable
                            if (v in d) {
                                kill = false }}}
                }*/
                if (kill){
                    KILL(slime,obstacles);
                }
            }
            break;
        }
    }
}

void Slime::KILL(Slime slime, vector<unique_ptr<Element>>& obstacles){
    role = role_Slime::KILLER;
    Vector dif = slime.pos - pos;
    //Lancer(dif*2,obstacles);
}

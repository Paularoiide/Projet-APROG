#include "player.h"
#include "physics.h"
#include "affichage.h"
#include "vector.h"
#include "niveaux.h"

/*role_Slime roleFromStr(string s){
    if(s=="Joueur") {return role_Slime::JOUEUR;}
    if(s=="Ennemi") {return role_Slime::SLIME_ENEMY;}
    if(s=="PNJ") {return role_Slime::PNJ;}
    if(s=="Killer") {return role_Slime::KILLER;}
}*/

Slime::Slime(role_Slime givenRole, Vector givenPos) {// constructeur
    pos = givenPos;
    pos_initial = givenPos;
    role = givenRole;
    if (role == role_Slime::SLIME_ENEMY) {
        speed = {0.5, 0.0}; // vitesse de départ vers la droite
    } if (role == role_Slime::JOUEUR) {
        speed = {0.0, 0.0};
    }
    sprite = {0,0};
    radius = 6;
}

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
        putSprite(stringSrcPath(strAssets+"slimebuddy.png"), pos.x, pos.y, sprite.i, sprite.j);
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
            putSprite(stringSrcPath(strAssets+"Slime2_Run_full.png"), pos.x, pos.y, sprite.i, sprite.j, 64, 64);
        }
        else {
            putSprite(stringSrcPath(strAssets+"Slime2_Walk_full.png"), pos.x, pos.y, sprite.i, sprite.j, 64, 64);
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
        putSprite(stringSrcPath(strAssets+"Slime2_Attack_full.png"), pos.x, pos.y, sprite.i, sprite.j, 64, 64);
    }
}

void Slime::Move(){
    if (role == role_Slime::JOUEUR){
        pos = pos +speed * dt;}
    if (role == role_Slime::SLIME_ENEMY){
        // Déplacement automatique autour de la position initiale sur l'axe x
        double amplitude = 50; // Distance maximale à gauche ou à droite
        double delta_x = pos.x - pos_initial.x;
        // Inverser la vitesse si on dépasse l'amplitude
        if (abs(delta_x) > amplitude) {
            speed = -1 * speed;
        }
        pos.x += speed.x * dt;
    }
    if (role == role_Slime::KILLER){
        pos = pos +speed * dt;}
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
    double pulse = sqrt(norm2(delta)) * 6 / (static_cast<double>(WIDTH)); // Arbitrary formula
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

bool Slime::CollisionSlime(const Slime& other) {
    // Calcul de la distance entre les centres des deux slimes
    Vector diff = pos - other.pos;
    double distanceSquared = norm2(diff); // Utilise norm2 (distance au carré pour l'optimisation)

    // Somme des rayons au carré
    double radiusSum = radius + other.radius;
    double radiusSumSquared = radiusSum * radiusSum;

    // Collision si distance² <= (rayon1 + rayon2)²
    return distanceSquared <= radiusSumSquared;
}



void Slime::Check(Slime& slime, vector<unique_ptr<Element>>& obstacles) {
    if (role == role_Slime::SLIME_ENEMY) {
        DirectionRange directions[4] = {
            {350.0, 10.0, 3}, // Droite
            {80.0, 100.0, 0}, // Haut
            {170.0, 190.0, 2}, // Gauche
            {260.0, 280.0, 1}  // Bas
        };

        for (const auto& dir : directions) {
            if (sprite.j == dir.spriteJ) {
                Vector dif = slime.pos - pos;
                double distance = sqrt(norm2(dif));
                double angle = atan2(dif.y, dif.x) * 180.0 / M_PI;
                if (angle < 0) angle += 360;

                bool inAngleRange =
                    (dir.minAngle < dir.maxAngle && angle >= dir.minAngle && angle <= dir.maxAngle) ||
                    (dir.minAngle > dir.maxAngle && (angle >= dir.minAngle || angle <= dir.maxAngle));

                if (inAngleRange && distance < 200.0) {  // Vérification de distance commune
                    cout << "[DEBUG] Joueur détecté dans l'angle de vision, déclenchement de KILL" << endl;
                            role = role_Slime::KILLER;
                }
                break;
            }
        }
    }
}




#include "player.h"
#include "physics.h"
#include "affichage.h"
#include "vector.h"
#include "niveaux.h"

Joueur::Joueur(Vector givenPos) {//Initialise un joueur avec une position donnée, une vitesse nulle, un rayon de 6 et un sprite par défaut.
    pos = givenPos;
    pos_initial = givenPos;
    speed = {0.0, 0.0};
    sprite = {0,0};
    radius = 6;
}

Ennemi::Ennemi(Vector givenPos) {//Initialise un ennemi avec une position donnée, une vitesse initiale vers la droite, un rayon de 6 et un état kill à false.
    pos = givenPos;
    pos_initial = givenPos;
    speed = {0.5, 0.0};
    sprite = {0,0};
    radius = 6;
    kill = false;
}


Ennemi::Ennemi(Vector givenPos, Vector *movePattern, int n) { // Initialise un ennemi avec un motif de déplacement (movePattern) en plus des attributs de base.
    pos = givenPos;
    pos_initial = givenPos;
    speed = {0.5, 0.0}; // vitesse de départ vers la droite
    sprite = {0,0};
    radius = 6;
    kill = false;
    pattern = new Vector[n];
    for(int i=0;i<n;i++) {
        pattern[i] = movePattern[i];
    }
}


struct DirectionRange {
    double minAngle;
    double maxAngle;
    int spriteJ;
};

void Joueur::Display(){ //Anime le sprite du joueur et met à jour sa direction en fonction de son angle de déplacement. Affiche le sprite correspondant.
    sprite.i = (sprite.i + 1) % 6; // Animation commune

    double angle = atan2(speed.y, speed.x);
    double deg = angle * 180.0 / M_PI;
    if (deg < 0)
        deg += 360;

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

void Ennemi::Display(){ // Anime le sprite de l'ennemi et met à jour sa direction. Affiche différents sprites selon si l'ennemi est en train de marcher, courir ou attaquer (kill = true).
    sprite.i = (sprite.i + 1) % 6; // Animation commune

    double angle = atan2(speed.y, speed.x);
    double deg = angle * 180.0 / M_PI;
    if (deg < 0)
        deg += 360;
    if (!kill) {
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
    else {
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


void Joueur::Move(){ // Déplace le joueur en fonction de sa vitesse et du temps écoulé (dt)
    pos = pos +speed * dt;}

void Ennemi::Move(){ // Déplace l'ennemi automatiquement autour de sa position initiale (va-et-vient horizontal). Si kill est vrai, l'ennemi se déplace normalement.
    if (!kill){
        // Déplacement automatique autour de la position initiale sur l'axe x
        double amplitude = 50; // Distance maximale à gauche ou à droite
        double delta_x = pos.x - pos_initial.x;
        // Inverser la vitesse si on dépasse l'amplitude
        if (abs(delta_x) > amplitude) {
            speed = -1 * speed;
        }
        pos.x += speed.x * dt;
    }
    else {
        pos = pos +speed * dt;}
}

void Slime::Accelerate(Vector a){ // Applique une accélération a à la vitesse du slime.
    speed = speed + a * dt;
}

Vector Joueur::Launch() const{ // Calcule et retourne un vecteur de lancement basé sur la position de la souris pour propulser le joueur.
    int mouse_x = 0, mouse_y = 0;
    getMouse(mouse_x, mouse_y);
    Vector mouse = {static_cast<double>(mouse_x), static_cast<double>(mouse_y)};
    Vector delta = mouse - pos;
    Vector dir = delta / sqrt(norm2(delta));
    double pulse = sqrt(norm2(delta)) * 6 / (static_cast<double>(WIDTH)); // Arbitrary formula
    Vector launch_vector = pulse * dir;
    return launch_vector;
}

void Slime::Shock(Collisionable *obstacle) { // Gère la réflexion de la vitesse du slime lors d'une collision avec un obstacle en utilisant la normale à la surface.
    Vector origin = projection(pos, obstacle->Point1, obstacle->Point2);
    Vector d = pos - origin;
    Vector normal = d / sqrt(norm2(d)); // Vecteur normal unitaire

    // Réflexion de la vitesse
    speed = reflect(speed, normal);
}

bool Slime::Collision(Collisionable *obstacle) const { // Vérifie si le slime entre en collision avec un obstacle en projetant sa position sur le segment de l'obstacle.
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

bool Joueur::CollisionSlime(const Ennemi& other) { // Vérifie si l'ennemi entre en collision avec le joueur en comparant la distance entre eux à la somme de leurs rayons.
    // Calcul de la distance entre les centres des deux slimes
    double diff = distance(pos,other.pos);

    // Somme des rayons au carré
    double Som_radius = radius + other.radius;

    // Collision si distance² <= (rayon1 + rayon2)²
    return diff <= Som_radius;
}



void Ennemi::Check(Joueur& joueur, vector<unique_ptr<Element>>& obstacles) { //Détecte si le joueur est dans le champ de vision de l'ennemi (angle et distance). Si oui, active le mode attaque (kill = true).
    DirectionRange directions[4] = {
        {350.0, 10.0, 3}, // Droite
        {80.0, 100.0, 0}, // Haut
        {170.0, 190.0, 2}, // Gauche
        {260.0, 280.0, 1}  // Bas
    };

    for (const auto& dir : directions) {
        if (sprite.j == dir.spriteJ) {
            Vector dif = joueur.pos - pos;
            double distance = sqrt(norm2(dif));
            double angle = atan2(dif.y, dif.x) * 180.0 / M_PI;
            if (angle < 0) angle += 360;

            bool inAngleRange =
                (dir.minAngle < dir.maxAngle && angle >= dir.minAngle && angle <= dir.maxAngle) ||
                (dir.minAngle > dir.maxAngle && (angle >= dir.minAngle || angle <= dir.maxAngle));

            if (inAngleRange && distance < 200.0) {  // Vérification de distance commune
                        kill = true;
            }
            break;
        }
    }

}




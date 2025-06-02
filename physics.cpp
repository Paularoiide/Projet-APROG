#include "physics.h"


Vector Acceleration(Vector v, bool frein, double friction_coeff, double friction_coeff2){ // Calcul une accélération en prenant en compte différents types de frottements
    Vector acc = {0.,0.};

    //ajout du frein
    if(frein) {
        Vector freinage = -2*friction_coeff * v;
        Vector freinage2 = -friction_coeff2*v/sqrt(norm2(v));
        acc =acc + freinage+freinage2;
    }

    // ajout de la friction
    Vector friction = -friction_coeff * v;
    acc =acc + friction;

    // ajout de la friction constante
    Vector friction2 = -friction_coeff2*v/sqrt(norm2(v));
    acc = acc + friction2;

    return acc;
}

// Detecte les inputs au clavier
int keyboard() {
    Event e;
    do {
        getEvent(0,e);
        if(e.type==EVT_KEY_ON)
            return e.key;
    } while(e.type!=EVT_NONE);
    return 0;
}

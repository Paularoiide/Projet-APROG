#include "niveaux.h"

Mur::Mur(Vector PointA, Vector PointB) {
    Point1 = PointA;
    Point2 = PointB;
}

NiveauTextuel::NiveauTextuel(int nbElem1) {
    nbElem = nbElem1;
    lignes = new string[nbElem1];
}

NiveauTextuel::~NiveauTextuel() {
    delete[] lignes;
}

NiveauTextuel ouvrir_niveau(string nom_fichier) {
    ifstream f(nom_fichier);

    if (!f.is_open()) {
        cerr << "Erreur à l'ouverture du fichier !" << endl;
        return NiveauTextuel(0); // On retourne un objet vide
    }

    // Compter les lignes
    int nbLignes = count(istreambuf_iterator<char>(f),
                         istreambuf_iterator<char>(), '\n');

    // Remettre le curseur au début du fichier
    f.clear();
    f.seekg(0);

    NiveauTextuel niveauActuel(nbLignes);

    for (int i = 0; i < nbLignes; i++) {
        getline(f, niveauActuel.lignes[i]);
    }

    cout << "Niveau chargé" << endl;

    f.close();
    return niveauActuel;
}

void NiveauTextuel::detruire() {
    delete[] lignes;
    lignes = nullptr;
}

Niveau::~Niveau() {
    for (Element* obj : elements) {
        delete obj;
    }
}

void Niveau::ajouterElement(Element* obj) {
    elements.push_back(obj);
}

void Niveau::afficher() {
    for (Element* obj : elements) {
        obj->afficher();
    }
}

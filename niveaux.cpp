#include "niveaux.h"

Niveau::Niveau(int nbElem1) {
    nbElem = nbElem1;
    lignes = new string[nbElem1];
}

Niveau::~Niveau() {
    delete[] lignes;
}

Niveau ouvrir_niveau(string nom_fichier) {
    ifstream f(nom_fichier);

    if (!f.is_open()) {
        cerr << "Erreur à l'ouverture du fichier !" << endl;
        return Niveau(0); // On retourne un objet vide
    }

    // Compter les lignes
    int nbLignes = count(istreambuf_iterator<char>(f),
                         istreambuf_iterator<char>(), '\n');

    // Remettre le curseur au début du fichier
    f.clear();
    f.seekg(0);

    Niveau niveauActuel(nbLignes);

    for (int i = 0; i < nbLignes; i++) {
        getline(f, niveauActuel.lignes[i]);
    }

    cout << "Niveau chargé" << endl;

    f.close();
    return niveauActuel;
}

void Niveau::detruire() {
    delete[] lignes;
    lignes = nullptr;
}

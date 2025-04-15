#include "niveaux.h"

Niveau ouvrir_niveau(string nom_fichier) {

    // Open the text file named "input.txt"
    ifstream f(nom_fichier);


    // Check if the file is successfully opened
    if (!f.is_open()) {
        cerr << "Error opening the file!";
        return 1;
    }

    int nbLignes;
    nbLignes = count(istreambuf_iterator<char>(f),
                     istreambuf_iterator<char>(), '\n');

    Niveau niveauActuel(nbLignes);

    // String variable to store the read data
    string s;

    // Read each line of the file and print it to the
    // standard output stream till the whole file is
    // completely read
    for(int i=0;i<nbLignes;i++) {
        niveauActuel.lignes[i] ="";
        getline(f, niveauActuel.lignes[i]);
    }

    cout << "niveau chargé" << endl;

    f.close();
    return niveauActuel;
}

void Niveau::detruire(){
    delete[] lignes;
}

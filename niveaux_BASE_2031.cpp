#include "niveaux.h"
#include <Imagine/Graphics.h>
using namespace Imagine;


Mur::Mur(Vector PointA, Vector PointB, int epais) {
    Point1 = PointA;
    Point2 = PointB;
    epaisseur = epais;
}
void Mur::afficher(){
    cout << "affichage Mur" << endl;

    drawLine(Point1.x,Point1.y,Point2.x,Point2.y,BLACK,epaisseur);
}

Bordure::Bordure(Vector PointA, Vector PointB) {
    Point1 = PointA;
    Point2 = PointB;
}
void Bordure::afficher(){
    cout << "affichage Bordure" << endl;
    Vector coins[4] = {{0.,0.},{WIDTH,0.},{WIDTH,HEIGHT},{0.,HEIGHT}};
    double distances[8] = {distance(coins[0],Point1),distance(coins[0],Point2),
                         distance(coins[1],Point1),distance(coins[1],Point2),
                         distance(coins[2],Point1),distance(coins[2],Point2),
                           distance(coins[3],Point1),distance(coins[3],Point2)};
    double max=0;
    for(int i=0;i<8;i++) {
        if(max<distances[i]) {
            max = distances[i];
        }
    }
    int epaisseurNecessaire = max;
    Vector Point3 = Point2 + rotate((Point2-Point1)*epaisseurNecessaire/distance(Point2,Point1),90);
    Vector Point4 = Point3 + Point1 - Point2;
    int lesX[4] = {Point1.x,Point2.x,Point3.x,Point4.x};
    int lesY[4] = {Point1.y,Point2.y,Point3.y,Point4.y};
    fillPoly(lesX,lesY,4,BLACK);
}

Pique::Pique(Vector Base1, Vector Sommet1, int largeur1) {
    Base = Base1;
    Sommet=Sommet1;
    largeur=largeur1;
}


NiveauTextuel::NiveauTextuel(int nbElem1) {
    lignes.resize(nbElem1);  // plus besoin de gestion manuelle de mémoire
}
NiveauTextuel::NiveauTextuel() =default;
NiveauTextuel::~NiveauTextuel() =default;

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
    lignes.clear();
}

Niveau::~Niveau() {
    for (Element* obj : elements) {
        delete obj;
    }
}

void Niveau::ajouterElement(Element* obj) {
    elements.push_back(obj);
}
void Niveau::remplir_niveau(NiveauTextuel texte) {
    for (size_t i = 0; i < texte.lignes.size(); ++i) {
        string ligne = texte.lignes[i];
        stringstream s(ligne);
        vector<string> row;
        row.clear();
        string word="";
        while (getline(s, word, ','))
        {
            regex regexp(":\\d+");// On cherche le nombre après le ":"
            smatch m;
            if (regex_search(word, m, regexp)) {
                row.push_back(m[0].str().substr(1)); // extrait le nombre sans le ":"
            }
        }
        if(row[0]=="Bordure") {
            Vector Point1 = {stoi(row[1]),stoi(row[2])};
            Vector Point2 = {stoi(row[3]),stoi(row[4])};
            Element* bord = new Bordure(Point1, Point2);
            elements.push_back(bord);
        } else if (row[0]=="Mur") {
            Vector Point1 = {stoi(row[1]),stoi(row[2])};
            Vector Point2 = {stoi(row[3]),stoi(row[4])};
            int epaiss = stoi(row[4]);
            Element* mur = new Mur(Point1,Point2,epaiss);
            elements.push_back(mur);
        } else {
            cerr << "erreur de lecture : impossible d'identifier l'élément de type '"<<
                row[0]<<"' à la ligne "<<ligne<< ", n° : "<<i<<endl;
        }
    }
}
void Niveau::afficher() {
    for (Element* obj : elements) {
        obj->afficher();
    }
}

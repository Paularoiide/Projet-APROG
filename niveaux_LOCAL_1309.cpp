#include "niveaux.h"
#include "player.h"
#include <Imagine/Graphics.h>
using namespace Imagine;


Mur::Mur(Vector PointA, Vector PointB, int epais) {
    Point1 = PointA;
    Point2 = PointB;
    epaisseur = epais;
}
void Mur::afficher() {
    drawLine(Point1.x,Point1.y,Point2.x,Point2.y,BLACK,epaisseur);
}
bool Mur::is_in(Vector v) {
    Vector proj = projection(v, Point1, Point2);
    return norm2(v - proj) < epaisseur;
}

Bordure::Bordure(Vector PointA, Vector PointB) {
    Point1 = PointA;
    Point2 = PointB;
}
void Bordure::afficher(){
    /*Vector coins[4] = {{0.,0.},{static_cast<double>(WIDTH),0.},{static_cast<double>(WIDTH),static_cast<double>(HEIGHT)},{0.,static_cast<double>(HEIGHT)}};
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
    Vector Point4 = Point3 + Point1 - Point2;*/


    Vector Point3 = intersection_bords(Point2,fmod((angle_horiz(Point2-Point1)+90.),360.));
    Vector Point4 = intersection_bords(Point1,fmod((angle_horiz(Point2-Point1)+90.),360.));
    int lesX[4] = {static_cast<int>(Point1.x), static_cast<int>(Point2.x), static_cast<int>(Point3.x), static_cast<int>(Point4.x)};
    int lesY[4] = {static_cast<int>(Point1.y), static_cast<int>(Point2.y), static_cast<int>(Point3.y), static_cast<int>(Point4.y)};
    fillPoly(lesX,lesY,4,BLUE);
}
bool Bordure::is_in(Vector v) {
    return false;
}

Porte::Porte(Vector PointA, Vector PointB, int epais) {
    Point1 = PointA;
    Point2 = PointB;
    epaisseur = epais;
}
void Porte::afficher() {
    cout << " Porte" << endl;

}

Pique::Pique(Vector Base1, Vector Sommet1, int largeur1) {
    Base = Base1;
    Sommet=Sommet1;
    largeur=largeur1;
    Vector dir = rotate(Sommet1-Base1,90)/norm2(Sommet1-Base1);
    Sommets[0] = Sommet;
    Sommets[1] = Base - (largeur / 2) * dir;
    Sommets[2] = Base + (largeur / 2) * dir;
}
bool Pique::Collision(Slime *slime){
    for (int i =0; i < 3; i++){
        if (norm2(slime->pos - Sommets[i]) < slime->radius){
            return true;
        }
    }
    return false;
}


NiveauTextuel::NiveauTextuel(int nbElem1) {
    lignes.resize(nbElem1);  // plus besoin de gestion manuelle de mémoire
}
NiveauTextuel::NiveauTextuel() =default;
NiveauTextuel::~NiveauTextuel() =default;
NiveauTextuel ouvrir_niveau(string nom_fichier) {
    cout << "ouverture du fichier niveau : "<< nom_fichier << endl;
    ifstream f(nom_fichier);
    if (!f.is_open()) {
        cerr << "Erreur à l'ouverture du fichier !" << endl;
            return NiveauTextuel(0); // On retourne un objet vide
    }

    // Compter les lignes avec espaces et commentaires:
    int nbLignes = count(istreambuf_iterator<char>(f),
                         istreambuf_iterator<char>(), '\n')+1;
    f.clear();
    f.seekg(0);

    // compter sans lignes vides ou lignes avec # (commentaire)
    int nbLignesNonVides=0;
    string ligne;
    while(getline(f, ligne))
    {
        ligne.erase(0, ligne.find_first_not_of(" \t\r\n"));
        // Ignore ligne vide ou ligne de commentaire
        if (ligne.empty() || ligne[0] == '#' || (ligne.size() >= 2 && ligne[0] == '/' && ligne[1] == '/')|| (ligne.size() >= 2 && ligne[0] == '\\' && ligne[1] == 'n'))
            continue;

        nbLignesNonVides++;
    }
    // convention locale : on ne met pas de ligne vide à la fin
    cout << "nombre de lignes :"<<nbLignes<<endl;
    cout << "nombre de lignes non vides:"<<nbLignesNonVides<<endl;
    // Remettre le curseur au début du fichier
    f.clear();
    f.seekg(0);

    NiveauTextuel niveauActuel(nbLignesNonVides);
    string ligneActu = "";
    int j = 0;

    for (int i = 0; i < nbLignes; i++) {
        getline(f, ligneActu);
        ligne.erase(0, ligneActu.find_first_not_of(" \t\r\n"));
        // Ignore ligne vide ou ligne de commentaire
        if (ligneActu.empty() || ligneActu[0] == '#' || (ligneActu.size() >= 2 && ligneActu[0] == '/' && ligneActu[1] == '/')|| (ligneActu.size() >= 2 && ligneActu[0] == '\\' && ligneActu[1] == 'n'))
        {
            cout << "ligne n°"<< i << " vide." << endl;
            continue;
        } else {
            niveauActuel.lignes[j] = ligneActu;
            j++;}
    }

    cout << "Niveau chargé" << endl;
    f.close();
    return niveauActuel;
}
void NiveauTextuel::detruire() {
    lignes.clear();
}



void Niveau::ajouterElement(unique_ptr<Element> obj) {
    elements.push_back(move(obj));
}
void Niveau::remplir_niveau(NiveauTextuel texte) {
    for (size_t i = 1; i < texte.lignes.size(); i++) {
        string ligne = texte.lignes[i];
        stringstream s(ligne);
        vector<string> row;
        row.clear();

        regex pattern(R"(^([^,]+))");
        smatch match;
        if (std::regex_search(ligne, match, pattern) && match.size() > 1) {
            row.push_back(match[1].str());
        } else {
            cerr << "ligne mal définie : le type d'élément (avant la première virgule) n'est pas défini !" << endl;
                cout << "ligne : " << ligne << endl;
                continue;
        }

        string word;
        while (getline(s, word, ',')) {
            regex regexp(":([a-zA-Z0-9]+)");
            smatch m;
            if (regex_search(word, m, regexp)) {
                row.push_back(m[0].str().substr(1));
            }
        }

        string row_explicite = "";
        for (string mot : row) {
            row_explicite = row_explicite + "|" + mot;
        }
        cout << "row : " << row_explicite << endl;

        if (row[0] == "Bordure") {
            if (row.size() < 5) {
                cerr << "erreur de lecture : pas assez d'arguments pour l'élément de type '" << row[0] << "' à la ligne " << ligne << ", n° : " << i << endl;
                continue;
            }
            //cout << "lecture d'une bordure. Arguments : " << row[1] << ", " << row[2] << "," << row[3] << ", " << row[4] << endl;
            Vector Point1 = {static_cast<double>(stoi(row[1])), static_cast<double>(stoi(row[2]))};
            Vector Point2 = {static_cast<double>(stoi(row[3])), static_cast<double>(stoi(row[4]))};
            auto bord = std::make_unique<Bordure>(Point1, Point2);
            elements.push_back(std::move(bord));
        } else if (row[0] == "Mur") {
            if (row.size() < 6) {
                cerr << "erreur de lecture : pas assez d'arguments pour le mur à la ligne " << ligne << ", n° : " << i << endl;
                continue;
            }
            //cout << "lecture d'un mur. Arguments : " << row[1] << ", " << row[2] << "," << row[3] << ", " << row[4] << ", " << row[5] << endl;
            Vector Point1 = {static_cast<double>(stoi(row[1])), static_cast<double>(stoi(row[2]))};
            Vector Point2 = {static_cast<double>(stoi(row[3])), static_cast<double>(stoi(row[4]))};
            int epaiss = stoi(row[5]);
            //cout << "epaisseur du mur : " << epaiss << endl;
            auto mur = std::make_unique<Mur>(Point1, Point2, epaiss);
            elements.push_back(std::move(mur));
        }else if (row[0]=="Slime") {
            if (row.size() < 4) {
                cerr << "erreur de lecture : pas assez d'arguments pour l'élément de type '" << row[0] << "' à la ligne " << ligne << ", n° : " << i << endl;
                        cout << "nb elements : " << row.size();
                    continue;
            }
            role_Slime role = roleFromStr(row[1]);
            Vector pos = {static_cast<double>(stoi(row[2])), static_cast<double>(stoi(row[3]))};

            if(row.size() == 4) {
                auto slime = std::make_unique<Slime>(role,pos);
                elements.push_back(std::move(slime));
            } else {
                int nbOfPatterns = (row.size()-4)/2;
                Vector *pattern = new Vector[nbOfPatterns];
                for(int j=0;j<nbOfPatterns;j++) {
                    pattern[j].x = static_cast<double>(stoi(row[4+2*j]));
                    pattern[j].y = static_cast<double>(stoi(row[5+2*j]));
                }
                auto slime = std::make_unique<Slime>(role,pos,pattern,nbOfPatterns);
                elements.push_back(std::move(slime));
            }

        }else {
            cerr << "erreur de lecture : impossible d'identifier l'élément de type '" << row[0] << "' à la ligne " << ligne << ", n° : " << i << endl;
        }
        cout << "element lu." << endl;
    }
    cout << "fin de construction du niveau" << endl;
    cout << "analyse du niveau :" << endl;
    for (int i = 0; i < elements.size(); i++) {
        cout << "Type de l'objet : " << typeid(*elements[i]).name() << endl;
    }
}

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

void Niveau::afficher() {
    for (auto& obj : elements) { // Utilisez une référence pour éviter de copier le unique_ptr
        /*cout << "affichage d'un élément" << endl;
                cout << "Test avec typeid(*obj).name(). Type de l'objet : " << typeid(*obj).name() << endl;
*/
        // Obtenez le pointeur brut pour utiliser dynamic_cast
        Element* rawPtr = obj.get();

        /*if (dynamic_cast<Mur*>(rawPtr)) {
            cout << "2e test. C'est un Mur" << endl;
        } else {
            cout << "deuxième test. Pas un Mur" << endl;
        }

        if (dynamic_cast<Bordure*>(rawPtr)) {
            cout << "2e test. C'est une Bordure" << endl;
        } else {
            cout << "deuxième test. Pas une Bordure" << endl;
        }

        if (dynamic_cast<Collisionable*>(rawPtr)) {
            cout << "element est de type Collisionable" << endl;
        } else {
            cout << "element n'est pas de type Collisionable" << endl;
        }*/

        if (rawPtr) { // Vérifiez que le pointeur n'est pas nul
            rawPtr->afficher();
        } else {
            cerr << "objet non construit au moment de l'affichage" << endl;
        }
    }

}

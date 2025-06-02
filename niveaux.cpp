#include "niveaux.h"
#include "player.h"
#include <Imagine/Graphics.h>
using namespace Imagine;


Mur::Mur(Vector PointA, Vector PointB, int epais) {
    Point1 = PointA;
    Point2 = PointB;
    epaisseur = epais;
}

// la fonction d'affichage ne sert pas dans le jeu final :
// elle sert pour le déboggage/ l'analyse de niveaux
// (par exemple, vérifier qu'il n'y a pas de mur manquant)
void Mur::afficher() {
    drawLine(Point1.x,Point1.y,Point2.x,Point2.y,BLACK,epaisseur);
}

// vérifie si une trajectoire rentre dans un mur (afin de déclencher le rebond)
bool Mur::is_in(Vector v) {
    Vector proj = projection(v, Point1, Point2);
    return norm2(v - proj) < epaisseur;
}

Porte::Porte(Vector PointA, Vector PointB, int epais) {
    Point1 = PointA;
    Point2 = PointB;
    epaisseur = epais;
}

bool Porte::is_in(Vector v) {
    Vector proj = projection(v, Point1, Point2);
    return norm2(v - proj) < epaisseur;
}

void Porte::afficher() {
    drawLine(Point1.x,Point1.y,Point2.x,Point2.y,BLUE,epaisseur);
    cout << " Porte" << endl;
}




NiveauTextuel::NiveauTextuel(int nbElem1) {
    lignes.resize(nbElem1);
}

NiveauTextuel::NiveauTextuel() =default;
NiveauTextuel::~NiveauTextuel() =default; // destructeur par défaut

// "lit" un fichier texte (.txt) en stockant chaque ligne dans un tableau de string
// saute les lignes vide ou commentées à l'aide d'un # ou //
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
    //cout << "nombre de lignes :"<<nbLignes<<endl;
    //cout << "nombre de lignes non vides:"<<nbLignesNonVides<<endl;
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
            continue;
        } else {
            niveauActuel.lignes[j] = ligneActu;
            j++;}
    }

    //cout << "Niveau chargé" << endl;
    f.close();
    return niveauActuel;
}


// ajoute un objet de type élémeent à la liste elements d'un Niveau
void Niveau::ajouterElement(unique_ptr<Element> obj) {
    elements.push_back(move(obj));
}

// crée une liste d'objets de type Mur, Slime, Porte... (descendant l'Element) à partir d'un
// NiveauTextuel. Pour cela, il lit avec Regex les attributs suivant les ':', qui sont ordonnés
// (Dans la syntaxe "clé":valeur, "clé" est à titre indicatif pour l'écriture du fichier .txt
// dont est issu le NiveauTextuel, et n'est donc jamais lue)
// c'est le premier mot qui définit le type d'objet associé à chaque ligne
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
        //cout << "row : " << row_explicite << endl;

        if (row[0] == "Mur") {
            if (row.size() < 6) {
                cerr << "erreur de lecture : pas assez d'arguments pour le mur à la ligne " << ligne << ", n° : " << i << endl;
                    continue;
            }
            //cout << "lecture d'un mur. Arguments : " << row[1] << ", " << row[2] << "," << row[3] << ", " << row[4] << ", " << row[5] << endl;
            Vector Point1 = {static_cast<double>(stoi(row[1]))+ decalage_x, static_cast<double>(stoi(row[2])) + decalage_y};
            Vector Point2 = {static_cast<double>(stoi(row[3])) + decalage_x, static_cast<double>(stoi(row[4])) + decalage_y};
            int epaiss = stoi(row[5]);
            //cout << "epaisseur du mur : " << epaiss << endl;
            auto mur = std::make_unique<Mur>(Point1, Point2, epaiss);
            elements.push_back(std::move(mur));
        } else if (row[0] == "Porte") {
            if (row.size() < 6) {
                cerr << "erreur de lecture : pas assez d'arguments pour la porte à la ligne " << ligne << ", n° : " << i << endl;
                    continue;
            }
            //cout << "lecture d'une porte. Arguments : " << row[1] << ", " << row[2] << "," << row[3] << ", " << row[4] << ", " << row[5] << endl;
            Vector Point1 = {static_cast<double>(stoi(row[1]))+ decalage_x, static_cast<double>(stoi(row[2])) + decalage_y};
            Vector Point2 = {static_cast<double>(stoi(row[3])) + decalage_x, static_cast<double>(stoi(row[4])) + decalage_y};
            int epaiss = stoi(row[5]);
            //cout << "epaisseur de la porte : " << epaiss << endl;
            auto porte = std::make_unique<Porte>(Point1, Point2, epaiss);
            elements.push_back(std::move(porte));
        } else if (row[0]=="Slime") {
            
            if(row.size() == 4) {
                Vector pos = {static_cast<double>(stoi(row[2])), static_cast<double>(stoi(row[3]))};

                auto ennemi = std::make_unique<Ennemi>(pos);
                ennemis.push_back(std::move(ennemi));
            }else if(row.size()==3) {
                Vector pos = {static_cast<double>(stoi(row[1])) + decalage_x, static_cast<double>(stoi(row[2])) + decalage_y};
                auto ennemi = std::make_unique<Ennemi>(pos);
                ennemis.push_back(std::move(ennemi));
                cout << "Slime charge" << endl;
            }
        }else {
            cerr << "erreur de lecture : impossible d'identifier l'élément de type '" << row[0] << "' à la ligne " << ligne << ", n° : " << i << endl;
        }
        //cout << "element lu." << endl;
    }
    //cout << "fin de construction du niveau" << endl;
    /*cout << "analyse du niveau :" << endl;
    for (int i = 0; i < elements.size(); i++) {
        cout << "Type de l'objet : " << typeid(*elements[i]).name() << endl;
    }*/
}

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

void Niveau::afficher() {
    for (auto& obj : elements) { // Utilisez une référence pour éviter de copier le unique_ptr

        // Obtention d'un pointeur brut pour utiliser dynamic_cast (notament dans les tests)
        Element* rawPtr = obj.get();

        /*cout << "affichage d'un élément" << endl;
                cout << "Test avec typeid(*obj).name(). Type de l'objet : " << typeid(*obj).name() << endl;
        */
        /*if (dynamic_cast<Mur*>(rawPtr)) {
            cout << "C'est un Mur" << endl;
        }*/

        if (rawPtr) { // Vérifiez que le pointeur n'est pas nul
            rawPtr->afficher();
        } else {
            cerr << "objet non construit au moment de l'affichage" << endl;
        }
    }
}

Niveau generer_niveau(std::string chemin_niveau) {
    NiveauTextuel texte = ouvrir_niveau(chemin_niveau);
    Niveau niveauActuel;
    niveauActuel.remplir_niveau(texte);
    return niveauActuel;
}

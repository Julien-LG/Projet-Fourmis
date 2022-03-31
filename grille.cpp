#include <iostream>
#include <vector>
#include<array>
#include <algorithm>
#include <exception>
#include <iomanip>

#include <iomanip> // pour setfill, setw
#include <sstream> // pour ostringstream
#include <fstream> // pour ofstream

using namespace std;
/*#include <doctest/doctest.h>*/
#include "doctest.h"

#include "grille.hpp"

int compteurFichier = 0;

// BEGIN: constructeur
Grille::Grille() {  
    grille_ = vector<vector<Place>>(TAILLEGRILLE);
    for ( long unsigned int i = 0; i < grille_.size(); i++ ) {
        grille_[i] = vector<Place>(TAILLEGRILLE,Coord{0,0});
    }
    
    for(int i = 0; i < TAILLEGRILLE; i++) {
        for(int j = 0; j < TAILLEGRILLE; j++) {
            Place p = Place(Coord{i,j});
            grille_[i][j] = p;
        }
    }        
}
// END: constructeur


// BEGIN: Accès Grille
vector<vector<Place>> Grille::getGrille() const {
    return grille_;
}

vector<Coord> Grille::getCoords() const {
    vector<Coord> v = {};
    for(long unsigned int i = 0; i < grille_.size(); i++) {
        for(long unsigned int j = 0; j < grille_.size(); j++) {
            Place p = grille_[i][j];
            v.push_back(p.coord());
        }
    }
    return v;
}

Place Grille::chargePlace(Coord c) const {
    Place p = grille_[c.getLig()][c.getCol()];
    return p;
}

// END: Accès Grille


// BEGIN: operators
ostream &operator<<(ostream &out, const Grille &g) { 
    vector<vector<Place>> gr = g.getGrille();
    
    for(int i = 0; i < TAILLEGRILLE; i++) {
        for(int j = 0; j < TAILLEGRILLE; j++) {
            if(gr[i][j].estVide()) {
                out << "   ";
            }
            else{
                if(gr[i][j].contientSucre())
                    out << " S ";
                if(gr[i][j].contientNid())
                    out << " N ";
                if(gr[i][j].numeroFourmi() != -1) {
                    if(gr[i][j].numeroColonie() != 0)
                        out << "0" << "f" << gr[i][j].numeroColonie() << "";
                    else
                        out << "f" << gr[i][j].numeroColonie() << " ";
                }
            }
        }
        out << endl;
    }
    return out;
}

bool Grille::operator==(const Grille &g2) const {
    return (getCoords() == g2.getCoords());
}

bool Grille::operator!=(const Grille &g2) const {
    return not(*this == g2);
}
// END: operators


// BEGIN: Modifications Grille
void Grille::rangePlace(Place p) {
    grille_[p.coord().getLig()][p.coord().getCol()] = p;
}
// END: Modifications Grille


// BEGIN: Méthodes avancées Grille
void Grille::placeNid(EnsCoord ensNid, int numColonie) {
    vector<Coord> v = ensNid.getEns();
    for (Coord c : v) {
        Place p = chargePlace(c);    // copie de la place
        p.poseNid(numColonie);
        p.posePheroNid(1, numColonie);
        rangePlace(p);               // range la copie dans la grille
   }
}

void Grille::placeSucre(EnsCoord ensSucre) {
    vector<Coord> v = ensSucre.getEns();
    for (Coord c : v) {
        Place p = chargePlace(c);    // copie de la place
        p.poseSucre();
        //p.posePheroSucre(1);
        rangePlace(p);               // range la copie dans la grille
   }
}

void Grille::placeFourmi(vector<Fourmi> v) {
    for (Fourmi f : v) {
        Place p = chargePlace(f.coord());    // copie de la place
        p.poseFourmi(f);
        rangePlace(p);                       // range la copie dans la grille
   }
}

void Grille::initialiseGrille(EnsCoord vSucre, vector<Colonie> colonies) {
    placeSucre(vSucre);
    
    for(unsigned int i = 0; i < colonies.size(); i++) {
        placeNid(colonies[i].coords(), i);
        placeFourmi(colonies[i].tabFourmi());
        linearisePheroNid(i);
        //linearisePheroNid2(colonies[i]);
    } 
}

void Grille::linearisePheroNid(int numColonie) {
    bool stable = false;
    
    while(not stable) {
        stable = true;
        vector<Coord> v = getCoords();
        for(Coord c : v) {
            Place p = chargePlace(c);
            if(p.pheroNid(numColonie) < 1) {
                EnsCoord CoordVoisins = voisines(c);
                float maxPhero = 0;
                
                for(Coord voi : CoordVoisins.getEns()) {
                    Place voisin = chargePlace(voi);
                    maxPhero = max(maxPhero, voisin.pheroNid(numColonie));
                }
                maxPhero = maxPhero -1. / TAILLEGRILLE;
                if(maxPhero > p.pheroNid(numColonie)) {
                    p.posePheroNid(maxPhero, numColonie);
                    rangePlace(p);
                    stable = false;
                }
            }
        } 
    }
}

void Grille::linearisePheroNid2(Colonie col) {
    Coord cMilieu = col.coords().getEns()[0];
    for(Coord c : getCoords()) {
        Place p = chargePlace(c);
        float distance = c.euclideanDistance(cMilieu);
        p.posePheroNid(distance, col.numero());
        rangePlace(p);
    }
}
    
void Grille::diminuePheroSucre() {
    for(int i = 0; i < TAILLEGRILLE; i++) {
        for(int j = 0; j < TAILLEGRILLE; j++) {
            Place p = chargePlace(Coord{i,j});
            p.diminuePheroSucre();
            rangePlace(p);
        }
    }
}

void Grille::affichePheroNid(int numColonie) {  
    for(int i = 0; i < TAILLEGRILLE; i++) {
        for(int j = 0; j < TAILLEGRILLE; j++) {
            if(grille_[i][j].pheroNid(numColonie) >= 0.95) {
                cout << "  " << grille_[i][j].pheroNid(numColonie) << " ";
            }
            else {
                cout << setprecision(1) << grille_[i][j].pheroNid(numColonie) << " ";
            }      
        }
        cout << endl;
    }
    cout << endl;
}

void Grille::affichePheroSucre(int numColonie) {  
    for(int i = 0; i < TAILLEGRILLE; i++) {
        for(int j = 0; j < TAILLEGRILLE; j++) {
            cout << "" << grille_[i][j].pheroSucre(numColonie) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Grille::afficheSucre() {  
    for(int i = 0; i < TAILLEGRILLE; i++) {
        for(int j = 0; j < TAILLEGRILLE; j++) {
            cout << " " << grille_[i][j].sucre() << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Grille::afficheNumFourmis() {
    for(int i = 0; i < TAILLEGRILLE; i++) {
        for(int j = 0; j < TAILLEGRILLE; j++) {
            if(grille_[i][j].numeroFourmi() > 9) {
                cout << "" << grille_[i][j].numeroFourmi() << " ";
            }
            else if(grille_[i][j].numeroFourmi() >= 0) {
                cout << " " << grille_[i][j].numeroFourmi() << " ";
            }
            else {
                cout << "   ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

Coord Grille::voisinVideAleatoire(Place p) {
    EnsCoord voiscoord = voisines(p.coord());
    
    Coord coo = voiscoord.choixHasard();
    Place p2 = chargePlace(coo);
        
    while(not p2.estVide()) {
        Coord coo = voiscoord.choixHasard();
        p2 = chargePlace(coo);
    }
    return coo;
}

void Grille::ajouteSucreAlea() {  
    Place p = chargePlace(Coord{rand() % TAILLEGRILLE, rand() % TAILLEGRILLE});
        
    while(not p.estVide()) {
        p = chargePlace(Coord{rand() % TAILLEGRILLE, rand() % TAILLEGRILLE});
    }
    
    p.poseSucre();
    rangePlace(p);
}

void Grille::dessinerGrille(vector<Colonie> lesColonies) {
    int r,g,b;
    ostringstream filename;
    // creation d’un nouveau nom de fichier de la forme img347.ppm
    filename << "img/img" << setfill('0') << setw(3) << compteurFichier << ".ppm";
    compteurFichier++;
    cout << "Ecriture dans le fichier : " << filename.str() << endl;
    // ouverture du fichier
    ofstream fic(filename.str(), ios::out | ios::trunc);
    // ecriture de l’entete
    fic << "P3" << endl << TAILLEGRILLE << " " << TAILLEGRILLE << " " << endl << 255 << " " << endl;
       
    for(int i = 0; i < TAILLEGRILLE; i++) {
        for(int j = 0; j < TAILLEGRILLE; j++) {
            Place p = chargePlace(Coord{i,j});
            
            if(p.contientSucre()) {
                if(p.sucre() >= 5) {
                    r = 128; g = 128; b = 128;
                }
                else {
                    r = 192; g = 192; b = 192;
                }
            }
            else if(p.contientNid()) {
                r = 0; g = 0; b = 0;
            }
            else if(p.numeroFourmi() != -1) {
                Fourmi f = lesColonies[p.numeroColonie()].tabFourmi()[p.numeroFourmi()];
                if(p.numeroColonie() == 0) {
                    if(f.porteSucre()) {
                        r = 255; g = 0; b = 0;
                    }
                    else {
                        r = 255; g = 128; b = 128;
                    }
                }
                else if(p.numeroColonie() == 1) {
                    if(f.porteSucre()) {
                        r = 0; g = 0; b = 255;
                    }
                    else {
                        r = 128; g = 128; b = 255;
                    }
                }
                else {
                    if(f.porteSucre()) {
                        r = 0; g = 255; b = 0;
                    }
                    else {
                        r = 128; g = 255; b = 128;
                    }
                }
                
                //r = 0; g = 0; b = 0;
            }
            else if(p.pheroSucre(0) != 0) {
                r = 255; g = 192; b = 192;
            }
            else if(p.pheroSucre(1) != 0) {
                r = 192; g = 192; b = 255;
            }
            /*else if(p.pheroSucre(2) != 0) {
                r = 192; g = 255; b = 192;
            }*/
            else {
                r = 255; g = 255; b = 255;
            }
            
            // ecriture de la couleur dans le fichier
            fic << r << " " << g << " " << b << " ";
        }
        // fin de ligne dans l’image
        fic << endl;
    }
    // fermeture du fichier
    fic.close();
}
// END: Méthodes avancées Grille

// BEGIN: fonction Grille
void controleGrille(Grille g, vector<Colonie> colonies) {
    //Verifie si les indices des fourmis correspondes avec le tabfourmi
    /*for(int i = 0; i < int(tabFourmi.size()); i++){
        if(tabFourmi[i].numero() != i) {
            runtime_error("Erreur indice fourmis et tabFourmi");
        }
    }*/ 
    
    vector<vector<Place>> grille = g.getGrille();
    for(int i = 0; i < TAILLEGRILLE; i++){
        for(int j = 0; j < TAILLEGRILLE; j++){
            Coord c = grille[i][j].coord();
            
            //Vérifie la concordance des coord entre l'emplacement dans la grille et la place associée
            if(c.getLig() != i or c.getCol() != j) {
                runtime_error("Erreur coord place et indices grille");
            }
            
            //Vérifie  si une fourmi inexistante dans le tabFourmi n'est pas présente dans la grille
            if(grille[i][j].numeroFourmi() < -1) {
                runtime_error("Erreur fourmi inexistante présente (<)");
                /*cout << tabFourmi.size() << endl;
                cout << grille[i][j].numeroFourmi() << endl;*/
            }
            for(int y = 0; y < int(colonies.size()); y++) {
                if(grille[i][j].numeroFourmi() >= int(colonies[y].nbFourmis())) {
                    runtime_error("Erreur fourmi inexistante présente (>)");
                }
                   
                //Vérifie la concordance des coord entre l'emplacement dans la grille et la fourmi associée
                if(grille[i][j].numeroFourmi() != -1) {
                    int num = grille[i][j].numeroFourmi();
                    Fourmi f = colonies[y].tabFourmi()[num];
                    Coord c2 = f.coord();
                    if(c2.getLig() != i or c2.getCol() != j) {
                        runtime_error("Erreur coord fourmi et indices grille");
                    }

                    if(grille[i][j].numeroColonie() != y) {
                        runtime_error("Erreur de numero de colonie entre la place et le tableau de colonie");
                    }
                }
                else {
                    if(grille[i][j].numeroColonie() != -1) {
                        runtime_error("Erreur numero de colonie present sans fourmi");
                    }
                }
            }
        }
    }
    
    // Verfie si une fourmi n'est pas superposer sur du sucre ou un nid
    for(unsigned int y = 0; y < colonies.size(); y++) {
        for(int i = 0; i < int(colonies[y].tabFourmi().size()); i++){
            Fourmi f = colonies[y].tabFourmi()[i];
            Place p = grille[f.coord().getLig()][f.coord().getCol()];
            if(p.contientSucre()) {
                runtime_error("Erreur fourmi avec du sucre");
            }
            if(p.contientNid()) {
                runtime_error("Erreur fourmi avec un nid");
            }
        }
    }
}
// END: fonction Grille

/*
TEST_CASE("test 2") {
    Grille g = Grille();
    vector<Coord> v = g.getCoords();
    for(Coord c: v) {
        cout << c << " "; 
    }
}*/
/*
TEST_CASE("test 3") {
    Grille g = Grille();
    g.affichePhero();
    g.linearisePheroNid();
    g.affichePhero();
}*/



// BEGIN: tests 
TEST_CASE("Accès - Modifications - Méthodes") {  
    Grille g = Grille();
    
    Place p1 = Place(Coord{0,0});
    Place p2 = Place(Coord{5,3});
    Place p3 = p1;
    
    // getGrille
    CHECK(g.getGrille()[0][0] == p1);
    CHECK(g.getGrille()[5][3] == p2);
    
    // getCoords
    CHECK(g.getCoords().size() == (TAILLEGRILLE*TAILLEGRILLE));
    
    // chargePlace
    CHECK(g.chargePlace(Coord{0,0}) == p1);
    CHECK(g.chargePlace(Coord{5,3}) == p2);
    CHECK_FALSE(g.chargePlace(Coord{5,3}) == p1);
    
    // rangePlace
    //p1.placeSucre(EnsCoord({Coord{0,0}}));
    p1.poseSucre();
    g.rangePlace(p1);
    CHECK(g.chargePlace(Coord{0,0}) == p1);
    CHECK_FALSE(g.chargePlace(Coord{0,0}) == p3);
    
    p1 = Place(Coord{0,0});
    p3 = p1;
    g.rangePlace(p1);
    
    // placeNid
    g.placeNid(EnsCoord({Coord{0,0}}), 0);
    p1.poseNid(0);
    p1.posePheroNid(1, 0);
    CHECK(g.chargePlace(Coord{0,0}) == p1);
    CHECK_FALSE(g.chargePlace(Coord{0,0}) == p3);
    
    p1 = Place(Coord{0,0});
    g.rangePlace(p1);
    
    // placeSucre
    g.placeSucre(EnsCoord({Coord{0,0}}));
    p1.poseSucre();
    CHECK(g.chargePlace(Coord{0,0}) == p1);
    CHECK_FALSE(g.chargePlace(Coord{0,0}) == p3);
    
    p1 = Place(Coord{0,0});
    g.rangePlace(p1);
    
    // placeFourmi
    g.placeFourmi({Fourmi(Coord{0,0}, 0, 0)});
    p1.poseFourmi(Fourmi(Coord{0,0}, 0, 0));
    CHECK(g.chargePlace(Coord{0,0}) == p1);
    CHECK_FALSE(g.chargePlace(Coord{0,0}) == p3);
    
    p1 = Place(Coord{0,0});
    g.rangePlace(p1);
    
    // initialiseGrille
    g.initialiseGrille(EnsCoord({Coord{0,0}}), {Colonie(0, Coord{2,2})});
    CHECK(g.chargePlace(Coord{0,0}).contientSucre());
    
    CHECK(g.chargePlace(Coord{2,2}).contientNid());
    CHECK(g.chargePlace(Coord{2,3}).contientNid());
    CHECK(g.chargePlace(Coord{3,2}).contientNid());
    CHECK(g.chargePlace(Coord{3,3}).contientNid());
    
    CHECK(g.chargePlace(Coord{1,1}).numeroFourmi() == 0);
    CHECK(g.chargePlace(Coord{1,2}).numeroFourmi() == 1);
    CHECK(g.chargePlace(Coord{1,3}).numeroFourmi() == 2);
    CHECK(g.chargePlace(Coord{2,1}).numeroFourmi() == 3);
    CHECK(g.chargePlace(Coord{3,1}).numeroFourmi() == 4);
    CHECK(g.chargePlace(Coord{1,4}).numeroFourmi() == 5);
    CHECK(g.chargePlace(Coord{2,4}).numeroFourmi() == 6);
    CHECK(g.chargePlace(Coord{3,4}).numeroFourmi() == 7);
    CHECK(g.chargePlace(Coord{4,1}).numeroFourmi() == 8);
    CHECK(g.chargePlace(Coord{4,2}).numeroFourmi() == 9);
    CHECK(g.chargePlace(Coord{4,3}).numeroFourmi() == 10);
    CHECK(g.chargePlace(Coord{4,4}).numeroFourmi() == 11);
    
    // diminuePheroSucre
    p2 = g.chargePlace(Coord{5,0});
    p2.posePheroSucre(15, 0);
    g.rangePlace(p2);
    CHECK(g.chargePlace(Coord{5,0}).pheroSucre(0) == 15);
    g.diminuePheroSucre();
    CHECK_FALSE(g.chargePlace(Coord{5,0}).pheroSucre(0) == 15);
    CHECK(g.chargePlace(Coord{5,0}).pheroSucre(0) == 10);
}
// END: tests

// BEGIN: test affichage
TEST_CASE("test affichage") {
    Grille g = Grille();
    
    vector<Coord> vSucre = {Coord{1,1},Coord{TAILLEGRILLE-2,TAILLEGRILLE-2}};

    //Un seul nid au milieu de la grille
    vector<Colonie> colonies = {Colonie(0, Coord{(TAILLEGRILLE/2)-1,(TAILLEGRILLE/2)-1})};
    //vector<Colonie> colonies = {Colonie(0, Coord{1,TAILLEGRILLE-3}), Colonie(1, Coord{TAILLEGRILLE-3,1})};
    //7.1 / 1.7
    
    // Affichage des pheromone de sucre
    g.affichePheroSucre(0);
    
    // Affichage des numéros de fourmis
    g.afficheNumFourmis();
    
    // Affichage de pheromone de nid
    g.affichePheroNid(0);
    g.affichePheroNid(1);
    g.initialiseGrille(vSucre, colonies);
    
    // Affichage "classique", avec num fourmis, sucre, nids
    cout << g << endl;
    g.affichePheroNid(0);
    g.affichePheroNid(1);
    controleGrille(g, colonies);
}
// END: test affichage

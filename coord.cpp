#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;
/*#include <doctest/doctest.h>*/
#include "doctest.h"

#include "coord.hpp"

// BEGIN: constructeurs
Coord::Coord(int lig, int col) : numLigne_{lig}, numColonne_{col} {
    if(lig >= TAILLEGRILLE or lig < 0)
        throw runtime_error("taille ligne incorrecte");
    if(col >= TAILLEGRILLE or col < 0)
        throw runtime_error("taille colonne incorrecte");
}

EnsCoord::EnsCoord(vector<Coord> ensemble) : ensemble_{ensemble} {
    
}
// END: constructeurs

// BEGIN: getters
int Coord::getLig() const {
    return numLigne_;
}
    
int Coord::getCol() const {
    return numColonne_;
}

vector<Coord> EnsCoord::getEns() const {
    return ensemble_;
}
// END: getters

// BEGIN: tests getters
TEST_CASE("getLig | getCol") {
    Coord c = Coord(5,3);
    CHECK(c.getLig() == 5);
    CHECK_FALSE(c.getLig() == 3);
    CHECK_FALSE(c.getLig() == 0);
    CHECK(c.getCol() == 3);
    CHECK_FALSE(c.getCol() == 5);
    CHECK_FALSE(c.getCol() == 0);
}

TEST_CASE("getEns") {
    vector<Coord> vc = {Coord(5,3), Coord(3,5)};
    vector<Coord> vc2 = {Coord(3,5), Coord(5,3)};
    EnsCoord e = EnsCoord(vc);
    CHECK(e.getEns() == vc);
    CHECK_FALSE(e.getEns() == vc2);
}
// END: tests getters

// BEGIN: operators
ostream &operator<<(ostream &out, const Coord &coord) {
    out << "(" << coord.getLig() << "," << coord.getCol() << ")";
    return out;
}

bool Coord::operator==(const Coord &c2) const {
    return (numLigne_ == c2.getLig() and numColonne_ == c2.getCol());
}

bool Coord::operator!=(const Coord &c2) const {
    return not(*this == c2);
}

ostream &operator<<(ostream &out, const EnsCoord &ensCoord) {
    int len = ensCoord.getEns().size();
    if(len > 0) {
        out << "{";
        for(int i = 0; i < len; i++) {
            out << ensCoord.getEns()[i] << ",";
        }
        out << "}";
    }
    return out;
}

bool EnsCoord::operator==(const EnsCoord &e2) const {
    if(taille() == e2.taille()){
        for(int i = 0; i < taille(); i++){
            if(not(e2.contient(ieme(i))))
                return false;
        }
        return true;
    }
    return false;
}

bool EnsCoord::operator!=(const EnsCoord &e2) const {
    return not(*this == e2);
}

// END: operators

// BEGIN: tests operators
TEST_CASE("operators == Coord") {
    CHECK(Coord{5,3} == Coord{5,3});
    CHECK_FALSE(Coord{5,3} == Coord{3,5});
    CHECK_FALSE(Coord{5,3} == Coord{3,3});
    CHECK_FALSE(Coord{5,3} == Coord{5,5});
}

TEST_CASE("operators == EnsCoord") {
    EnsCoord ens = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2},Coord{6,4}});
    EnsCoord ens2 = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2},Coord{6,4}});
    EnsCoord ens3 = EnsCoord(vector<Coord>{Coord{1,2},Coord{5,3},Coord{6,4}});
    EnsCoord ens4 = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2}});
    EnsCoord ens5 = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2},Coord{6,5}});
    
    CHECK(ens == ens2);
    CHECK(ens == ens3);
    CHECK_FALSE(ens == ens4);
    CHECK_FALSE(ens == ens5);
}
// END: tests operators

// BEGIN: fonction coord
float Coord::euclideanDistance(const Coord c2) const {
    return sqrt((c2.getLig() - getLig())*(c2.getLig() - getLig()) + (c2.getCol() - getCol())*(c2.getCol() - getCol()));
}
// END: fonction coord

// BEGIN: test fonction coord
TEST_CASE("euclideanDistance") {
    float e1 = Coord{5,3}.euclideanDistance(Coord{3,1});
    float e2 = Coord{9,5}.euclideanDistance(Coord{0,8});
    float e3 = Coord{5,3}.euclideanDistance(Coord{5,3});
    float e4 = Coord{5,3}.euclideanDistance(Coord{3,5});
    float e5 = Coord{0,0}.euclideanDistance(Coord{0,0});
    
    CHECK((e1 > 2.82842 and e1 < 2.82844));
    CHECK((e2 > 9.48682 and e2 < 9.48684));
    CHECK(e3 == 0);
    CHECK((e4 > 2.82842 and e4 < 2.82844));
    CHECK(e5 == 0);
}
// END: test fonction coord

// BEGIN: méthodes EnsCoord
int EnsCoord::position(Coord c) const {
    int len = ensemble_.size();
    if(len > 0) {
        for(int i = 0; i < len; i++) {
            if(ensemble_[i] == c)
                return i;
        }
    }
    return -1;
}

bool EnsCoord::contient(Coord c) const {
    return position(c) != -1;
}

void EnsCoord::ajoute(Coord c) {
    if(not(contient(c))){
        ensemble_.push_back(c);
    }
}
void EnsCoord::supprime(Coord c) {
    if(contient(c)){
        int pos = position(c);
        ensemble_.erase(ensemble_.begin() + pos);        
    }
    else {
        throw runtime_error("Impossible de supprimer la coordonnée, car elle n'est pas présente.");
    }
}

bool EnsCoord::estVide() {
    return ensemble_.size() == 0;
}

int EnsCoord::taille() const {
    return ensemble_.size();
}

Coord EnsCoord::ieme(int n) const {
    if(n < taille() and n >= 0) {
        return ensemble_[n];
    }
    throw runtime_error("Position incorrecte.");
}

Coord EnsCoord::choixHasard() {
    if(not(estVide())) {
        int t = taille();
        return ieme(rand() % t);
    }
    throw runtime_error("Vide, impossible de renvoyer une coordonnée.");
}
// END: méthodes EnsCoord

// BEGIN: tests méthodes EnsCoord
TEST_CASE("contient") {
    EnsCoord ens = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2},Coord{6,4}});
    
    CHECK(ens.contient(Coord{5,3}) == true);
    CHECK(ens.contient(Coord{1,2}) == true);
    CHECK(ens.contient(Coord{6,4}) == true);
    CHECK(ens.contient(Coord{0,0}) == false);
}

TEST_CASE("ajoute") {
    EnsCoord ens = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2},Coord{6,4}});
    
    ens.ajoute(Coord{5,4});
    CHECK(ens.getEns().size() == 4);
    ens.ajoute(Coord{4,5});
    CHECK(ens.getEns().size() == 5);
    ens.ajoute(Coord{3,3});
    CHECK(ens.getEns().size() == 6);
    ens.ajoute(Coord{5,3});
    CHECK(ens.getEns().size() == 6);
}

TEST_CASE("supprime") {
    EnsCoord ens = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2},Coord{6,4}});
    
    ens.supprime(Coord{5,3});
    CHECK(ens.getEns().size() == 2);
    ens.supprime(Coord{6,4});
    CHECK(ens.getEns().size() == 1);
    ens.supprime(Coord{1,2});
    CHECK(ens.getEns().size() == 0);
}

TEST_CASE("estVide") {
    EnsCoord ens = EnsCoord(vector<Coord>{Coord{5,3}});
    
    CHECK(ens.estVide() == false);
    ens.supprime(Coord{5,3});
    CHECK(ens.estVide() == true);
}

TEST_CASE("taille") {
    EnsCoord ens = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2},Coord{6,4}});
    
    CHECK(ens.taille() == 3);
    ens.supprime(Coord{5,3});
    CHECK(ens.taille() == 2);
    ens.supprime(Coord{6,4});
    CHECK(ens.taille() == 1);
    ens.supprime(Coord{1,2});
    CHECK(ens.taille() == 0);
}

TEST_CASE("ieme") {
    EnsCoord ens = EnsCoord(vector<Coord>{Coord{5,3},Coord{1,2},Coord{6,4}});
    
    CHECK(ens.ieme(0) == Coord{5,3});
    CHECK(ens.ieme(1) == Coord{1,2});
    CHECK(ens.ieme(2) == Coord{6,4});
}

TEST_CASE("choixHasard") {
    EnsCoord ens1 = EnsCoord(vector<Coord>{Coord{5,3}});
    EnsCoord ens2 = EnsCoord(vector<Coord>{Coord{1,2}});
    EnsCoord ens3 = EnsCoord(vector<Coord>{Coord{6,4}});
    
    CHECK(ens1.choixHasard() == Coord{5,3});
    CHECK(ens2.choixHasard() == Coord{1,2});
    CHECK(ens3.choixHasard() == Coord{6,4});
}
// END: tests méthodes EnsCoord


// BEGIN: fonctions
EnsCoord voisines(Coord c) {
    EnsCoord ens = EnsCoord(vector<Coord>{});
    
    int imin = max(c.getLig()-1,0);
    int imax = min(c.getLig()+1,TAILLEGRILLE-1);
    int jmin = max(c.getCol()-1,0);
    int jmax = min(c.getCol()+1,TAILLEGRILLE-1);
    
    for(int i = imin; i <= imax; i++){
        for(int j = jmin; j <= jmax; j++){
            if(Coord{i,j} != Coord{c.getLig(),c.getCol()}){
                ens.ajoute(Coord{i,j});
            }
        }
    }
    return ens;
}


// END: fonctions

        
// BEGIN: tests fonctions
        
TEST_CASE("voisines") {
    // Pour taillegrille = 10
    /*EnsCoord ens2 = EnsCoord(vector<Coord>{Coord{7,8},Coord{7,9},Coord{8,8},Coord{9,8},Coord{9,9}});
    EnsCoord ens3 = EnsCoord(vector<Coord>{Coord{0,1},Coord{1,0},Coord{1,1}});
    EnsCoord ens4 = EnsCoord(vector<Coord>{Coord{8,8},Coord{8,9},Coord{9,8}});*/
    
    // Pour taillegrille de n'importe quelle taille
    EnsCoord ens = EnsCoord(vector<Coord>{Coord{1,0},Coord{1,1},Coord{1,2},Coord{2,2},Coord{3,2},Coord{3,1},Coord{3,0},Coord{2,0}});
    EnsCoord ens1 = EnsCoord(vector<Coord>{Coord{1,0},Coord{1,1},Coord{1,2},Coord{2,0},Coord{2,2},Coord{3,0},Coord{3,1},Coord{3,2}});
    EnsCoord ens2 = EnsCoord(vector<Coord>{Coord{TAILLEGRILLE-3,TAILLEGRILLE-2},Coord{TAILLEGRILLE-3,TAILLEGRILLE-1},Coord{TAILLEGRILLE-2,TAILLEGRILLE-2},Coord{TAILLEGRILLE-1,TAILLEGRILLE-2},Coord{TAILLEGRILLE-1,TAILLEGRILLE-1}});
    EnsCoord ens3 = EnsCoord(vector<Coord>{Coord{0,1},Coord{1,0},Coord{1,1}});
    EnsCoord ens4 = EnsCoord(vector<Coord>{Coord{TAILLEGRILLE-2,TAILLEGRILLE-2},Coord{TAILLEGRILLE-2,TAILLEGRILLE-1},Coord{TAILLEGRILLE-1,TAILLEGRILLE-2}});
    
    CHECK(voisines(Coord{2,1}) == ens);
    CHECK(voisines(Coord{2,1}) == ens1);
    /*CHECK(voisines(Coord{8,9}) == ens2);
    CHECK(voisines(Coord{9,9}) == ens4);*/
    CHECK(voisines(Coord{TAILLEGRILLE-2,TAILLEGRILLE-1}) == ens2);
    CHECK(voisines(Coord{0,0}) == ens3);
    CHECK(voisines(Coord{TAILLEGRILLE-1,TAILLEGRILLE-1}) == ens4);
}
// END: tests fonctions

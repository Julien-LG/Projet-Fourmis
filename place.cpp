#include <iostream>
#include <vector>

using namespace std;
/*#include <doctest/doctest.h>*/
#include "doctest.h"

#include "place.hpp"

// BEGIN: constructeur
Place::Place(Coord c) : coord_{c}, numFourmi_{-1}, numColonieFourmi_{-1}, sucre_{0}, nid_{-1}, pheroSucre_{vector<int>(NOMBRECOLONIES, 0)}, pheroNid_{vector<float>(NOMBRECOLONIES, 0)} {
    
}
// END: constructeur


// BEGIN: Accès Place
Coord Place::coord() const {
    return coord_;
}
    
int Place::pheroSucre(int colonie) const {
    return pheroSucre_[colonie];
}

float Place::pheroNid(int colonie) const {
    return pheroNid_[colonie];
}

int Place::numeroFourmi() const {
    return numFourmi_;
}

int Place::numeroColonie() const {
    return numColonieFourmi_;
}

int Place::sucre() const {
    return sucre_;
}
// END: Accès Place


// BEGIN: Prédicats Place
bool Place::contientSucre() const {
    return sucre_ > 0;
}
    
bool Place::contientNid() const {
    return nid_ != -1 ;
}

bool Place::estSurUnePiste(int colonie) const {
    return pheroSucre_[colonie] > 0;
}

bool Place::estVide() const {
    return not(contientSucre()) and not(contientNid()) and numFourmi_ == -1;
}
/*
bool Place::estPlusProcheNid(Place &p1, Place &p2) {
    return coord().euclideanDistance(p1.coord()) < coord().euclideanDistance(p2.coord());
}

bool Place::estPlusLoinNid(Place &p1, Place &p2) {
    return coord().euclideanDistance(p1.coord()) > coord().euclideanDistance(p2.coord());
}*/

bool Place::estPlusProcheNid(Place &p2, int colonie) {
    return pheroNid(colonie) > p2.pheroNid(colonie);
    //return pheroNid(colonie) < p2.pheroNid(colonie);
}

bool Place::estPlusLoinNid(Place &p2, int colonie) {
    return pheroNid(colonie) < p2.pheroNid(colonie);
    //return pheroNid(colonie) >= p2.pheroNid(colonie);
}
// END: Prédicats Place


// BEGIN: operators
ostream &operator<<(ostream &out, const Place &p) {
    out << "Place aux coord : " << p.coord();
    if(p.numeroFourmi() != -1)
        out << " contient la fourmi n° " << p.numeroFourmi() << " de la colonie n°" << p.numeroColonie();
    if(p.contientSucre())
        out << " contient du sucre ";
    if(p.contientNid())
        out << " contient un élément de nid ";
    
    for(int i = 0; i < NOMBRECOLONIES; i++) {
        if(p.pheroSucre(i) != 0)
            out << " contient " << ((p.pheroSucre(i)/255)*100) << "% de phero de sucre de la colonie n°" << i;
        if(p.pheroNid(i) != 0)
            out << " contient " << (p.pheroNid(i)*100) << "% de phero de nid de la colonie n°" << i;
    }
    
    return out;
}

bool Place::operator==(const Place &p2) const {
    return (coord_ == p2.coord() and numFourmi_ == p2.numeroFourmi() and numColonieFourmi_ == p2.numeroColonie() and contientSucre() == p2.contientSucre() and contientNid() == p2.contientNid() and pheroSucre(0) == p2.pheroSucre(0) and pheroNid(0) == p2.pheroNid(0));
}

bool Place::operator!=(const Place &p2) const {
    return not(*this == p2);
}
// END: operators


// BEGIN: Modifications Place
void Place::poseSucre() {
    sucre_ = 10;
}

void Place::enleveSucre() {
    sucre_ -= 1;
}

void Place::poseNid(int colonie) {
    nid_ = colonie;
}
      
void Place::poseFourmi(Fourmi f) {
    numFourmi_ = f.numero();
    numColonieFourmi_ = f.colonie();
}
      
void Place::enleveFourmi() {
    numFourmi_ = -1;
    numColonieFourmi_ = -1;
}
      
void Place::posePheroNid(float qte, int colonie) {
    pheroNid_[colonie] = qte;
}
      
void Place::posePheroSucre (int qte, int colonie) {
    pheroSucre_[colonie] = qte;
}
      
void Place::diminuePheroSucre() {  
    //Diminue les pheroSucre de toutes les conlonies
    for (unsigned int i = 0; i < pheroSucre_.size(); i++) {
        if(pheroSucre_[i] > 0) {
            pheroSucre_[i] -= 5;
        }
    }
}
// END: Modifications Place


// BEGIN: Fonction Place
void deplace(Fourmi &f, Place &p1, Place &p2) {
    if(p2.estVide()) {
        f.deplace(p2.coord());
        p1.enleveFourmi();
        p2.poseFourmi(f);
    }
}
// END: Fonction Place

// BEGIN: tests Accès 
TEST_CASE("Accès") {
    Place p = Place(Coord{5,3});
    
    // Coord
    CHECK(p.coord() == Coord{5,3});
    CHECK_FALSE(p.coord() == Coord{3,5});
    
    // pheroSucre
    CHECK(p.pheroSucre(0) == 0);
    CHECK_FALSE(p.pheroSucre(0) == 1);
    
    // pheroNid
    CHECK(p.pheroNid(0) == 0);
    CHECK_FALSE(p.pheroNid(0) == 1);
    
    // numeroFourmi
    CHECK(p.numeroFourmi() == -1);
    CHECK_FALSE(p.numeroFourmi() == 0);
    CHECK_FALSE(p.numeroFourmi() == 5);
    
    // numeroColonie
    CHECK(p.numeroColonie() == -1);
    CHECK_FALSE(p.numeroColonie() == 0);
    CHECK_FALSE(p.numeroColonie() == 1);
    
    // sucre
    CHECK(p.sucre() == 0);
    CHECK_FALSE(p.sucre() == -1);
    CHECK_FALSE(p.sucre() == 1);
}
// END: tests Accès


// BEGIN: tests Prédicats 
TEST_CASE("Prédicats") {
    Place p = Place(Coord{5,3});
    //Place p2 = Place(Coord{5,4});
    
    // contientSucre
    CHECK_FALSE(p.contientSucre());
    
    // contientNid
    CHECK_FALSE(p.contientNid());
    
    // estSurUnePiste
    CHECK_FALSE(p.estSurUnePiste(0));
    
    // estVide
    CHECK(p.estVide());
}

TEST_CASE("Prédicats 2") {
    Place p = Place(Coord{5,3});
    Place p2 = Place(Coord{5,4});
    Place p3 = Place(Coord{9,9});
    Fourmi f = Fourmi(Coord{5,3}, 2, 0);
    
    p.posePheroSucre(5, 0);
    CHECK(p.pheroSucre(0) == 5);
    CHECK(p.estSurUnePiste(0));
    
    p.poseSucre();
    CHECK(p.contientSucre());
    CHECK_FALSE(p.estVide());
    
    p2.poseNid(0);
    CHECK(p2.contientNid());
    CHECK_FALSE(p2.estVide());
    
    p3.poseFourmi(f);
    CHECK(p3.numeroFourmi() == 2);
    CHECK(p3.numeroColonie() == 0);
    CHECK_FALSE(p3.estVide());
    
    p.posePheroNid(0.9, 0);
    p2.posePheroNid(0.1, 0);
    CHECK(p.pheroNid(0) == 0.9f);
    CHECK(p2.pheroNid(0) == 0.1f);
    CHECK(p.estPlusProcheNid(p2, 0));
    CHECK(p2.estPlusLoinNid(p, 0));
}
// END: tests Prédicats 


// BEGIN: tests operators
TEST_CASE("operators") {
    Place p1 = Place(Coord{5,3});
    Place p2 = Place(Coord{5,3});
    Place p3 = Place(Coord{3,5});
    Place p4 = Place(Coord{5,5});
    
    CHECK(p1 == p2);
    CHECK_FALSE(p1 == p3);
    CHECK_FALSE(p1 == p4);
}
// END: tests operators


// BEGIN: tests Modifications Place
TEST_CASE("poseSucre - enleveSucre") {
    Place p = Place(Coord{5,3});
    
    CHECK_FALSE(p.contientSucre());
    p.poseSucre();    
    CHECK(p.contientSucre());
    p.enleveSucre();
    CHECK(p.contientSucre());
    for(int i = 0; i < 9; i++) {
        p.enleveSucre();
    }
    CHECK_FALSE(p.contientSucre());
    p.poseSucre();
    p.poseSucre();
    CHECK(p.contientSucre());
}

TEST_CASE("poseNid") {
    Place p = Place(Coord{5,3});
    
    CHECK_FALSE(p.contientNid());
    p.poseNid(0);
    CHECK(p.contientNid());
    p.poseNid(0);
    CHECK(p.contientNid());
}

TEST_CASE("poseFourmi - enleveFourmi") {
    Place p = Place(Coord{5,3});
    Fourmi f = Fourmi(Coord{5,3}, 2, 0);
    
    CHECK(p.numeroFourmi() == -1);
    p.poseFourmi(f);    
    CHECK(p.numeroFourmi() == 2);
    p.enleveFourmi();
    CHECK(p.numeroFourmi() == -1);
    p.enleveFourmi();
    CHECK(p.numeroFourmi() == -1);
    p.poseFourmi(f);
    p.poseFourmi(f);
    CHECK(p.numeroFourmi() == 2);
}

TEST_CASE("posePheroNid") {
    Place p = Place(Coord{5,3});
    
    CHECK(p.pheroNid(0) == 0);
    p.posePheroNid(1, 0);    
    CHECK(p.pheroNid(0) == 1);
    p.posePheroNid(2, 0);
    CHECK(p.pheroNid(0) == 2);
    p.posePheroNid(3, 0);
    CHECK(p.pheroNid(0) == 3);
    p.posePheroNid(1.1, 0);
    CHECK(p.pheroNid(0) == 1.1f);
    p.posePheroNid(1, 1);
    CHECK(p.pheroNid(1) == 1);
}

TEST_CASE("posePheroSucre - diminuePheroSucre") {
    Place p = Place(Coord{5,3});
    
    CHECK(p.pheroSucre(0) == 0);
    p.posePheroSucre(1, 0);    
    CHECK(p.pheroSucre(0) == 1);
    p.posePheroSucre(2, 0);
    CHECK(p.pheroSucre(0) == 2);
    p.posePheroSucre(3, 0);
    CHECK(p.pheroSucre(0) == 3);
    p.posePheroSucre(5, 0);
    CHECK(p.pheroSucre(0) == 5);
    
    p.diminuePheroSucre();
    CHECK(p.pheroSucre(0) == 0);
    p.posePheroSucre(10, 0);
    p.posePheroSucre(10, 1);
    p.diminuePheroSucre();
    p.diminuePheroSucre();
    CHECK(p.pheroSucre(0) == 0);
    CHECK(p.pheroSucre(1) == 0);
}
// END: tests Modifications Place

// BEGIN: tests Fonctions Place
TEST_CASE("deplace") {
    Place p1 = Place(Coord{5,3});
    Place p2 = Place(Coord{3,3});
    Fourmi f = Fourmi(Coord{5,3}, 2, 0);
    p1.poseFourmi(f);
    
    CHECK(f.coord() == Coord{5,3});
    CHECK(p1.numeroFourmi() == 2);
    CHECK(p2.numeroFourmi() == -1);
    
    deplace(f, p1, p2);
    
    CHECK(f.coord() == Coord{3,3});
    CHECK(p1.numeroFourmi() == -1);
    CHECK(p2.numeroFourmi() == 2);
    
    deplace(f, p2, p1);
    
    CHECK(f.coord() == Coord{5,3});
    CHECK(p1.numeroFourmi() == 2);
    CHECK(p2.numeroFourmi() == -1);
    
    p2.poseNid(0);
    
    deplace(f, p1, p2);
    
    CHECK(f.coord() == Coord{5,3});
    CHECK(p1.numeroFourmi() == 2);
    CHECK(p2.numeroFourmi() == -1);
}
// END: tests Fonctions Place
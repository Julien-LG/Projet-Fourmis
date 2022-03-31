#include <iostream>
#include <vector>

using namespace std;
/*#include <doctest/doctest.h>*/
#include "doctest.h"

#include "fourmi.hpp"

// BEGIN: constructeur
Fourmi::Fourmi(Coord c, int num, int colonie) : coord_{c}, porteSucre_{false}, numero_{num}, colonie_{colonie}, enVie_{true} {
    
}
// END: constructeur

// BEGIN: getters Fourmi
Coord Fourmi::coord() const {
    return coord_;
}
    
int Fourmi::numero() const {
    return numero_;
}

int Fourmi::colonie() const {
    return colonie_;
}

bool Fourmi::porteSucre() const {
    return porteSucre_;
}

bool Fourmi::chercheSucre() const {
    return not(porteSucre_);
}

bool Fourmi::estEnVie() const {
    return enVie_;
}

// END: getters Fourmi

// BEGIN: tests getters
TEST_CASE("getters") {
    Fourmi f = Fourmi(Coord{5,3}, 1, 1);
    
    // coord
    CHECK(f.coord() == Coord{5,3});
    CHECK_FALSE(f.coord() == Coord{3,5});
    
    // numero
    CHECK(f.numero() == 1);
    CHECK_FALSE(f.numero() == 0);
    
    // porteSucre
    CHECK(f.porteSucre() == false);
    CHECK_FALSE(f.porteSucre() == true);
    
    // chercheSucre
    CHECK(f.colonie() == 1);
    CHECK_FALSE(f.colonie() == 0);
    
    // estEnVie
    CHECK(f.estEnVie() == 1);
    CHECK_FALSE(f.estEnVie() == 0);
}
// END: tests getters

// BEGIN: operators
ostream &operator<<(ostream &out, const Fourmi &f) {
    out << "Fourmi n°" << f.numero() << " de la colonie n°" << f.colonie() << " aux coord : " << f.coord();
    if(f.porteSucre())
        out << " porte du sucre ";
    else
        out << " ne porte pas du sucre "; 
    if(f.estEnVie())
        out << " et est en vie.";
    else
        out << " et est morte.";
    return out;
}

bool Fourmi::operator==(const Fourmi &f2) const {
    return (coord_ == f2.coord() and porteSucre_ == f2.porteSucre() and numero_ == f2.numero() and colonie_ == f2.colonie() and enVie_ == f2.estEnVie());
}

bool Fourmi::operator!=(const Fourmi &f2) const {
    return not(*this == f2);
}
// END: operators

// BEGIN: tests operators
TEST_CASE("operators") {
    Fourmi f1 = Fourmi(Coord{5,3}, 1, 1);
    Fourmi f2 = Fourmi(Coord{5,3}, 1, 1);
    Fourmi f3 = Fourmi(Coord{3,5}, 1, 1);
    Fourmi f4 = Fourmi(Coord{5,3}, 2, 1);
    Fourmi f5 = Fourmi(Coord{5,3}, 1, 2);
    
    CHECK(f1 == f2);
    CHECK_FALSE(f1 == f3);
    CHECK_FALSE(f1 == f4);
    CHECK_FALSE(f1 == f5);
}
// END: tests operators

// BEGIN: setters Fourmi
void Fourmi::prendSucre() {
    porteSucre_ = true;
}

void Fourmi::poseSucre() {
    porteSucre_ = false;
}

void Fourmi::doitMourir() {
    enVie_ = false;
}

void Fourmi::deplace(Coord c) {
    coord_ = c;
}
// END: setters Fourmi

// BEGIN: tests setters Fourmi

TEST_CASE("prend | pose Sucre") {
    Fourmi f = Fourmi(Coord{5,3}, 1, 1);
    
    CHECK_FALSE(f.porteSucre());
    f.prendSucre();
    CHECK(f.porteSucre());
    f.poseSucre();
    CHECK_FALSE(f.porteSucre());
    f.poseSucre();
    CHECK_FALSE(f.porteSucre());
}

TEST_CASE("doitMourir") {
    Fourmi f = Fourmi(Coord{5,3}, 1, 1);
    
    CHECK(f.estEnVie());
    f.doitMourir();
    CHECK_FALSE(f.estEnVie());
    f.doitMourir();
    CHECK_FALSE(f.estEnVie());
}

TEST_CASE("deplace") {
    Fourmi f = Fourmi(Coord{5,3}, 1, 1);
    
    f.deplace(Coord{5,3});
    CHECK(f.coord() == Coord{5,3});
    f.deplace(Coord{1,2});
    CHECK(f.coord() == Coord{1,2});
    f.deplace(Coord{6,4});
    CHECK(f.coord() == Coord{6,4});
    f.deplace(Coord{0,0});
    CHECK(f.coord() == Coord{0,0});
}

// END: tests setters Fourmi

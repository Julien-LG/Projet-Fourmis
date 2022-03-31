#include <iostream>
#include <vector>

using namespace std;
/*#include <doctest/doctest.h>*/
#include "doctest.h"

#include "colonie.hpp"

// BEGIN: constructeur
Colonie::Colonie(int num, Coord c) : numero_{num}, fourmis_{{}}, coordsNid_{{}}, stockSucre_{0}, numDernierefourmi_{-1} {
    // Prépare l'emplacemet du nid de 2x2 qui se trouve a coté de la premiere coord
    vector<Coord> vNid = {c, Coord{c.getLig(), c.getCol()+1}, Coord{c.getLig()+1,c.getCol()}, Coord{c.getLig()+1,c.getCol()+1}};
    vector<Fourmi> vFourmi = vector<Fourmi>{};
    
    // Genere des fourmis autour du nid
    EnsCoord coordFourmis = voisines(vNid[0]);
    for(unsigned int i = 1; i < vNid.size(); i++) {
        EnsCoord e2 = voisines(vNid[i]);
        for(int j = 0; j < e2.taille(); j++){
            Coord c = e2.ieme(j);
            if(not coordFourmis.contient(c))
                coordFourmis.ajoute(c);
        }
    }
    
    // Supprime les coordonnées des éléments de nid voisins
    for(unsigned int y = 0; y < vNid.size(); y++) {
        Coord c = vNid[y];
        if(coordFourmis.contient(c))
                coordFourmis.supprime(c);
    }  
    
    // Ajoute les fourmis dans le tableau
    for(int i = 0; i < coordFourmis.taille(); i++) {
        vFourmi.push_back(Fourmi(coordFourmis.ieme(i), i, num));
    }
    
    coordsNid_ = EnsCoord(vNid);
    fourmis_ = vFourmi;
    numDernierefourmi_ = vFourmi.size()-1;
}
// END: constructeur

// BEGIN: getters Colonie
EnsCoord Colonie::coords() const {
    return coordsNid_;
}
    
int Colonie::numero() const {
    return numero_;
}

int Colonie::stockSucre() const {
    return stockSucre_;
}

vector<Fourmi> Colonie::tabFourmi() const{
    return fourmis_;
}

int Colonie::nbFourmis() const{
    return numDernierefourmi_ + 1;
}

int Colonie::nbFourmisVivantes() const{
    int nb = 0;
    for(Fourmi f : fourmis_) {
        if(f.estEnVie())
            nb++;
    }
    return nb;
}

// END: getters Colonie

// BEGIN: tests getters Colonie
TEST_CASE("getters") {
    Colonie c = Colonie(0, Coord{2,2});
    vector<Fourmi> vf = {};
    
    // coords
    CHECK(c.coords() == EnsCoord({Coord{2,2},Coord{2,3},Coord{3,2},Coord{3,3}}));
    CHECK_FALSE(c.coords() == EnsCoord({Coord{3,5}}));
    
    // numero
    CHECK(c.numero() == 0);
    CHECK_FALSE(c.numero() == -1);
    
    // stockSucre
    CHECK(c.stockSucre() == 0);
    CHECK_FALSE(c.stockSucre() == 1);
    
    // tabFourmi
    CHECK_FALSE(c.tabFourmi() == vf);
    vf = {Fourmi(Coord{1,1}, 0, 0), Fourmi(Coord{1,2}, 1, 0), Fourmi(Coord{1,3}, 2, 0), Fourmi(Coord{2,1}, 3, 0), Fourmi(Coord{3,1}, 4, 0), Fourmi(Coord{1,4}, 5, 0), Fourmi(Coord{2,4}, 6, 0), Fourmi(Coord{3,4}, 7, 0), Fourmi(Coord{4,1}, 8, 0), Fourmi(Coord{4,2}, 9, 0), Fourmi(Coord{4,3}, 10, 0), Fourmi(Coord{4,4}, 11, 0)};
    CHECK(c.nbFourmis() == vf.size());
    
    // nbFourmis
    CHECK_FALSE(c.nbFourmis() == 11);
    CHECK(c.nbFourmis() == 12);
    CHECK_FALSE(c.nbFourmis() == 13);
    
    // nbFourmisVivantes
    CHECK_FALSE(c.nbFourmisVivantes() == 11);
    CHECK(c.nbFourmisVivantes() == 12);
    CHECK_FALSE(c.nbFourmisVivantes() == 13);
}
// END: tests getters Colonie

// BEGIN: operators
ostream &operator<<(ostream &out, const Colonie &col) {
    out << "Colonie n°" << col.numero() << " qui commence a la coord : " << col.coords().getEns()[0] << " possède " << col.nbFourmis() << "fourmis et " << col.stockSucre() << " morceaux de sucre ";
    return out;
}

bool Colonie::operator==(const Colonie &col2) const {
    return (coordsNid_ == col2.coords() and numero_ == col2.numero() and stockSucre_ == col2.stockSucre() and fourmis_ == col2.tabFourmi() and nbFourmis() == col2.nbFourmis());
}

bool Colonie::operator!=(const Colonie &col2) const {
    return not(*this == col2);
}
// END: operators

// BEGIN: tests operators
TEST_CASE("operators") {
    Colonie col1 = Colonie(0, Coord{5,3});
    Colonie col2 = Colonie(0, Coord{5,3});
    Colonie col3 = Colonie(0, Coord{3,5});
    Colonie col4 = Colonie(1, Coord{5,3});
    
    CHECK(col1 == col2);
    CHECK_FALSE(col1 == col3);
    CHECK_FALSE(col1 == col4);
}
// END: tests operators

// BEGIN: setters Colonie
void Colonie::majTabFourmi(vector<Fourmi> tab) {
    fourmis_ = tab;
}

void Colonie::naissanceFourmi() {
    if(NAISSANCE and stockSucre_ >= STOCKSUCREPOURNAISSANCE and nbFourmisVivantes() < nbMaxFOURMIS) {
        Coord c1 = coordsNid_.getEns()[0];
        Coord c2 = Coord{c1.getLig(), c1.getCol()-1};
        numDernierefourmi_++;
        Fourmi f = Fourmi(c2, numDernierefourmi_, numero_);        
        fourmis_.push_back(f);
        
        stockSucre_ -= STOCKSUCREPOURNAISSANCE;
    }
}

void Colonie::ajouteSucre(int qte) {
    stockSucre_ += qte;
}

void Colonie::tueFourmi(int numFourmi) {
    fourmis_[numFourmi].doitMourir();
}
// END: setters Colonie

// BEGIN: tests setters Colonie
TEST_CASE("majTabFourmi") {
    Colonie col1 = Colonie(0, Coord{2,2});
    vector<Fourmi> vf = {Fourmi(Coord{1,1}, 0, 0), Fourmi(Coord{1,2}, 1, 0), Fourmi(Coord{1,3}, 2, 0), Fourmi(Coord{2,1}, 3, 0), Fourmi(Coord{3,1}, 4, 0), Fourmi(Coord{1,4}, 5, 0), Fourmi(Coord{2,4}, 6, 0), Fourmi(Coord{3,4}, 7, 0), Fourmi(Coord{4,1}, 8, 0), Fourmi(Coord{4,2}, 9, 0), Fourmi(Coord{4,3}, 10, 0), Fourmi(Coord{4,4}, 11, 0)};
    vector<Fourmi> vf2 = {Fourmi(Coord{2,2}, 0, 0)};
    
    CHECK(col1.tabFourmi() == vf);
    col1.majTabFourmi(vf2);
    CHECK(col1.tabFourmi() == vf2);
}

TEST_CASE("naissanceFourmi") {
    Colonie col1 = Colonie(0, Coord{2,2});
    vector<Fourmi> vf = {Fourmi(Coord{1,1}, 0, 0), Fourmi(Coord{1,2}, 1, 0), Fourmi(Coord{1,3}, 2, 0), Fourmi(Coord{2,1}, 3, 0), Fourmi(Coord{3,1}, 4, 0), Fourmi(Coord{1,4}, 5, 0), Fourmi(Coord{2,4}, 6, 0), Fourmi(Coord{3,4}, 7, 0), Fourmi(Coord{4,1}, 8, 0), Fourmi(Coord{4,2}, 9, 0), Fourmi(Coord{4,3}, 10, 0), Fourmi(Coord{4,4}, 11, 0)};
    
    CHECK(col1.tabFourmi() == vf);
    vf.push_back(Fourmi(Coord{2,1}, 12, 0));
    col1.ajouteSucre(11);
    col1.naissanceFourmi();
    CHECK(col1.tabFourmi() == vf);
}

TEST_CASE("ajouteSucre") {
    Colonie col1 = Colonie(0, Coord{5,3});
    
    CHECK(col1.stockSucre() == 0);
    col1.ajouteSucre(5);
    CHECK(col1.stockSucre() == 5);
    col1.ajouteSucre(10);
    CHECK(col1.stockSucre() == 15);
}
// END: tests setters Colonie

#include <iostream>
#include <vector>

using namespace std;
//#define DOCTEST_CONFIG_DISABLE
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"


//#include "coord.hpp"
#include "grille.hpp"

// BEGIN: Procédures main

//* Initialise les emplacements des fourmis, du sucre et du nid
//* @param[out] leSucre : Tableau qui contient les coordonnées du sucre
//* @param[out] lesColonies : Le tableau des conlonies de fourmis
void initialiserEmplacements(EnsCoord &leSucre, vector<Colonie> &lesColonies) {
    
    // Prépare l'emplacement du sucre 
    vector<Coord> vSucre = {Coord{1,1}, Coord{1,2}, Coord{2,1}, Coord{2,2}, Coord{TAILLEGRILLE-2,TAILLEGRILLE-2}, Coord{TAILLEGRILLE-2,TAILLEGRILLE-3}, Coord{TAILLEGRILLE-3,TAILLEGRILLE-2}, Coord{TAILLEGRILLE-3,TAILLEGRILLE-3}, Coord{(TAILLEGRILLE/2)-1,(TAILLEGRILLE/2)-1}, Coord{(TAILLEGRILLE/2),(TAILLEGRILLE/2)}, Coord{(TAILLEGRILLE/2)-1,(TAILLEGRILLE/2)}, Coord{(TAILLEGRILLE/2),(TAILLEGRILLE/2)-1}};
    leSucre = EnsCoord(vSucre);
    
    // Un seul nid au milieu de la grille
    //lesColonies = {Colonie(0, Coord{(TAILLEGRILLE/2)-1,(TAILLEGRILLE/2)-1})};
    
    // Deux nids en haut a droite et en bas a gauche de la grille
    lesColonies = {Colonie(0, Coord{3,TAILLEGRILLE-5}), Colonie(1, Coord{TAILLEGRILLE-5,3})};
    
    // Trois nids dans la grille
    //lesColonies = {Colonie(0, Coord{3,TAILLEGRILLE-5}), Colonie(1, Coord{TAILLEGRILLE-5,3}), Colonie(2, Coord{TAILLEGRILLE-5, TAILLEGRILLE/2})};
}

// condition n°1
//* Retourne vrai si la fourmi peut tuer une autre fourmi
//* @param[in] f : la fourmi que l'on veut déplacer
//* @param[in] p2 : Enventuelle place ou se trouve une autre fourmi
//* @return : Vrai si la fourmi peut tuer
bool conditionPeutTuerFourmi(Fourmi  f, Place p2) {
    return f.estEnVie() and p2.numeroFourmi() != -1 and f.colonie() != p2.numeroColonie();
}

//* Fait tuer une autre fourmi
//* @param[in/out] p2 : La place de la seconde fourmi
//* @param[in/out] lesColonies : Le tableau des conlonies de fourmis
void tuerFourmi(Place &p2, vector<Colonie> &lesColonies) {
    lesColonies[p2.numeroColonie()].tueFourmi(p2.numeroFourmi());
    p2.enleveFourmi();
}

// condition n°2
//* Retourne vrai si la fourmi peut charger du sucre
//* @param[in] f : la fourmi
//* @param[in] p2 : Enventuelle place ou se trouve du sucre
//* @return : Vrai si la fourmi peut charger du sucre
bool conditionPeutChargerSucre(Fourmi  f, Place p1, Place p2) {
    return f.chercheSucre() and p2.contientSucre();
}

//* Fait transporter du sucre a la fourmi
//* @param[in/out] f : La fourmi qui souhaite obtenir du sucre
//* @param[in/out] p1 : Place de la fourmi f
//* @param[in/out] p2 : Place contenant du sucre
void chargerSucre(Fourmi &f, Place &p1, Place &p2) {
    f.prendSucre();
    p2.enleveSucre();
    p1.posePheroSucre(255, f.colonie());
}

// condition n°3
//* Retourne vrai si la fourmi peut poser son sucre
//* @param[in] f : La fourmi qui souhaite poser du sucre
//* @param[in] p2 : Enventuelle place ou se trouve un nid
//* @return : Vrai si la fourmi peut poser du sucre
bool conditionPeutPoserSucre(Fourmi  f, Place p2) {
    return f.porteSucre() and p2.contientNid();
}

//* Fait poser le sucre de la fourmi
//* @param[in/out] f : la fourmi qui veut poser du sucre
//* @param[in/out] lesColonies : Le tableau des conlonies de fourmis
void poserSucre(Fourmi &f, vector<Colonie> &lesColonies) {
    f.poseSucre();
    lesColonies[f.colonie()].ajouteSucre(1);
}

// condition n°4
//* Retourne vrai si la fourmi peut se déplacer
//* @param[in] f : la fourmi qui veut rapporter du sucre
//* @param[in] p1 : Place de la fourmi f
//* @param[in] p2 : Nouvelle place voulu pour la fourmi f
//* @return : Vrai si la fourmi peut rapporter du sucre
bool conditionPeutRapporterSucre(Fourmi  f, Place p1, Place p2) {
    return f.porteSucre() and p2.estVide() and p2.estPlusProcheNid(p1, f.colonie());
}

//* Fait se déplacer la fourmi
//* @param[in/out] f : la fourmi que l'on veut déplacer
//* @param[in/out] p1 : Place de la fourmi f
//* @param[in/out] p2 : Nouvelle place voulu pour la fourmi f
void rapporterSucre(Fourmi &f, Place &p1, Place &p2) {
    deplace(f, p1, p2);
    p2.posePheroSucre(255, f.colonie());
}

// condition n°5
//* Retourne vrai si la fourmi peut se déplacer
//* @param[in] f : la fourmi que l'on veut déplacer
//* @param[in] p1 : Place de la fourmi f
//* @param[in] p2 : Nouvelle place voulu pour la fourmi f
//* @return : Vrai si la fourmi peut se déplacer
bool conditionPeutSeDeplacer1(Fourmi  f, Place p1, Place p2) {
    return f.chercheSucre() and p1.estSurUnePiste(f.colonie()) and p2.estVide() and p2.estPlusLoinNid(p1, f.colonie()) and p2.estSurUnePiste(f.colonie());
}

// condition n°6
//* Retourne vrai si la fourmi peut se déplacer
//* @param[in] f : la fourmi que l'on veut déplacer
//* @param[in] p2 : Nouvelle place voulu pour la fourmi f
//* @return : Vrai si la fourmi peut se déplacer
bool conditionPeutSeDeplacer2(Fourmi  f, Place p2) {
    return f.chercheSucre() and p2.estSurUnePiste(f.colonie()) and p2.estVide();
}

// condition n°7
//* Retourne vrai si la fourmi peut se déplacer
//* @param[in] f : la fourmi que l'on veut déplacer
//* @param[in] p2 : Nouvelle place voulu pour la fourmi f
//* @return : Vrai si la fourmi peut se déplacer
bool conditionPeutSeDeplacer3(Fourmi  f, Place p2) {
    return f.chercheSucre() and p2.estVide();
}

//* Fait se déplacer la fourmi
//* @param[in/out] f : la fourmi que l'on veut déplacer
//* @param[in/out] p1 : Place de la fourmi f
//* @param[in/out] p2 : Nouvelle place voulu pour la fourmi f
void seDeplacer(Fourmi &f, Place &p1, Place &p2) {
    deplace(f, p1, p2);
}


//* Retourne Vrai si la condition de déplacement de la règle numéro x s’applique sur la fourmi f, située sur la place p1, pour un déplacement vers la place p2
//* @param[in] x : Numéro de la règle
//* @param[in] f : la fourmi a déplacer
//* @param[in] p1 : Place de la fourmi f
//* @param[in] p2 : Nouvelle place voulu pour la fourmi f
bool condition_n(int x, Fourmi  f, Place p1, Place p2) {
    switch(x) {
        case 1: return conditionPeutTuerFourmi(f, p2);
        case 2: return conditionPeutChargerSucre(f, p1, p2);
        case 3: return conditionPeutPoserSucre(f, p2);
        case 4: return conditionPeutRapporterSucre(f, p1, p2);
        case 5: return conditionPeutSeDeplacer1(f, p1, p2);
        case 6: return conditionPeutSeDeplacer2(f, p2);
        case 7: return conditionPeutSeDeplacer3(f, p2);
    }
}

//* Applique l’action de déplacement de la règle numéro x sur la fourmi f, située sur la place p1, pour un déplacement vers la place p2
//* @param[in] x : Numéro de la règle
//* @param[in/out] f : la fourmi a déplacer
//* @param[in/out] p1 : Place de la fourmi f
//* @param[in/out] p2 : Nouvelle place voulu pour la fourmi f
void action_n(int x, Fourmi  &f, Place &p1, Place &p2, vector<Colonie> &lesColonies) {
    switch(x) {
        case 1: tuerFourmi(p2, lesColonies);
            return;
        case 2: chargerSucre(f, p1, p2);
            return;
        case 3: poserSucre(f, lesColonies);
            return;
        case 4: rapporterSucre(f, p1, p2);
            return;
        case 5: seDeplacer(f, p1, p2);
            return;
        case 6: seDeplacer(f, p1, p2);
            return;
        case 7: seDeplacer(f, p1, p2);
            return;
    }
}


//* Déplace une fourmi en appliquant les règles de comportement des fourmis
//* @param[in/out] laGrille : la grille
//* @param[in/out] uneFourmi : la fourmi a déplacer
void mettreAJourUneFourmi(Grille  &laGrille, Fourmi  &uneFourmi, vector<Colonie> &lesColonies) {
    Coord cf = uneFourmi.coord();
    Place pf = laGrille.chargePlace(cf);
    EnsCoord voiscoord = voisines(cf);
    for(int regle = 1; regle <= 7; regle++){
        for(Coord cv : voiscoord.getEns()) {
            Place vois = laGrille.chargePlace(cv);
            
            if(uneFourmi.estEnVie() and condition_n(regle, uneFourmi, pf, vois)) {
                if(regle >= 4) {
                    Place pAlea = laGrille.chargePlace(laGrille.voisinVideAleatoire(pf));
                    action_n(regle, uneFourmi, pf, pAlea, lesColonies);
                    laGrille.rangePlace(pf);
                    laGrille.rangePlace(pAlea);
                }
                else {
                    action_n(regle, uneFourmi, pf, vois, lesColonies);
                    laGrille.rangePlace(pf);
                    laGrille.rangePlace(vois);
                }
                return;
            }
        }
    }
}


//* Déplace toutes les fourmis en appliquant les règles de leur comportement
//* @param[in/out] laGrille : la grille
//* @param[in/out] lesFourmis : tableau des fourmis
//void mettreAJourEnsFourmis(Grille  &laGrille, vector<Fourmi>  &lesFourmis) {
void mettreAJourEnsFourmis(Grille  &laGrille, vector<Colonie> &lesColonies) {
    //Génére une duplication des fourmis
    /*for(Fourmi f : lesFourmis) {
        mettreAJourUneFourmi(laGrille, f);
    }*/

    for(unsigned int i = 0; i < lesColonies.size(); i++) {
        vector<Fourmi> lesFourmis = lesColonies[i].tabFourmi();
        for(unsigned int j = 0; j < lesFourmis.size(); j++) {
            mettreAJourUneFourmi(laGrille, lesFourmis[j], lesColonies);
            lesColonies[i].majTabFourmi(lesFourmis);
            lesColonies[i].naissanceFourmi();
            lesFourmis = lesColonies[i].tabFourmi();
        }
        
        /* // Ne fait naitre une fourmi que si toutes les fourmis se sont déplacées
        lesColonies[i].majTabFourmi(lesFourmis);
        lesColonies[i].naissanceFourmi();*/
        
    }
}
// END: Procédures main

int main(int argc, const char **argv) {
    Grille g = Grille();
    EnsCoord vSucre = EnsCoord({});
    vector<Colonie> vColonie = {};
    int tourSucre = 0;
    
    initialiserEmplacements(vSucre, vColonie);
    g.initialiseGrille(vSucre, vColonie);
    
    
    //g.afficheNumFourmis();
    for(int i = 0; i < 300; i++) {
        if(tourSucre == toursSucreAlea) {
            g.ajouteSucreAlea();
            tourSucre = 0;
        }
        tourSucre++;
        
        mettreAJourEnsFourmis(g, vColonie);
        controleGrille(g, vColonie);
        g.dessinerGrille(vColonie);
        
        
        //cout << g << endl;
        //g.afficheNumFourmis();
        //g.afficheSucre();
        
        g.diminuePheroSucre();
        //g.affichePheroSucre();
    }
    
    cout << g << endl;
    //g.afficheNumFourmis();
    
    for(Colonie col : vColonie) {
        cout << "Nb fourmis colonie n°" << col.numero() << " : " << col.nbFourmis() << " dont vivantes : " << col.nbFourmisVivantes() << endl;
        for(Fourmi f : col.tabFourmi()) {
            cout << f << endl;
        }
    }
    
    return 0;
}
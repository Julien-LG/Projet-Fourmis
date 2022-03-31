#include<array>
#include <vector>

#include "place.hpp"

const int toursSucreAlea = 20;

class Grille {

 public:

    //* Constructeur par défaut
    //* @param[out] g : la nouvelle Grille
    Grille ();
    
    //* Renvoie la grille de Grille
    //* @return : La grille
    vector<vector<Place>> getGrille() const;
    
    //* Renvoie le tableau des Coord de la grille
    //* @return : Le tableau des Coord
    vector<Coord> getCoords() const;

    //* Renvoie une Copie de la place
    //* @param[in] c : Coord de la place voulu
    //* @return : la place voulu
    Place chargePlace(Coord c) const;
  
    
    //* Range la place dans la grille après l’avoir modifiée
    //* @param[in] p : La place modifiée 
    void rangePlace(Place p);    
  
    //* Place le nid dans la grille
    //* @param[in] v : contient l'ensemble des coordonnées du nid
    //* @param[in] numColonie : num de la colonie a qui appartient le nid
    void placeNid(EnsCoord v, int numColonie);
    
    //* Place le sucre dans la grille
    //* @param[in] v : contient l'ensemble des coordonnées du sucre
    void placeSucre(EnsCoord ensSucre);
    
    //* Place les fourmi dans la grille
    //* @param[in] v : tableau qui contient les fourmis
    void placeFourmi(vector<Fourmi> v);
    
    //* Place les fourmi dans la grille
    //* @param[in] vSucre : tableau qui contient les coordonnées du sucre
    //* @param[in] colonies : tableau qui contient les colonies de fourmis
    void initialiseGrille(EnsCoord vSucre, vector<Colonie> colonies);
    
    //* Linéarise l’intensité en phéromones de nid des cases de la grille
    //* @param[in] numColonie : num de la colonie a qui appartient les phéromones
    void linearisePheroNid(int numColonie);
    
    //* Linéarise l’intensité en phéromones de nid des cases de la grille en utilisant distance euclidienne
    //* @param[in] col : colonie a qui appartient les phéromones
    void linearisePheroNid2(Colonie col);
    
    //* Diminue suite à l’évaporation les phéromones de sucre sur toute les cases de la grille
    void diminuePheroSucre();
  
    
    //* Affiche la grille avec les pheromones de Nid
    //* @param[in] numColonie : num de la colonie a qui appartient les phéromones
    void affichePheroNid(int numColonie);
    
    //* Affiche la grille avec les pheromones de Sucre
    //* @param[in] numColonie : num de la colonie a qui appartient les phéromones
    void affichePheroSucre(int numColonie);
    
    //* Affiche la grille avec les num des fourmis
    void afficheNumFourmis();
    
    //* Affiche la grille avec les quantités de sucre
    void afficheSucre();
    
    //*  Retourne les coordonnées dans la grille g d’un voisin vide de p choisi aléatoirement
    //* @param[in] p : La place initial
    //* @return : Une coord voisine vide aléatoire
    Coord voisinVideAleatoire(Place p);
    
    //*  Ajoute un morceau de sucre sur une place vide aléatoire
    void ajouteSucreAlea();
    
    //* * Dessine une image de la grille dans un fichier
    //* @param[in] laGrille : La Grille
    //* @param[in] lesColonies : Le tableau des conlonies de fourmis
    void dessinerGrille(vector<Colonie> lesColonies);
    
    
    
    bool operator==(const Grille &g2) const;

    bool operator!=(const Grille &g2) const;
    


 private:
    vector<vector<Place>> grille_;
};

ostream &operator<<(ostream &out, const Grille &g);

//* Retourne une erreur si l'intégrité de la grille n'est pas bonne.
void controleGrille(Grille g, vector<Colonie> colonies);


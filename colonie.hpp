#include "fourmi.hpp"

// Nombre de colonies que l'on va utiliser
const int NOMBRECOLONIES = 2;
// Nombre maximum de fourmis en vie dans chaque colonies
const int nbMaxFOURMIS = 20;
// True si on autorise la naissance de nouvelles fourmis
const bool NAISSANCE = true;
// Stock de sucre necessaire pour qu'une fourmi naisse
const int STOCKSUCREPOURNAISSANCE = 5;

class Colonie {

 public:

    //* Constructeur par défaut
    //* @param[in] num : numero de la colonie
    //* @param[in] c : Coord sur la grille  
    //* @param[out] f : la Colonie de fourmis
    Colonie (int num, Coord c);

    
    //* Renvoie l'ensemble des coords du Nid
    //* @return : L'ensemble des coords du nid
    EnsCoord coords() const;
    
    //* Renvoie le numéro de la colonie
    //* @return : numéro de la colonie
    int numero() const;
  
    //* Renvoie la quantité de sucre de la colonie
    //* @return : quantité de sucre
    int stockSucre() const;
    
    //* Renvoi le tableau de fourmis de la colonie
    //* @return : Le tableau de fourmis
    vector<Fourmi> tabFourmi() const;
    
    //* Renvoi le nombre de fourmis de la colonie
    //* @return : Le nombre de fourmis
    int nbFourmis() const;
    
    //* Renvoi le nombre de fourmis de la colonie encore en vie
    //* @return : Le nombre de fourmis en vie
    int nbFourmisVivantes() const;
    
    
    //* Met a jour le tableau de fourmi avec celui donnée en paramètre
    //* @param[in] tab : Le nouveau tableau de fourmi
    void majTabFourmi(vector<Fourmi> tab);
    
    //* Ajoute automatiquement une nouvelle fourmi si les conditions le permettent
    void naissanceFourmi();
    
    //* Ajoute du sucre dans le stock de sucre
    //* @param[in] qte : la quantité de sucre a ajouter
    void ajouteSucre(int qte);
    
    //* Tue la fourmi correspondant au parametre
    //* @param[in] numFourmi : le numero de la fourmi a tuer
    void tueFourmi(int numFourmi);
  
  
    bool operator==(const Colonie &col2) const;

    bool operator!=(const Colonie &col2) const;
    


 private:
    
    int numero_;
    vector<Fourmi> fourmis_;
    EnsCoord coordsNid_;
    int stockSucre_;
    int numDernierefourmi_;
};

ostream &operator<<(ostream &out, const Colonie &col);

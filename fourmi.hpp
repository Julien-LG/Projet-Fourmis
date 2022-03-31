#include "coord.hpp"

class Fourmi {

 public:

  //* Constructeur par défaut
  //* @param[in] c : Coord sur la grille
  //* @param[in] num : Le numero de la fourmi qui correspond a son indice dans le tableau
  //* @param[in] colonie : Le numero de la colonie de la fourmi
  //* @param[out] f : La fourmi
  Fourmi (Coord c, int num, int colonie);

  //* Renvoie les coordonnées d’une fourmi
  //* @return : Les coord de la fourmi
  Coord coord() const;
  
  
  //* Renvoie le numéro de la fourmi
  //* @return : Le numéro de la fourmi
  int numero() const;
    
  //* Renvoie le numéro de la colonie de la fourmi
  //* @return : Le numéro de la colonie de la fourmi
  int colonie() const;
  
  //* Renvoie vrai si la fourmi rentre au nid en portant du sucre
  //* @return : Vrai si la fourmi porte du sucre
  bool porteSucre() const;
    
  //* Renvoie vrai si la fourmi cherche du sucre
  //* @return : Vrai si la fourmi ne porte pas de sucre
  bool chercheSucre() const;
    
  //* Renvoie vrai si la fourmi est vivante
  //* @return : vrai si elle est en vie
  bool estEnVie() const;
    
    
  //* Ajoute une charge de sucre sur la fourmi
  void prendSucre();
    
  //* Supprime la charge de sucre portée par la fourmi
  void poseSucre();
    
  //* La fourmi meurt
  void doitMourir();
    
  //* Déplace une fourmi sur une nouvelle coord
  //* @param[in] c : Coord sur la grille
  void deplace(Coord c);
  
  
  bool operator==(const Fourmi &f2) const;

  bool operator!=(const Fourmi &f2) const;
    


 private:
    
    Coord coord_;
    bool porteSucre_;
    int numero_;
    int colonie_;
    bool enVie_;
};

ostream &operator<<(ostream &out, const Fourmi &f);

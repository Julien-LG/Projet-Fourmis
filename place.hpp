#include "colonie.hpp"

class Place {

 public:

  //* Constructeur par défaut
  //* @param[in] c : Coord de la Place
  //* @param[out] p : la nouvelle Place
  Place (Coord c);

    
  //* Renvoie les coordonnées de la place
  //* @return : La coord de la place
  Coord coord() const;
    
  //* Renvoie l’intensité en phéromone de sucre de la place
  //* @param[in] colonie : Le numero de colonie dont on veut les pheroSucre
  //* @return : L'intensité en phéromone de sucre
  int pheroSucre(int colonie) const;
  
  //* Renvoie l’intensité en phéromone de nid de la place
  //* @param[in] colonie : Le numero de colonie dont on veut les pheroNid
  //* @return : intensité en phéromone de nid 
  float pheroNid(int colonie) const;
    
  //* Renvoie le numéro de la fourmi sur la place
  //* @return : le numéro de la fourmi, ou −1 si pas de fourmi
  int numeroFourmi() const;
    
  //* Renvoie le numéro de colonie de la fourmi sur la place
  //* @return : le numéro de colonie de la fourmi, ou −1 si pas de fourmi
  int numeroColonie() const;
    
  //* Renvoie la quantitée de sucre sur la place
  //* @return : La quantitée de sucre
  int sucre() const;

    
  //* Renvoie vrai si la place contient du sucre
  //* @return : vrai si la place contient du sucre
  bool contientSucre() const;
    
  //* Renvoie vrai si la place contient un élément de nid
  //* @return : vrai si la place contient un élément de nid
  bool contientNid() const;

  //* Renvoie vrai si la place se trouve sur une piste vers du sucre
  //* @param[in] colonie : Le numero de colonie dont on veut les pheroSucre
  //* @return : vrai si la place possede une pheromone de sucre
  bool estSurUnePiste(int colonie) const;
  
  //* Renvoie vrai si la place ne contient ni sucre, ni élément de nid, ni fourmi
  //* @return : vrai si la place est vide
  bool estVide() const;
    
  //* Renvoie vrai si la place donnée en 1er paramètre est plus proche du nid que la place donnée en 2e paramètre
  //* @param[in] p2 : La place avec laquelle on veut comparer
  //* @param[in] colonie : Le numero de colonie
  //* @return : Vrai si p1 est plus proche du nid que p2
  bool estPlusProcheNid(Place &p2, int colonie);
    
  //* Renvoie vrai si la place donnée en 1er paramètre est plus loin du nid que la place donnée en 2e paramètre
  //* @param[in] p2 : La place avec laquelle on veut comparer
  //* @param[in] colonie : Le numero de colonie
  //* @return : Vrai si p2 est plus proche du nid que p1
  bool estPlusLoinNid(Place &p2, int colonie);
  
    
  //* Pose du sucre sur la place
  void poseSucre();
    
  //* Enlève du sucre de la place
  void enleveSucre();
      
  //* Pose un élément de nid sur la place
  void poseNid(int colonie);
      
  //* Pose la fourmi donnée en paramètre sur la place
  //* @param[in] f : La fourmi que l'on veut poser
  void poseFourmi(Fourmi f);
      
  //* Enlève la fourmi de la place
  void enleveFourmi();
      
  //* Pose sur la place une phéromone de nid d’intensité donnée
  //* @param[in] qte : La quantité de phero a poser
  //* @param[in] colonie : Le numero de colonie dont on veut poser une pheroNid
  void posePheroNid(float qte, int colonie);
      
  //* Pose une phéromone de sucre d’intensité maximale sur la place
  //* @param[in] qte : La quantité de phero a poser
  //* @param[in] colonie : Le numero de colonie dont on veut poser une pheroSucre
  void posePheroSucre(int qte, int colonie);
      
  //* Diminue (par évaporation) l’intensité en phéromone de sucre sur la place
  void diminuePheroSucre();
    
      
  
  bool operator==(const Place &p2) const;

  bool operator!=(const Place &p2) const;
    


 private:
    
    Coord coord_;
    int numFourmi_;
    int numColonieFourmi_;
    int sucre_;
    int nid_;
    vector<int> pheroSucre_;
    vector<float> pheroNid_;
};

ostream &operator<<(ostream &out, const Place &p);

//* Déplace la fourmi donnée en 1er paramètre qui est sur la place donnée en 2e paramètre vers la place donnée en 3e paramètre
//* @param[in] f : La fourmi a deplacer
//* @param[in] p1 : La place actuelle de la fourmi
//* @param[in] p2 : La futur place de la fourmi
void deplace(Fourmi &f, Place &p1, Place &p2);

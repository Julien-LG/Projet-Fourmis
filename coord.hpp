const int TAILLEGRILLE = 30;

class Coord {

 public:

    //* Constructeur par défaut
    //* @param[in] lig : entier de la ligne
    //* @param[in] col : entier de la colonne
    //* @param[out] c : La nouvelle coord
    Coord (int lig, int col);

    //* Ajoute un objet à un ensemble
    //* @return : la valeur de ligne
    int getLig() const;
    
    //* Ajoute un objet à un ensemble
    //* @return : La valeur de colonne
    int getCol() const;
  
    bool operator==(const Coord &c2) const;

    bool operator!=(const Coord &c2) const;
  
    
    //* Calcul la distance euclidienne entre deux coord
    //* @param[in] c2 : La deuxieme coord
    //* @return : La distance euclidienne
    float euclideanDistance(const Coord c2) const;
    

 private:

    int numLigne_;
    int numColonne_;
};

class EnsCoord {

 public:

    //* Constructeur par défaut
    //* @param[in] ensemble : un vecteur de Coord
    //* @param[out] e : Un ensemble de Coord
    EnsCoord (vector<Coord> ensemble);
    
    //* Renvoie l'ensemble des coord d'ensemble_
    //* @return : Le vector de coord d'ensemble_
    vector<Coord> getEns() const;   
    
    //* Renvoie vrai si la Coord est présente dans l'EnsCoord
    //* @param[in] c : une Coord
    //* @return : Booléen qui indique si la Coord est présente
    bool contient(Coord c) const;
    
    //* Ajoute une coord a EnsCoord
    //* @param[in] c : La Coord a ajouter
    void ajoute(Coord c);
    
    //* Supprime une coord a EnsCoord
    //* @param[in] c : La Coord a supprimer
    void supprime(Coord c);
    
    //* Indique si l'EnsCoord est vide
    //* @return : Vrai si l'ensemble de coord est vide
    bool estVide();
    
    //* Indique la taille de l'EnsCoord
    //* @return : Un entier qui vaut la taille
    int taille() const;
    
    //* Retourne la coord qui est à la position n dans l'EnsCoord
    //* @param[in] n : position n de la coord voulu
    //* @return : la Coord a la position n
    Coord ieme(int n) const;
    
    //* Retourne une coordonnée au hasard parmi un ensemble de coordonnées
    //* @return : la Coord tirée au hasard
    Coord choixHasard();
    
    bool operator==(const EnsCoord &e2) const;

    bool operator!=(const EnsCoord &e2) const;
    

 private:

    vector<Coord> ensemble_;
    
    //* Renvoie la position d'une Coord de l'EnsCoord
    //* @param[in] c : Une Coord
    //* @return : La position de la Coord dans EnsCoord (-1 s'il n'y a pas de position)
    int position(Coord c) const;
    
};

ostream &operator<<(ostream &out, const Coord &coord);
ostream &operator<<(ostream &out, const EnsCoord &ensCoord);

//* Retourne un EnsCoord des coordonnées voisines à c
//* @param[in] c : La coordonnée dont on veut les voisins
//* @return : L'ensemble des coord voisines de c
EnsCoord voisines(Coord c);

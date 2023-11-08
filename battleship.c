#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "battleship.h"

/**
 * Le nom des bateaux sous forme de chaînes de caractères immutables.
 * Les entrées du tableau coïncident avec les valeurs de l'`enum ship_kind`
 */
static const char* const ship_labels[] = {
    "Destroyer", "Submarine", "Cruiser", "Battleship", "Carrier"
};


/**
 * Affiche une grille de bataille navale de dimension `board`, en affichant
 * la bateaux (tableau `fleet` de taille `fleet_size`).
 *
 * Le `#` représente une case occupée par un bateau, et le `X` une case
 * touchée.
 *
 */
void print_grid(struct rules rules,
        int fleet_size, const struct ship fleet[], struct dimension board)
{
int b = (board.width) * (board.height);
char tab[b];  // creation d'un tableau de vagues (~)
for (int i = 0; i < b; i++)
{
  tab[i] = '~';
}
for (int n = 0; n < board.width ; ++n){
    for (int m = 0; m < board.height; ++m){
      for(int a = 0; a < fleet_size; ++a){
        struct ship ship = fleet[a];
        if ((ship.position.x == m) && (ship.position.y == n)){
          int d = ship_size(rules, ship);
          if (ship.orientation == 1){   // verif de l'orientation du bateau pour l'afficher
            for (int i = 0; i < d; i++)
            {
             if (ship.state[i] == 1)  // controle de l'état du bateau touché ou non
             {
             tab[n+ i +m*10] = 'X';}
             else
             {
             tab[n+ i +m*10] = '#';}
            }}
          else{
            for (int i = 0; i < d; i++)
            {
            if (ship.state[i] == 1)  // controle de l'état du bateau touché ou non
             {
             tab[n+(m+i)*10] = 'X';}
             else
             {
             tab[n+(m+i)*10] = '#';}
            }
          }        
           }
      }
      printf("%c ",tab[n + m*10]);}   // affichage d'une partie du tableau qui correspond à 1 ligne.
printf("\n");
}
}
/**
 * Retourne la somme des `n` premiers éléments du tableau d'entier `array`.
 * Le tableau est supposé contenir au moins `n` éléments
 */
int sum(int n, const int array[])
{
  int b = 0;
  for(int a = 0; a < n; ++a)
    b = b + array[a];
  return b;
}

/**
 * Retourne vrai (1) si la position `pos` appartient au rectangle dont
 * le coin supérieur gauche est `origin` de dimension `dim`. Cette fonction
 * retourne faux (0) sinon.
 *
 * Exemples:
 * - Vrai: pos={1, 2}, origin={0, 0}, dim={3, 3}
 * - Faux: pos={4, 3}, origin={1, 0}, dim={3, 3}
 */
int inside(struct position pos, struct position origin, struct dimension dim)
{
  return ((pos.x >= origin.x) * (pos.y >= origin.y) * ((dim.width + origin.x) > pos.x)
          * ((dim.height + origin.y) > pos.y)); // Fait un ET logique entre la position en X et Y
}

/**
 * Contraint la position `pos` aux dimensions `dim`.
 * Si la position y appartient déjà elle est inchangée, sinon elle prendra
 * la valeur de la position valide la plus proche.
 *
 * Exemple:
 * - pos={7,3}, dim={4,4} -> pos devient {3,3}
 */
void constrain(struct position *pos, struct dimension dim)
{
  if (pos->x  >= dim.width)
    pos->x = dim.width - 1;
  if (pos->x < 0)
    pos->x = 0 ;
  if (pos->y >= dim.height)
    pos->y = dim.height - 1;
   if (pos->y < 0)
    pos->y = 0 ;

 
       
    
}

/**
 * Étant donné une position `origin` désignant le coin supérieur
 * gauche du bateau, et une position `pos` désignant une case du
 * bateau, détermine l'indice de la case correspondant à cette
 * position dans le tableau `state` du bateau.
 *
 * RQ : il n'y a pas besoin de savoir si le bateau est horizontal ou
 * vertical pour écrire cette fonction.
 *
 * Exemples:
 * - pos={2,3}, origin={0,3} => 2
 * - pos={0,4}, origin={0,3} => 1
 */
int offset(struct position pos, struct position origin){
  int a = abs(pos.x + pos.y - origin.x - origin.y);
  return a;
}

/**
 * Retourne la taille du bateau `ship` (en nombre de cases) en fonction d'une
 * règle du jeu.
 *
 * EX (Avec des règles par défaut): CARRIER => 5
 */
int ship_size(struct rules rules, struct ship ship)
  {
int a = ship.kind;
int b = rules.ships_size[a];
return b;
  }

/**
 * Retourne une dimension représentant la taille du bateau `ship` et son
 * orientation.
 *
 * RQ: Tous les bateaux ne font qu'une case de large
 * EX (Avec des règles par défaut): CARRIER VERTICAL => {1, 5}
 */
struct dimension ship_dimension(struct rules rules, struct ship ship) { 
struct dimension local;
local.width = ship.orientation;
local.height = ship_size(rules, ship);
return local;
};

/**
 * Retourne le nombre de points de vie restant au bateau `ship`.
 * Si ce nombre est 0, on dit que le bateau est coulé.
 */
int remaining_life(struct rules rules, struct ship ship) 
{
int a = rules.ships_size[ship.kind];
for(int i = 0; i == a; --i){
a = a - ship.state[i];
}
return a;
}

/**
 * Affiche la position `pos` sous la forme YX
 * ou Y (ordonnée) est une lettre entre `A` et `Z`
 * et X (abscisse) est un chiffre entre `0` et `9`
 */
void print_position(struct position pos) 
{
pos.x = abs(pos.x);
pos.y = abs(pos.y);
int n = pos.x;
int m = pos.y;
int a = 0;
if (n  > 25){
  n = pos.x % 26;
  }
if( m > 9){
  a = pos.y/10 ;
  m = pos.y % 10;
}
pos.x = n + 'A';
pos.y = m + '0';
printf("%C %d%C\n", pos.x , a, pos.y);
}

/**
 * Affiche le bateau `ship` sous la forme:
 * Type (PDV_RESTANT/TAILLE) POS_SUPGAUCHE-POS_INFDROIT
 * suivi d'un saut de ligne !
 *
 * Ex: Carrier (3/5) E4-E8
 * RQ: Les positions sont affichées sous la forme décrite par `print_position`
 * RQ: Vous pouvez utiliser le tableau ship_labels pour afficher le type.
 * RQ: Le comportement de cette fonction est indéfini si le bateau n'est pas
 *     valide (i.e., pas de piège)
 */
void print_ship(struct rules rules, struct ship ship) ;

/**
 * Construit une position à partir d'une chaîne contenant des chiffres (x)
 * et des lettres majuscule ou minuscule (y).
 * Si il y a plus d'un chiffre (resp. une lettre), seule la dernière
 * occurrence compte.
 *
 * RQ1: Les abscisses sont comprises entre 0 et 9, les ordonées entre 0 et 25
 * RQ2: Cette fonction dispose de plusieurs tests, et une version
 *      approximative suffit pour passer à la suite.
 *      (si vous passez le premier exemple, vous aurez 50% des points)
 *
 * Ex: "A3" == "a3" == {3, 0}
 *     "3A" == "3a" == {3, 0}
 *     "1B3A" == "b13A" == {3, 0}
 */
struct position parse_position(const char *str);



/**
 * Retourne un pointeur sur le bateau (appartenant à la flotte `fleet` de
 * `fleet_size` bateaux) recouvrant la position `pos`, et NULL sinon.
 */
struct ship *find_target(struct rules rules, struct position pos,
        int fleet_size, struct ship fleet[]) ;

/**
 * Retourne l'état de la position `pos` en fonction de la flotte `fleet` (de
 * `fleet_size` bateaux):
 *
 *   -1 => Dans l'eau (i.e., pas de bateau )
 *   0 => une case bateau en bon état (i.e., pas touché)
 *   1 => une case bateau touchée
 *
 * RQ1: On suppose évidemment que les bateaux ne se recouvrent pas.
 * RQ2: Cette fonction ne donne pas d'information sur le fait qu'un bateau soit
 *     coulé ou non.
 */
int hit(struct rules rules, struct position pos,
        int fleet_size, struct ship fleet[]) ;

/**
 * Fait feu à la position `pos` sur la flotte `fleet` (de `fleet_size`
 * bateaux) et change l'état du bateau s'il est touché.
 *
 *   -1    => Dans l'eau (i.e., pas de bateau à la position `pos`)
 *   0     => Touche le bateau couvrant la position `pos`
 *   X > 0 => Le bateau couvrant la position `pos` est coulé et son type est
 *            X-1 (Exemple 1 => DESTROYER)
 */
int fire(struct rules rules, struct position pos,
        int fleet_size, struct ship fleet[]) ;

/**
 * Étant donné une flotte `fleet` (de `fleet_size` bateaux) et des
 * règles, cette fonction remplit dans le tableau `remains` pour
 * chaque type de bateau, le nombre restant de bateaux de ce type (aux
 * indices correspondant à l'`enum ship_kind`).
 *
 * Cette fonction retourne 1 s'il reste des bateaux à placer (selon
 * les règles), 0 si tous les bateaux son placés et -1 s'il y'a trop
 * de bateaux d'au moins un type.
 */
int ships_remaining(struct rules rules, int fleet_size,
        const struct ship fleet[], int remains[]) ;

/**
 * Retourne l'indice du premier bateau de la flotte `fleet` (de `fleet_size`
 * bateaux) qui chevauche le bateau `ship`, -1 s'il n'existe pas de tel bateau.
 *
 * RQ: réutiliser autant que possible les fonctions précédentes.
 */
int ship_overlap(struct rules rules, struct ship ship,
        int fleet_size, const struct ship fleet[]) ;

/**
 * Les règles par défaut de la bataille navale.
 *
 * RQ : Ce ne sont pas les seules règles utilisées dans les tests.
 * RQ2: Ces règles sont statiques, i.e., si vous les changez, ceci ne changera
 *      pas celles utilisés par les tests.
 */
static struct rules basic_rules = {
    { 2, 2, 1, 1, 1 },
    { 2, 1, 3, 4, 5 },
};

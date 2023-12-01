#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "battleship.h"

/**struct ship *find_target(struct rules rules, struct position pos,
        int fleet_size, struct ship fleet[])
{
 
  
  return 0;
}

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
		int fleet_size, const struct ship fleet[], struct dimension board,int tab3[])
{
  int b = (board.width+1) * (board.height+1);
  int tab[b];  // creation d'un tableau
  tab[0]=' ';
  for (int i = 1; i < board.width+2; ++i)
    tab[i]= '0' + i - 1;
  for (int k = 1; k < board.height+2; ++k)
    tab[k*(board.width+1)]='A'+ k -1;
  for (int n = 0; n < board.width ; ++n){
    for (int m = 0; m < board.height; ++m){
      for(int a = 0; a < fleet_size; ++a){
	struct position pos = {n,m};
	switch(hit(rules, pos, fleet_size, fleet))
	  {
	  case -1:
	    if (tab3[n+(m*board.width)] == 1)
	    tab[n+1+((m+1)*(board.width+1))]='o';
	    else
	      tab[n+1+((m+1)*(board.width+1))]=' ';
	    break;
	  case 0:
	    tab[n+1+((m+1)*(board.width+1))]='#';
	    break;
	  case 1:
	    tab[n+1+((m+1)*(board.width+1))]='X';
	    break;
	  default:
	    break;
	  }
      }
    }
  }
  for(int j = 0; j < b; ++j){
    printf("%c ",tab[(j/(board.width))*(board.width) + j%(board.width)]);
    if( j-2 > board.width){
      if ((j+1)%(board.width+1)== 0){
	printf("\n");}
      
    }
    else
      if (j == board.width)
	printf("\n");}
  // affichage d'une partie du tableau qui correspond à 1 ligne.
    
  
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
struct dimension ship_dimension(struct rules rules, struct ship ship) 
{ 
struct dimension local;
if (ship.orientation == 1){
  local.width = 1;
  local.height = ship_size(rules, ship);
  }
  else {
  local.width = ship_size(rules, ship);
  local.height = 1;
  }
return local;
};

/**
 * Retourne le nombre de points de vie restant au bateau `ship`.
 * Si ce nombre est 0, on dit que le bateau est coulé.
 */
int remaining_life(struct rules rules, struct ship ship) 
{
return ship_size(rules,ship) - sum(ship_size(rules,ship),ship.state); 
}

/**
 * Affiche la position `pos` sous la forme YX
 * ou Y (ordonnée) est une lettre entre `A` et `Z`
 * et X (abscisse) est un chiffre entre `0` et `9`
 */
void print_position(struct position pos) 
{
int n = abs(pos.x);
int m = abs(pos.y);
if (n  > 9){
  n = pos.x % 10;
  }
if( m > 26){
  m = pos.y % 26;
}
pos.x = n + '0';
pos.y = m + 'A';
printf("%C%C", pos.y , pos.x);
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
void print_ship(struct rules rules, struct ship ship)
{
int b = 0;
int c = 0;
printf("%s (%d/%d) ", ship_labels[ship.kind], remaining_life(rules, ship), rules.ships_size[ship.kind]); 
print_position(ship.position);
printf("-");
if (ship.orientation == 0)
  b = ship_size(rules, ship)- 1;
  else
    c = ship_size(rules, ship) - 1;
struct position p2 = {ship.position.x + b , ship.position.y + c};
print_position(p2);
printf("\n");
}

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
struct position parse_position(const char *str)
{
struct position position;
while(*str != 0) {
  if  (*str >= 'A' && *str <= ('z'+ 1 )){
    if (*str >= 'a' && *str <= 'z')
      position.y = *str - 'a';
    if (*str >= 'A' && *str <= 'Z')
      position.y = *str - 'A';
  }
  if ( *str >= '0' && *str <= '9')
      position.x = *str - '0' ; 
  str = str + 1 ; 
}
return position;
}

/**
 * Retourne un pointeur sur le bateau (appartenant à la flotte `fleet` de
 * `fleet_size` bateaux) recouvrant la position `pos`, et NULL sinon.
 */
struct ship *find_target(struct rules rules, struct position pos,
        int fleet_size, struct ship fleet[])
{
    while (fleet_size > 0){
        if( inside(pos, fleet[fleet_size - 1].position, 
                       ship_dimension(rules, fleet[fleet_size - 1])) == 1)
        return &fleet[fleet_size - 1];
        fleet_size = fleet_size - 1;}
    return NULL;
}
   

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
        int fleet_size, struct ship fleet[])
{ 
   while (fleet_size > 0){
        if( inside(pos, fleet[fleet_size - 1].position, 
                        ship_dimension(rules, fleet[fleet_size - 1]))) {
          if (fleet[fleet_size - 1].state[offset(pos,fleet[fleet_size - 1].position)]){
            return 1;}
          else 
            return 0;
        }
        fleet_size = fleet_size - 1;
    }   
return -1;  
}

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
	 int fleet_size, struct ship fleet[])
  {
  struct ship *tgt = find_target(rules, pos, fleet_size, fleet);

  if (tgt != NULL)
    {
    int n = offset(pos, tgt->position);
    tgt->state[n] = 1;
    if (remaining_life(rules, *tgt) == 0)
      return tgt->kind + 1;
    else
      return 0;
  }
  else 
    return -1;
}
 



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
        const struct ship fleet[], int remains[]) 
{
int b = 0;
for (int i = 0; i < LAST_SHIP; ++i){  
  remains[i] = rules.ships_count[i];
   }
for (int i = 0; i < fleet_size; ++i ){
  remains[fleet[i].kind] -= 1;
  if (remains[fleet[i].kind] < 0){
    b = 1;
    }
}
int a = sum(LAST_SHIP, remains);
if(b)
    return -1;
if( a > 0)
    return 1;
else
    return 0;
}


/**
 * Retourne l'indice du premier bateau de la flotte `fleet` (de `fleet_size`
 * bateaux) qui chevauche le bateau `ship`, -1 s'il n'existe pas de tel bateau.
 *
 * RQ: réutiliser autant que possible les fonctions précédentes.
 */
int ship_overlap(struct rules rules, struct ship ship,
        int fleet_size, const struct ship fleet[])
{
int b = -1;
for (int i = 0; i < fleet_size; i++){
  struct ship one = ship;
  print_position(one.position);
  for (int a = 0; a < ship_size(rules,ship); a++){
    // for (int c = 
    if (inside(one.position, fleet[i].position, ship_dimension(rules, fleet[i])) == 1){
      return i;
    }
    /* printf("%d inside\n",inside(one.position, fleet[i].position, ship_dimension(rules, fleet[i])));
    one.position.x = one.position.x + (1 < ship_dimension(rules, ship).width);
    one.position.y = one.position.y + (1 < ship_dimension(rules, ship).height);
    print_position(one.position);
    printf(" one\n");
    print_position(fleet[i].position);
    printf(" fleet\n");
    printf("%d b\n",b);
  
  printf("\n\n");*/
  }
}
return b;
}

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

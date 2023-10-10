#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Type représentant une position dans un plan
 */
struct position {
    int x; // Abscisse
    int y; // Ordonnée
};

/**
 * Type représentant des dimensions dans le plan
 */
struct dimension {
    int width;
    int height;
};

enum orientation { HORIZONTAL, VERTICAL };

/**
 * Types de bateaux existant dans la bataille navale
 * Les indices varient de DESTROYER (0) inclus à LAST_SHIP (5) exclu.
 */
enum ship_kind {
    DESTROYER, SUBMARINE, CRUISER, BATTLESHIP, CARRIER,
    LAST_SHIP
};

/**
 * Type représentant les règles d'une variante de la bataille navale.
 * La règle définit :
 * - Le nombre de bateaux de chaque type disponible (`ships_count`)
 * - La taille de chaque bateau (`ships_size`)
 *
 *  RQ: Les indices des tableaux `ships_count` et `ships_size` coïncident avec
 *  les valeurs de l'`enum ship_kind`
 */
struct rules {
    const int ships_count[LAST_SHIP];
    const int ships_size[LAST_SHIP];
};

// Aucun bateau ne pourra avoir une longueur dépassant LONGEST_SHIP
// Il n'est cependant pas requis qu'il existe un bateau aussi long en jeu.
#define LONGEST_SHIP    5

/**
 * Type représentant des bateaux dans notre version de la bataille navale
 *
 * RQ: La taille du bateau est définie à travers son type
 * `ship_kind` à l'intérieur d'une règle `rules`.
 */
struct ship {
    // Type de bateau
    enum ship_kind kind;

    // Coin supérieur gauche du bateau
    struct position position;
    // Orientation du bateau
    enum orientation orientation;

    // Tableau représentant l'état du bateau, (0: Bon état, 1: Touché)
    // RQ: Les valeurs au delà de la longueur spécifiée par les règles pour
    // ce type bateau sont non définies.
    int state[LONGEST_SHIP];
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
        int fleet_size, const struct ship fleet[], struct dimension board);

/**
 * Retourne la somme des `n` premiers éléments du tableau d'entier `array`.
 * Le tableau est supposé contenir au moins `n` éléments
 */
int sum(int n, const int array[]) ;

/**
 * Retourne vrai (1) si la position `pos` appartient au rectangle dont
 * le coin supérieur gauche est `origin` de dimension `dim`. Cette fonction
 * retourne faux (0) sinon.
 *
 * Exemples:
 * - Vrai: pos={1, 2}, origin={0, 0}, dim={3, 3}
 * - Faux: pos={4, 3}, origin={1, 0}, dim={3, 3}
 */
int inside(struct position pos,
        struct position origin, struct dimension dim) ;

/**
 * Contraint la position `pos` aux dimensions `dim`.
 * Si la position y appartient déjà elle est inchangée, sinon elle prendra
 * la valeur de la position valide la plus proche.
 *
 * Exemple:
 * - pos={7,3}, dim={4,4} -> pos devient {3,3}
 */
void constrain(struct position *pos, struct dimension dim) ;

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
int offset(struct position pos, struct position origin) ;

/**
 * Retourne la taille du bateau `ship` (en nombre de cases) en fonction d'une
 * règle du jeu.
 *
 * EX (Avec des règles par défaut): CARRIER => 5
 */
int ship_size(struct rules rules, struct ship ship) ;

/**
 * Retourne une dimension représentant la taille du bateau `ship` et son
 * orientation.
 *
 * RQ: Tous les bateaux ne font qu'une case de large
 * EX (Avec des règles par défaut): CARRIER VERTICAL => {1, 5}
 */
struct dimension ship_dimension(struct rules rules, struct ship ship) ;

/**
 * Retourne le nombre de points de vie restant au bateau `ship`.
 * Si ce nombre est 0, on dit que le bateau est coulé.
 */
int remaining_life(struct rules rules, struct ship ship) ;

/**
 * Affiche la position `pos` sous la forme YX
 * ou Y (ordonnée) est une lettre entre `A` et `Z`
 * et X (abscisse) est un chiffre entre `0` et `9`
 */
void print_position(struct position pos) ;

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
struct position parse_position(const char *str) ;

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

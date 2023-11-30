#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "battleship.h"







struct ship ask_ship(struct dimension dim)
{

  struct ship ship;

  //creer le type de bateau
  char kind[10];
  printf("Choisir le bateau a placer : DESTROYER, SUBMARINE, CRUISER, BATTLESHIP, CARRIER ?  \n");
  fgets(kind, 20, stdin);
  int ship_kind;
  if (kind == "DESTROYER")
  { 
    ship_kind = 0;

  }
  

  ship.kind = ship_kind;
  
// demander une position au joueur
  char pos[10] ;
  printf("Entrez des coordonnées : \n");
  fgets(pos, 10, stdin);
   
    //transformer la position en (chiffre;chiffre)
  struct position position = parse_position(pos);

    //contraindre la position dans la grille
  struct position poscorrect = position ;
  constrain( &poscorrect,  dim);

    //correction de la position si besoin 
  if ( poscorrect.x != position.x || poscorrect.y != position.y)
    {
      printf("Desolé, position non valide. La nouvelle position est : \%d,%\d ;) \n", poscorrect.x, poscorrect.y);
      ship.position = poscorrect;
    }
  else
    {
      ship.position = position;
    }

// demander l'orientation du bateau
 char orient[10];
 printf("Choisir l'orientation du bateau : HORIZONTAL ou VERTICAL ? \n");
 fgets(orient, 10, stdin);
 int orientation;
 if (orient == "HORIZONTAL")
 {
   orientation = 0;
 }
 ship.orientation = orientation;

return ship;

}








#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "battleship.h"




struct position ask_position(struct dimension dim)
{

  // demander une position au joueur
  char pos[10] ;
  printf("Entrez des coordonnées : \n");
  fgets(pos, 10, stdin);
   

  //transformer la position en (chiffre;chiffre)
  struct position position = parse_position(pos);

  //contraindre la position dans la grille
  struct position poscorrect = position ;
  constrain( &poscorrect,  dim);

  //debugage
  /* printf("position \%d,%\d \n", position.x, position.y); */
  /* printf("poscorrect \%d,%\d \n", poscorrect.x, poscorrect.y); */

  //correction de la position si besoin + return de la position
  if ( poscorrect.x != position.x || poscorrect.y != position.y)
    {
      printf("Desolé, ca rentre pas dans la grille, mais j'ai changé ca en \%d,%\d ;) \n", poscorrect.x, poscorrect.y);
      return poscorrect;
    }
  else
    {
      return position;
    }
}


struct ship ask_ship(struct dimension dim)
{

  //Kind bateau
  
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
      printf("Desolé, ca rentre pas dans la grille, mais j'ai changé ca en \%d,%\d ;) \n", poscorrect.x, poscorrect.y);
      return poscorrect;
    }
  else
    {
      return position;
    }



}






int main (int argc, char *argv[])
{
  struct dimension dim = {8, 8};


  for (int i = 0; i < 3; ++i)
    fleet[i] = ask_ship(dim);
  

  
  printf("Placement du  croiseur \n");
  struct position croiseur =  ask_position(dim);
  printf("la position du croiseur est %d %d \n", croiseur.x, croiseur.y);

  printf("Placement du  sous marin \n");
  struct position submarine =  ask_position(dim);
  printf("la position du sous marin est %d %d \n", submarine.x, submarine.y);

  printf("Placement du destroyer \n");
  struct position destroyer =  ask_position(dim);
  printf("la position du destroyer est %d %d \n", destroyer.x, destroyer.y);

  struct ship fleet[3] = {
    {CRUISER, croiseur, HORIZONTAL},
  };

  return 0;

}

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "battleship.h"




struct position ask_position(pos, )
{

  // demander une position au joueur
  char pos[10] ;
  printf("Entrez des coordonnées de bateaux a placer\n");
  fgets(pos, 10, stdin);

  //transformer la position en (chiffre;chiffre)
  struct position position = parse_position(pos);

  //contraindre la position dans la grille
  position = constrain(pos, dim);

  //verifier qu'on se remet pas sur un autre bateau apres le constrain
  ship_overlap(rules, ship, fleet_size, fleet);
 
  

  return position;
}


int main (int argc, char *argv[])
{
  struct dimension dim = {10, 10}

  ask_position(pos, grid);
  printf("la position est %d %d \n", position.x, position.y);

  

  return 0;

}

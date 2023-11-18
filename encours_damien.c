#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "battleship.h"



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



int main (int argc, char *argv[])
{
  struct rules r = {
    {1, 1, 1, 1, 1},
    {2, 2, 3, 4, 5}
  };

  struct ship fleet[] = {
    {CRUISER, {1, 1}, HORIZONTAL, {0, 0, 0, 0, 0}},
  };
  
  int fleet_size = 1;
    
  struct position pos = {0,0};
  
  int result = fire(r, pos, fleet_size, fleet);
  printf("fire ! %d\n", result);

  pos = {1,0};
  
  result = fire(r, pos, fleet_size, fleet);
  printf("fire ! %d\n", result);

  return 0 ;
}

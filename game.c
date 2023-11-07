#include <stdio.h>
#include "battleship.h"

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

int main(int argc, char *argv[])
{
  struct dimension grid_size = {10, 10};  // grille de 10 x 10 
  int fleet_size = 3;   
  struct ship fleet[] = {
    {DESTROYER, {4, 7}, VERTICAL, {1, 1}},
    {CARRIER, {1, 2}, HORIZONTAL, {0, 1, 0}},
    {BATTLESHIP, {5, 6}, HORIZONTAL, {0}},
  };
    struct dimension dev;
    dev = ship_dimension(basic_rules, fleet[0]);
    printf("%d\n",remaining_life(basic_rules, fleet[0]));
    printf("%d%d\n", dev.width, dev.height);
    print_grid(basic_rules, fleet_size, fleet, grid_size);
    return 0;
}


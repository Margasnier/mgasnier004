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


static const char* const ship_labels[] = {
    "Destroyer", "Submarine", "Cruiser", "Battleship", "Carrier"
};


int random_between(int min, int max)   //renvoi une valeur pseudo-aléatoire entre min et max.
{
  return  min = (rand()%(max-min)) + min;
}


//retour 0 si plus de bateau sinon le nombre restant

int notfleet(struct ship fleet[],int fleet_size, struct rules rules) 
{
  int a = 0;
  for( int i = 0; i < fleet_size; ++i){
   
    if (remaining_life(rules, fleet[i]) > 0)
      a = a + 1;
  }
  return a;
}

void state_fleet(struct rules rules,int fleet_size, struct ship fleet[]) 
{
  for(int i = 0; i < fleet_size; ++i)
    print_ship(rules, fleet[i]);
}

int game_ready(struct rules rules, struct dimension grid, int fleet_size,struct ship fleet[])
{
  int tab[grid.width*grid.height];
  int d = 0;
  int a = notfleet(fleet, fleet_size, rules);
  while (a > 0){
    struct position pos = {random_between(0, grid.width-1), random_between(0, grid.height-1)};
    int b = pos.x + (grid.width*pos.y);
    while (tab[b] == 1){
      if ( b > 0){
	b = b - 1;}
      else
	b = grid.width * grid.height;
      
    }
    tab[b] = 1;
    pos.x = b % grid.width;
    pos.y = b / grid.width;
    struct ship *target = find_target(rules, pos, fleet_size, fleet);
    if(target !=NULL){
      int n = offset(pos, target->position);
      target->state[n] = 1;
    }
    system ("clear");
    print_grid(rules, fleet_size, fleet, grid);
    state_fleet(basic_rules,fleet_size, fleet);
    usleep(100000);
    d = d + 1;
    a = notfleet(fleet, fleet_size, rules);
 
  }
  return d;
}


  int main(int argc, char *argv[])
  {
    struct ship fleet[]= {{DESTROYER,{4,5},HORIZONTAL},{DESTROYER,{1,2},HORIZONTAL},{DESTROYER,{7,8},HORIZONTAL}};
    int seed = init_rand(-1, "SEED");
    printf("seed %d\n", seed);
    int fleet_size = 3;
    struct dimension grid_size = {10, 10};    // grille de 10 x 10

    printf("Appuyer sur la touche retour pour lancer le jeu:\n");
    int a = game_ready(basic_rules, grid_size, fleet_size, fleet);
    printf("Bravo vous avez gagné en %d coups\n ", a);
    
    return 0;
  }


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
void print_message (int a,struct position pos)
{
  if (a==0){
    printf("Feu en ");
    print_position(pos);
  }
  printf("\n");
}

int game_ready(struct rules rules, struct dimension grid, int fleet_size,struct ship fleet[])
{
  int tab_game[grid.width*grid.height];
  int d = 0;
  int a = notfleet(fleet, fleet_size, rules);
  while (a > 0){
    struct position pos = {random_between(0, grid.width-1), random_between(0, grid.height-1)};
    int b = pos.x + (grid.width*pos.y);
    while (tab_game[b] == 1){
      if ( b > 0){
	b = b - 1;}
      else
	b = grid.width * grid.height;
      
    }
    tab_game[b] = 1;
    pos.x = b % grid.width;
    pos.y = b / grid.width;
    struct ship *target = find_target(rules, pos, fleet_size, fleet);
    if(target !=NULL){
      int n = offset(pos, target->position);
      target->state[n] = 1;
    }
    system ("clear");
    print_message(0, pos);
    print_grid(rules, fleet_size, fleet, grid,  tab_game),
    state_fleet(basic_rules,fleet_size, fleet);
    usleep(500000);
    d = d + 1;
    a = notfleet(fleet, fleet_size, rules);
 
  }
  return d;
}

struct ship ask_ship(struct dimension dim, int fleet_size)
{
 printf("vous jouez avec : %d bateaux \n \n", fleet_size);
 // struct ship ship;

  //creer le type de bateau
  char kind[10];
  printf("Choisir le bateau a placer : DESTROYER => 0, SUBMARINE => 1, CRUISER => 2, BATTLESHIP => 3, CARRIER => 4 ?  \n");
  fgets(kind, 20, stdin);
 int ship_kind = atoi(kind);
 // ship.kind = ship_kind;
  
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
      position = poscorrect;
    }
 

// demander l'orientation du bateau
 char orient[10];
 printf("Choisir l'orientation du bateau : HORIZONTAL => 0 ou VERTICAL => 1 ? \n");
 fgets(orient, 10, stdin);
 int orientation = atoi(orient);
 //ship.orientation = orientation;
 struct ship ship = {ship_kind, position, orientation};
 

return ship;


}




int main(int argc, char *argv[])
{
  int seed = init_rand(-1, "SEED"); // initialise le code pseudo-aléatoire
  struct dimension grid_size = {10, 10};    // grille de 10 x 10
  int fleet_size = 1;
  struct ship fleet[fleet_size];
  for (int i = 0; i < fleet_size; ++i)
    {
      fleet[i] = ask_ship(grid_size, fleet_size);
      state_fleet(basic_rules,fleet_size, fleet);
    }
  /*struct ship fleet[i]=
    {{DESTROYER,{4,5},HORIZONTAL},
    {DESTROYER,{1,2},HORIZONTAL},
    {DESTROYER,{7,8},HORIZONTAL}};*/
 
  //printf("seed %d\n", seed);
  /*int fleet_size = 3; */
  
  
  printf("Appuyer sur la touche retour pour lancer le jeu:\n");
  int buffer[100];
  fgets(buffer, 100, stdin);
  int a = game_ready(basic_rules, grid_size, fleet_size, fleet);
  printf("Bravo vous avez gagné en %d coups\n ", a);
    
  return 0;

}

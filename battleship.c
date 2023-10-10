#include "battleship.h"

/**
 * Le nom des bateaux sous forme de chaînes de caractères immutables.
 * Les entrées du tableau coïncident avec les valeurs de l'`enum ship_kind`
 */
static const char* const ship_labels[] = {
    "Destroyer", "Submarine", "Cruiser", "Battleship", "Carrier"
};

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
    return 0;
}

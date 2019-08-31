#ifndef FCT_DEMINEUR_HPP
#define FCT_DEMINEUR_HPP

#include <string>
#include <ctime>
#include <vector>
using namespace std;

//**********************************************************************************************************************************
//  STRUCTURE (Coord) contenant deux entiers pour la sélection d'une case du tableau + un opérateur d'égalité
struct Coord{
        int row;
        int column;
        bool operator == (const Coord& c) {
                      return this->row==c.row && this->column==c.column;
        }
};
typedef Coord Coord;

//**********************************************************************************************************************************
//  PROTOTYPES DES FONCTIONS
bool testplaceBombes(int row, int column);
void desempilementPileBombes();
void placeBombes(int **tab, int nbBombes, int N);
int nbBombesAlentour(int **tab, int N, int row, int column);

#endif // FCT_DEMINEUR_HPP

#include "fct_demineur.hpp"

//**********************************************************************************************************************************
//  VARIABLE GLOBALE
vector<Coord> pileBombes;

//**********************************************************************************************************************************
//  FONCTION teste l'emplacement d'une mine dans le tableau (i.e pour ne pas avoir deux mines ou plus dans la même case)
bool testplaceBombes(int row, int column)
{
    bool test=false;
    int i(0);
    Coord c;
    c.row=row;
    c.column=column;
    for(i=0;i<pileBombes.size();i++)
    {
        if(pileBombes[i].row==c.row)
        {
            if(pileBombes[i].column==c.column)
            {
                test=true;
            }
        }
    }
    pileBombes.push_back(c);
    return test;
}

//  FONCTION pour le désempilement du tableau dynamique (QVector pileBombes)
void desempilementPileBombes()
{
    while(pileBombes.size()!=0)
    {
        pileBombes.pop_back();
    }
}

//  FONCTION place les nbBombes aléatoirement dans les cases du tableau
void placeBombes(int **tab, int nbBombes, int N)
{
    int i(0),row(0),column(0);
    srand(time(NULL));

    for(i=0;i<nbBombes;i++)
    {
       do{
            row=rand()%N;
            column=rand()%N;
        }while(testplaceBombes(row,column));
        tab[row][column]=64;
    }
}

//  FONCTION retourne le nombre de mines autour de la coordonnée donnée en paramètres (les huit cases adjacentes)
int nbBombesAlentour(int **tab,int N, int row,int column)
{
    int nbBombes(0);

    //Haut gauche
    if((row-1) >= 0 && (column-1) >= 0)
    {
        if(tab[row-1][column-1] == 64)
        {
            nbBombes++;
        }
    }

    //Haut centre
    if((row-1) >= 0)
    {
        if(tab[row-1][column] == 64)
        {
            nbBombes++;
        }
    }
    //Haut droite
    if((row-1) >= 0 && (column+1) < N)
    {
        if(tab[row-1][column+1] == 64)
        {
            nbBombes++;
        }
    }

    //Centre gauche
    if((column-1) >= 0)
    {
        if(tab[row][column-1] == 64)
        {
            nbBombes++;
        }
    }

    //Centre droite
    if((column+1) < N)
    {
        if(tab[row][column+1] == 64)
        {
            nbBombes++;
        }
    }

    //Bas gauche
    if((row+1) < N && (column-1) >= 0)
    {
        if(tab[row+1][column-1] == 64)
        {
            nbBombes++;
        }
    }

    //Bas centre
    if((row+1) < N)
    {
        if(tab[row+1][column] == 64)
        {
            nbBombes++;
        }
    }

    //Bas droite
    if((row+1) < N && (column+1) < N)
    {
        if(tab[row+1][column+1] == 64)
        {
            nbBombes++;
        }
    }

    return nbBombes;
}

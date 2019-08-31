#ifndef DEMINEURFEN_HPP
#define DEMINEURFEN_HPP

#include <QApplication>
#include <QtWidgets>
#include "tableaujeu.hpp"
#include "difficultefen.hpp"
#include "fct_demineur.hpp"
#include "lcdnumber_p.hpp"
#include "meilleursscoresfen.hpp"
#include "nomjoueur.hpp"
#include "label_p.hpp"

//**********************************************************************************************************************************
//  CLASSE
class DemineurFen: public QMainWindow
{
Q_OBJECT

public:
    DemineurFen();

public slots:
    void algo(int row, int column);
    void placeDrapeau(int row, int column);
    void options();
    void nouvellePartie();
    void meilleursScores();
    void finMeilleursScores();
    void finSaisiNom();
    void timerTempsPause();
    void timerTempsRedeclenche();

private slots:
    void closeEvent(QCloseEvent *event);

private:
    TableauJeu *m_tab;                      //  Tableau de jeu visualisable par le joueur
    QLCDNumber *m_lcdBombes;                //  LCD affichant le nombre de mines restantes
    LCDNumber_p *m_lcdTemps;                //  LCD affichant le chronomètre
    Label_p *m_labelSmiley;                 //  Label contenant un smiley (normal, heureux ou triste)
    DifficulteFen m_difficulte;             //  Attribut permettant d'avoir une instance de classe pour la sélection du mode de jeu
    MeilleurScoresFen m_meilleurScores;     //  Attribut permettant d'avoir une instance de classe pour la fenêtre des meilleurs scores
    NomJoueurFen m_nomJoueur;               //  Attribut permettant d'avoir une instance de classe pour le nom du joueur
    int m_nbBombes;                         //  Entier contenant le nombre de mines maximal à déminer
    int m_nbBombesRestantDrapeau;           //  Entier contenant le nombre de mines restantes à déminer
    int m_N;                                //  Entier utilisé pour la taille du tableau de jeu (en fonction du mode de jeu)
    int **m_tabcachee;                      //  Tableau dynamique qui contiendra la solution après initialisation
    int m_nbCases;                          //  Entier contenant le nombre de cases restantes a dévoiler
    bool m_partieTerminee;                  //  Booléen permettant de savoir si la partie est terminée ou non (true oui, false sinon)

public:
    void init();
    void nbBombesvsDifficulte();
    void allocationdynamique();
    void desallocationdynamique();
    void desempilement();
    void setTabZeroBombes(int row, int column);
    bool testCoord(QVector <Coord> tab, int row, int column);
    int indicePlacementDrapeau(int row,int column);
    QColor couleurNombre(int nb);
    int getSelectionDifficulte();
};

#endif // DEMINEURFEN_HPP

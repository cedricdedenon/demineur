#ifndef MEILLEURSSCORESFEN_HPP
#define MEILLEURSSCORESFEN_HPP

#include <QApplication>
#include <QtWidgets>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

//**********************************************************************************************************************************
//STRUCTURE (MSCores) contenant une chaine de caractère (nom du joueur) et un entier (le temps effectué)
struct MScores{
        string nom;
        int temps;
};
typedef MScores MScores;

//**********************************************************************************************************************************
//  CLASSE
class MeilleurScoresFen: public QWidget
{
Q_OBJECT

public:
    MeilleurScoresFen();

private:
    QTabWidget *onglets;
    QWidget *pageDebutant;
    QWidget *pageIntermediaire;
    QWidget *pageExpert;
    QTextEdit *textDebutant;
    QTextEdit *textIntermediaire;
    QTextEdit *textExpert;
    QPushButton *m_boutonOK;
    QPushButton *m_boutonEffacerHist;
    QVector<MScores> m_scoresDebutant;          //Tableau dynamique contenant les cinq meilleurs scores pour le mode débutant
    QVector<MScores> m_scoresIntermedaire;      //Tableau dynamique contenant les cinq meilleurs scores pour le mode intermédiaire
    QVector<MScores> m_scoresExpert;            //Tableau dynamique contenant les cinq meilleurs scores pour le mode expert

public slots:
    void effacerHistorique();

private slots:
    void closeEvent(QCloseEvent *event);

public:
    void actualisationHistorique(string n, int t, int d);
    void initVector();
    void initVectorNull();
    void affichageVector();
    void desempilement();
    void effacerText();
    QPushButton* getBoutonOK();
    QVector<MScores> getScoreDebutant();
    QVector<MScores> getScoreIntermediaire();
    QVector<MScores> getScoreExpert();

signals:
    void meilleurscoresfenferme();
};

bool lessThan(const MScores & m1, const MScores & m2);

#endif // MEILLEURSSCORESFEN_HPP

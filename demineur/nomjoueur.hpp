#ifndef NOMJOUEUR_HPP
#define NOMJOUEUR_HPP

#include <QApplication>
#include <QtWidgets>
#include <string>
using namespace std;

//**********************************************************************************************************************************
//  CLASSE
class NomJoueurFen: public QWidget
{
Q_OBJECT

public:
    NomJoueurFen();

private:
    QLabel *m_labelNom;
    QLineEdit *m_lineEditNom;
    QPushButton *m_boutonOK;
    QPushButton *m_boutonAnnuler;

public slots:
    void effacerSaisiNom();

public:
    QPushButton* getValidationNom();
    string getNomJoueur();
};

#endif // NOMJOUEUR_HPP

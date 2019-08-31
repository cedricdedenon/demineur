#include "nomjoueur.hpp"

//**********************************************************************************************************************************
//  CONSTRUCTEUR
NomJoueurFen::NomJoueurFen()
{
    //Création et définition des widgets utiles à l'application
    m_labelNom = new QLabel("Entrer votre nom: ",this);
    m_lineEditNom = new QLineEdit;
    m_boutonOK = new QPushButton("OK",this);
    m_boutonAnnuler = new QPushButton("Annuler",this);;

    //Création des layouts pour le placement des widgets
    QHBoxLayout *hboxboutons = new QHBoxLayout;
    hboxboutons->addWidget(m_boutonOK,0);
    hboxboutons->addWidget(m_boutonAnnuler,1);

    QVBoxLayout *layoutprincipal = new QVBoxLayout;
    layoutprincipal->addWidget(m_labelNom,0);
    layoutprincipal->addWidget(m_lineEditNom,1);
    layoutprincipal->addLayout(hboxboutons,2);

    this->setFixedSize(200,100);
    setLayout(layoutprincipal);

    //Interactions avec l'utilisateur (appui sur un bouton ...)
    connect(m_boutonAnnuler,SIGNAL(clicked(bool)),this,SLOT(effacerSaisiNom()));
}

//**********************************************************************************************************************************
//  SLOT efface le contenu du texte lors de l'appui du bouton m_boutonAnnuler
void NomJoueurFen::effacerSaisiNom()
{
    m_lineEditNom->clear();
}

//**********************************************************************************************************************************
//  METHODE accesseur sur l'attribut m_boutonOK
QPushButton* NomJoueurFen::getValidationNom()
{
    return m_boutonOK;
}

//  METHODE retourne le texte entré par le joueur
string NomJoueurFen::getNomJoueur()
{
    return (m_lineEditNom->displayText().toStdString());
}

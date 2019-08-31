#include "difficultefen.hpp"
#include "demineurfen.hpp"

//**********************************************************************************************************************************
//  CONSTRUCTEUR
DifficulteFen::DifficulteFen(): QWidget()
{
    //Création et définition des widgets utiles à l'application
    QGroupBox *groupbox = new QGroupBox("Difficulte: ", this);
    m_debutant = new QRadioButton("Débutant");
    m_intermediaire = new QRadioButton("Intermédiaire");
    m_expert = new QRadioButton("Expert");

    //Par défaut le mode facile est sélectionné
    m_debutant->setChecked(true);

    m_boutonOK = new QPushButton("OK",this);

    //Création du layout et organisation des widgets dans la fenêtre
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_debutant);
    layout->addWidget(m_intermediaire);
    layout->addWidget(m_expert);
    layout->addWidget(m_boutonOK);

    this->setFixedSize(40,130);
    groupbox->setLayout(layout);

    //Interactions avec l'utilisateur (appui sur un bouton)
    connect(m_boutonOK,SIGNAL(clicked()),this,SLOT(close()));
}

//**********************************************************************************************************************************
//  SLOT ré-implantant (closeEvent) lors de la fermeture de la fenêtre (croix rouge) et emet un nouveau signal
void DifficulteFen::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit difficultefenferme();
}

//**********************************************************************************************************************************
//  METHODE accesseur pour connaître si le mode débutant est sélectionné
bool DifficulteFen::getSelection_debutant()
{
    return m_debutant->isChecked();
}

//  METHODE accesseur pour connaître si le mode intermédiaire est sélectionné
bool DifficulteFen::getSelection_intermediaire()
{
    return m_intermediaire->isChecked();
}

//  METHODE accesseur pour connaître si le mode expert est sélectionné
bool DifficulteFen::getSelection_expert()
{
    return m_expert->isChecked();
}

//  METHODE accesseur sur l'attribut m_boutonOK
QPushButton* DifficulteFen::getboutonOK()
{
    return m_boutonOK;
}

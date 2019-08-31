#include "meilleursscoresfen.hpp"

//**********************************************************************************************************************************
//  CONSTRUCTEUR
MeilleurScoresFen::MeilleurScoresFen()
{
    //Création et définition des widgets utiles à l'application
    onglets = new QTabWidget(this);
    pageDebutant = new QWidget;
    pageIntermediaire = new QWidget;
    pageExpert = new QWidget;

    textDebutant = new QTextEdit;
    textIntermediaire = new QTextEdit;
    textExpert = new QTextEdit;

    textDebutant->setFont(QFont("Arial",9));
    textDebutant->setReadOnly(true);
    textIntermediaire->setFont(QFont("Arial",9));
    textIntermediaire->setReadOnly(true);
    textExpert->setFont(QFont("Arial",9));
    textExpert->setReadOnly(true);

    m_boutonOK = new QPushButton("OK",this);
    m_boutonEffacerHist = new QPushButton("Effacer l'historique",this);

    //Appel aux fonctions d'initialisations
    initVector();
    affichageVector();

    //Création des layouts pour le placement des widgets
    QVBoxLayout *vboxdebutant = new QVBoxLayout;
    vboxdebutant->addWidget(textDebutant);
    pageDebutant->setLayout(vboxdebutant);

    QVBoxLayout *vboxintermediaire = new QVBoxLayout;
    vboxintermediaire->addWidget(textIntermediaire);
    pageIntermediaire->setLayout(vboxintermediaire);

    QVBoxLayout *vboxexpert = new QVBoxLayout;
    vboxexpert->addWidget(textExpert);
    pageExpert->setLayout(vboxexpert);

    onglets->addTab(pageDebutant, "Débutant");
    onglets->addTab(pageIntermediaire, "Intermédiaire");
    onglets->addTab(pageExpert, "Expert");

    QHBoxLayout *hboxboutons = new QHBoxLayout;
    hboxboutons->addWidget(m_boutonOK,0);
    hboxboutons->addWidget(m_boutonEffacerHist,1);

    QVBoxLayout *layoutprincipal = new QVBoxLayout;
    layoutprincipal->addWidget(onglets,0);
    layoutprincipal->addLayout(hboxboutons,1);

    this->setFixedSize(400,220);
    setLayout(layoutprincipal);

    //Interactions avec l'utilisateur (appui sur un bouton ...)
    connect(m_boutonEffacerHist,SIGNAL(clicked(bool)),this,SLOT(effacerHistorique()));
}

//**********************************************************************************************************************************
//  SLOT appelé pour effacer et ré-initialiser les meilleurs scores
void MeilleurScoresFen::effacerHistorique()
{
    effacerText();
    desempilement();
    initVectorNull();
    affichageVector();
}

//  SLOT ré-implantant (closeEvent) lors de la fermeture de la fenêtre (croix rouge) et emet un nouveau signal
void MeilleurScoresFen::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit meilleurscoresfenferme();
}

//**********************************************************************************************************************************
//  METHODE pour actualiser l'historique (avec en paramètres le nom du joueur, le temps effectué et le mode de jeu sélectionné)
void MeilleurScoresFen::actualisationHistorique(string n, int t, int d)
{
    MScores s;
    s.nom=n;
    s.temps=t;
    if(d == 1)
    {
        m_scoresDebutant.push_back(s);
        effacerText();
        sort(m_scoresDebutant.begin(),m_scoresDebutant.end(),lessThan);
        if(m_scoresDebutant.size() >= 5){
            while(m_scoresDebutant.size() != 5)
            {
                m_scoresDebutant.pop_back();
            }
        }
    }
    else if (d == 2)
    {
        m_scoresIntermedaire.push_back(s);
        effacerText();
        sort(m_scoresIntermedaire.begin(),m_scoresIntermedaire.end(),lessThan);
        if(m_scoresIntermedaire.size() >= 5){
            while(m_scoresIntermedaire.size() != 5)
            {
                m_scoresIntermedaire.pop_back();
            }
        }
    }
    else if (d == 3)
    {
        m_scoresExpert.push_back(s);
        effacerText();
        sort(m_scoresExpert.begin(),m_scoresExpert.end(),lessThan);
        if(m_scoresExpert.size() >= 5){
            while(m_scoresExpert.size() != 5)
            {
                m_scoresExpert.pop_back();
            }
        }
    }
    affichageVector();
}

//  METHODE d'initialisation de l'historique (des trois tableaux dynamiques QVector)
void MeilleurScoresFen::initVector()
{
    QFile f("scores.ini");
    //Si le fichier des meilleurs scores n'existe pas, les meilleurs scores prennent les valeurs par défaut
    if (!f.exists())
    {
        initVectorNull();
    }
    else{
        //Sinon les meilleurs scores sont lus puis chargés dans les tableaux dynamiques
        QSettings setting("scores.ini",QSettings::IniFormat);
        MScores mscores;
        for(int i=0;i<5;i++)
        {
            mscores.nom=(setting.value("Debutant/" + QString::number(i+1) + "/Nom").toString()).toStdString();
            mscores.temps=setting.value("Debutant/" + QString::number(i+1) + "/Temps").toInt();
            m_scoresDebutant.push_back(mscores);
            mscores.nom=(setting.value("Intermediaire/" + QString::number(i+1) + "/Nom").toString()).toStdString();
            mscores.temps=setting.value("Intermediaire/" + QString::number(i+1) + "/Temps").toInt();
            m_scoresIntermedaire.push_back(mscores);
            mscores.nom=(setting.value("Expert/" + QString::number(i+1) + "/Nom").toString()).toStdString();
            mscores.temps=setting.value("Expert/" + QString::number(i+1) + "/Temps").toInt();
            m_scoresExpert.push_back(mscores);
        }
    }
}

//  METHODE d'initialisation de l'historique par des valeurs par défaut
void MeilleurScoresFen::initVectorNull()
{
    MScores mscores;
    mscores.nom="______";
    mscores.temps=9999;
    int i(0);

    for(i=0;i<5;i++)
    {
        m_scoresDebutant.push_back(mscores);
        m_scoresIntermedaire.push_back(mscores);
        m_scoresExpert.push_back(mscores);
    }
}
//  METHODE pour afficher le contenu des tableaux dynamiques (les cinq meilleurs scores et selon le mode de jeu)
void MeilleurScoresFen::affichageVector()
{
    int i(0);
    for(i=0;i<5;i++)
    {
        textDebutant->insertPlainText(QString::number(i+1) +".\t" + QString::fromStdString(m_scoresDebutant[i].nom) + "\t" + QString::number(m_scoresDebutant[i].temps) + " secondes\n");
        textIntermediaire->insertPlainText(QString::number(i+1) +".\t" + QString::fromStdString(m_scoresIntermedaire[i].nom) + "\t" + QString::number(m_scoresIntermedaire[i].temps) + " secondes\n");
        textExpert->insertPlainText(QString::number(i+1) +".\t" + QString::fromStdString(m_scoresExpert[i].nom) + "\t" + QString::number(m_scoresExpert[i].temps) + " secondes\n");
    }
}

//  METHODE pour le désempilement de tous les tableaux dynamiques  (QVector)
void MeilleurScoresFen::desempilement()
{
    while(m_scoresDebutant.size()!=0)
    {
        m_scoresDebutant.pop_back();
    }
    while(m_scoresIntermedaire.size()!=0)
    {
        m_scoresIntermedaire.pop_back();
    }
    while(m_scoresExpert.size()!=0)
    {
        m_scoresExpert.pop_back();
    }
}

//  METHODE pour effacer les textes de trois modes de jeu (QEditText)
void MeilleurScoresFen::effacerText()
{
    textDebutant->clear();
    textIntermediaire->clear();
    textExpert->clear();
}

//  METHODE accesseur sur l'attribut m_boutonOK
QPushButton* MeilleurScoresFen::getBoutonOK()
{
    return m_boutonOK;
}

//  METHODE accesseur sur l'attribut m_scoresDebutant
QVector<MScores> MeilleurScoresFen::getScoreDebutant()
{
    return m_scoresDebutant;
}

//  METHODE accesseur sur l'attribut m_scoresIntermediaire
QVector<MScores> MeilleurScoresFen::getScoreIntermediaire()
{
    return m_scoresIntermedaire;
}

//  METHODE accesseur sur l'attribut m_scoresExpert
QVector<MScores> MeilleurScoresFen::getScoreExpert()
{
    return m_scoresExpert;
}

//**********************************************************************************************************************************
//  FONCTION pour comparer le temps du joueur avec un autre temps (utilisé pour le tri (sort))
bool lessThan(const MScores &m1, const MScores &m2)
{
    return (m1.temps < m2.temps);
}

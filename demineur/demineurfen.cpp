#include "demineurfen.hpp"

//**********************************************************************************************************************************
//  VARIABLES GLOBALES
QVector <Coord> TabZeroBombes;          //Tableau dynamique contenant toutes les coordonnées ne contenant que les zéros
QVector <Coord> TabNonBombes;           //Tableau dynamique contenant toutes les coordonnées
QVector <Coord> TabPlacementDrapeau;    //Tableau dynamique contenant toutes les coordonnées dont un drapeau a été déposé

//**********************************************************************************************************************************
//  CONSTRUCTEUR
DemineurFen::DemineurFen()
{
    QWidget *zoneCentrale = new QWidget;
    QFont maPolice("Arial",9);

    //Déclaration des menus, sous-menus et des actions
    QMenu *menuPartie = menuBar()->addMenu("&Partie");
    QMenu *menuAide = menuBar()->addMenu("&?");

    QAction *actionNvlPartie = new QAction("&Nouvelle Partie",this);
    QAction *actionOptions = new QAction("&Options",this);
    QAction *actionMeilleursScores = new QAction("&Meilleurs scores",this);
    QAction *actionQuitter = new QAction("&Quitter", this);
    QAction *actionAide = new QAction("&Aide",this);
    QAction *actionAPropos = new QAction("&A Propos",this);

    menuPartie->addAction(actionNvlPartie);
    menuPartie->addAction(actionOptions);
    menuPartie->addAction(actionMeilleursScores);
    menuPartie->addAction(actionQuitter);
    menuAide->addAction(actionAide);
    menuAide->addAction(actionAPropos);

    //Raccourci pour les menus
    actionNvlPartie->setShortcut(QKeySequence("CTRL+N"));
    actionQuitter->setShortcut(QKeySequence("CTRL+Q"));
    actionOptions->setShortcut(QKeySequence("CTRL+O"));
    actionMeilleursScores->setShortcut(QKeySequence("CTRL+S"));
    actionAide->setShortcut(QKeySequence("CTRL+F1"));
    actionAPropos->setShortcut(QKeySequence("CTRL+I"));

    //Fenetre de dialogue pour l'aide
    QDialog *fenaide = new QDialog(zoneCentrale);
    QVBoxLayout *layoutaide = new QVBoxLayout;
    QLabel *labelaide = new QLabel("Le jeu du démineur consiste à localiser des mines/bombes sur un tableau avec pour seule "
                                    "indication le nombre de mines adjacentes.\n\nChaque case de la grille peut soit cacher une mine, "
                                    "soit être vide. Le but du jeu est de découvrir toutes les cases libres sans faire exploser les mines."
                                    "\nLorsque le joueur clique sur une case libre comportant au moins une mine dans l'une de ses cases "
                                    "avoisinantes, un chiffre apparaît, indiquant ce nombre de mines.\nSi en revanche toutes les cases "
                                    "adjacentes  sont vides, une case vide est affichée et la même opération est répétée sur ces cases, "
                                    "et ce jusqu'à ce que la zone vide soit entièrement délimitée par des chiffres.\nGrâce aux informations "
                                    "donnés par ces chiffres, le joueur peut localiser l'emplacement des mines. Si le joueur touche une mine "
                                    "la partie est terminée.\n\nLe joueur a la possibilité de déposer des drapeaux (clic droit) pour signaler "
                                    "l'emplacement des mines, le joueur peut retirer ces drapeaux (avec un deuxième clic droit), il ne peut "
                                    "néanmoins pas placer \nplus de drapeaux que le nombre de mines.\n\nLe jeu est chronomètré et les cinq "
                                    "meilleurs scores de chaque niveau de difficulté seront conservés.\nLe chronomètre se mettra sur pause "
                                    "lorsque le joueur sélectionne divers options (aide, menu option, meilleurs scores...).\nLe chronomètre "
                                    "se réenclenchera lorsque les précédentes fenêtres seront fermées.\n\nLe joueur a la possibilité de "
                                    "sélectionner trois modes de jeux:\n - débutant: 10 mines et grille 10x10\n - intermédiaire: 40 mines et "
                                    " grille 16x16\n - expert: 99 mines et grille 24x24.\n\nBonne chance et amusez-vous :-)",fenaide);
    labelaide->setFont(maPolice);
    layoutaide->addWidget(labelaide);
    fenaide->setLayout(layoutaide);

    //Fenetre de dialogue pour l'"A Propos"
    QDialog *fenapropos = new QDialog(zoneCentrale);
    QVBoxLayout *layoutapropos = new QVBoxLayout;
    QLabel *labelapropos = new QLabel("Ce logiciel a été crée par Cédric D.\n\n2016\nTous droits réservés",fenaide);
    labelapropos->setFont(maPolice);
    layoutapropos->addWidget(labelapropos);
    fenapropos->setLayout(layoutapropos);

    //Création des widgets utiles à l'application
    m_tab = new TableauJeu(this);
    m_lcdBombes = new QLCDNumber(this);
    m_lcdTemps = new LCDNumber_p(this);
    m_labelSmiley = new Label_p(this);

    //Appel aux fonctions d'initialisations
    init();

    //Création des layouts pour le placement des widgets
    QHBoxLayout *hboxlayout = new QHBoxLayout;
    hboxlayout->addWidget(m_lcdBombes,0,Qt::AlignHCenter & Qt::AlignLeft);
    hboxlayout->addWidget(m_labelSmiley,1,Qt::AlignHCenter);
    hboxlayout->addWidget(m_lcdTemps,2,Qt::AlignHCenter & Qt::AlignRight);

    QVBoxLayout *layoutprincipal = new QVBoxLayout;
    layoutprincipal->addLayout(hboxlayout,0);
    layoutprincipal->addWidget(m_tab,1,Qt::AlignHCenter);

    //Interactions avec l'utilisateur (appui sur un bouton, selections des paramètres ...)
    connect(actionNvlPartie,SIGNAL(triggered(bool)),this,SLOT(nouvellePartie()));
    connect(m_difficulte.getboutonOK(),SIGNAL(clicked()),this,SLOT(nouvellePartie()));
    connect(&m_difficulte,SIGNAL(difficultefenferme()),this,SLOT(timerTempsRedeclenche()));
    connect(actionOptions,SIGNAL(triggered(bool)),this,SLOT(options()));
    connect(actionMeilleursScores,SIGNAL(triggered(bool)),this,SLOT(meilleursScores()));
    connect(m_meilleurScores.getBoutonOK(),SIGNAL(clicked(bool)),this,SLOT(finMeilleursScores()));
    connect(&m_meilleurScores,SIGNAL(meilleurscoresfenferme()),this,SLOT(timerTempsRedeclenche()));
    connect(m_nomJoueur.getValidationNom(),SIGNAL(clicked(bool)),this,SLOT(finSaisiNom()));
    connect(actionQuitter,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(actionAide,SIGNAL(triggered(bool)),this,SLOT(timerTempsPause()));
    connect(actionAide,SIGNAL(triggered(bool)),fenaide,SLOT(exec()));
    connect(fenaide,SIGNAL(rejected()),this,SLOT(timerTempsRedeclenche()));
    connect(actionAPropos,SIGNAL(triggered(bool)),this,SLOT(timerTempsPause()));
    connect(actionAPropos,SIGNAL(triggered(bool)),fenapropos,SLOT(exec()));
    connect(fenapropos,SIGNAL(rejected()),this,SLOT(timerTempsRedeclenche()));
    connect(m_tab, SIGNAL(rightClicked(int,int)), this, SLOT(placeDrapeau(int,int)));
    connect(m_tab, SIGNAL(leftClicked(int,int)), this, SLOT(algo(int,int)));
    connect(m_lcdTemps->getTimer(),SIGNAL(timeout()),m_lcdTemps,SLOT(tick()));
    connect(m_labelSmiley,SIGNAL(leftClicked()),this,SLOT(nouvellePartie()));

    zoneCentrale->setLayout(layoutprincipal);
    setCentralWidget(zoneCentrale);
}

//**********************************************************************************************************************************
//  SLOT appelé après un click gauche sur une des cases du tableau
void DemineurFen::algo(int row, int column)
{
    //S'il y a un drapeau déjà présent sur la coordonnée, on ne fait rien
    if(!testCoord(TabPlacementDrapeau,row,column))
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        QVector<int> tabInter;
        Coord c;
        int i(0),j(0),cpt(0);

        //On teste si une mine se trouve sur la coordonnée (i.e la partie est perdue)
        if(m_tabcachee[row][column] == 64)
        {
            m_labelSmiley->setPixmap(QPixmap("smiley_triste.jpeg"));
            m_lcdTemps->getTimer()->stop();
            m_partieTerminee=true;
            m_tab->setItem(row,column,item);
            item->setData(Qt::DecorationRole, QPixmap("bombe.jpeg"));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            QMessageBox::information(this,"Demineur","Vous avez perdu la partie !!! \n");
            m_tab->setEnabled(false);
            while(tabInter.size()!=0)
            {
                tabInter.pop_back();
            }
        }
        else{
            m_tab->setItem(row,column,item);

            //Sinon on teste si la coordonnée contient un chiffre différent de zéro et on l'affiche
            if(m_tabcachee[row][column] != 0)
            {
                item->setText(QString::number(m_tabcachee[row][column]));
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                item->setTextAlignment(5);
                item->setTextColor(couleurNombre(m_tabcachee[row][column]));
                item->setFont(QFont("Arial",9,QFont::Bold, true));
                if(!testCoord(TabNonBombes,row,column))
                {
                    c.row=row;
                    c.column=column;
                    TabNonBombes.push_back(c);
                    m_nbCases--;
                }
            }
            else
            {
                //Si la coordonnée est égale à zéro, on effectue un algorithme permettant d'afficher tous les zéros et les chiffres adjacents
                setTabZeroBombes(row,column);
                if(TabZeroBombes.size()!=0)
                {
                    do{
                        tabInter.push_back(TabZeroBombes.size());
                        for(i=cpt;i<tabInter.back();i++)
                        {
                            c=TabZeroBombes[i];
                            setTabZeroBombes(c.row,c.column);
                            cpt=cpt+1;
                        }
                    }while(cpt!=TabZeroBombes.size());
                }

                //On teste si un drapeau etait déjà présent avant sur la zone (via l'algorihme) lorsque l'on clique sur une coordonnée avec un zéro
                for(i=0;i<TabNonBombes.size();i++)
                {
                    for(j=0;j<TabPlacementDrapeau.size();j++)
                    {
                        c=TabNonBombes[i];
                        if(c==TabPlacementDrapeau[j])
                        {
                            int indice=indicePlacementDrapeau(c.row,c.column);
                            TabPlacementDrapeau.remove(indice);
                            m_nbBombesRestantDrapeau++;
                            m_lcdBombes->display(m_nbBombesRestantDrapeau);
                        }
                    }
                }
            }

            //Si le nombre de cases découvert est inférieur ou égale au nombre de mines, la partie est gagnée
            if(m_nbCases<=m_nbBombes)
            {
                m_labelSmiley->setPixmap(QPixmap("smiley_heureux.jpeg"));
                m_lcdTemps->getTimer()->stop();
                m_partieTerminee=true;
                QMessageBox::information(this,"Demineur","Félicitation !!! \nVous avez mis " + QString::number(m_lcdTemps->getTemps()) + " secondes" );
                m_tab->setEnabled(false);
                m_nomJoueur.setWindowModality(Qt::ApplicationModal);
                m_nomJoueur.show();
                while(tabInter.size()!=0)
                {
                    tabInter.pop_back();
                }
            }
        }
    }
}

//  SLOT appelé après un click droit sur une des cases du tableau
void DemineurFen::placeDrapeau(int row, int column)
{
    //On teste si la coordonnée n'a pas déjà été dévoilée (click gauche), si c'est le cas, on ne fait rien
    if(!testCoord(TabNonBombes,row,column))
    {
        //On teste si le nombre de drapeaux disponibles est égale à zéro
        if(m_nbBombesRestantDrapeau != 0)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            m_tab->setItem(row,column,item);

            //La coordonnée n'a pas été dévoilée et il reste des drapeaux de disponibles. On teste s'il y a déjà un drapeau sur la coordonnée
            if(!testCoord(TabPlacementDrapeau,row,column))
            {
                //Un drapeau n'est pas présent, on place alors le drapeau et on décrémente le compteur de mines (drapeaux)
                Coord c;
                c.row=row;
                c.column=column;
                TabPlacementDrapeau.push_back(c);
                item->setData(Qt::DecorationRole, QPixmap("drapeau.jpeg"));
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                m_nbBombesRestantDrapeau--;
            }
            else{
                //Un drapeau est déjà présent, on retire le drapeau et on incrémente le compteur de mines (drapeaux)
                int indice=indicePlacementDrapeau(row,column);
                TabPlacementDrapeau.remove(indice);
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                item->setBackground(QBrush(Qt::lightGray));
                m_nbBombesRestantDrapeau++;
            }
        }
        //Il ne reste plus de drapeaux disponibles, on peut néanmoins les retirer sur les coordonnées contenant déjà un drapeau
        else if(testCoord(TabPlacementDrapeau,row,column))
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            m_tab->setItem(row,column,item);
            int indice=indicePlacementDrapeau(row,column);
            TabPlacementDrapeau.remove(indice);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            item->setBackground(QBrush(Qt::lightGray));
            m_nbBombesRestantDrapeau++;
        }
    }
    m_lcdBombes->display(m_nbBombesRestantDrapeau);
}

//  SLOT permettant de sélectionner un des trois modes de jeux (débutant, intermédiaire ou expert)
void DemineurFen::options()
{
    m_difficulte.setWindowModality(Qt::ApplicationModal);
    m_difficulte.show();
    m_lcdTemps->getTimer()->stop();
}

//  SLOT permettant de lancer une nouvelle partie
void DemineurFen::nouvellePartie()
{
    init();
    m_tab->setEnabled(true);
}

//  SLOT permettant d'afficher la fenêtre des meilleurs scores des trois modes de jeux
void DemineurFen::meilleursScores()
{
    m_meilleurScores.setWindowModality(Qt::ApplicationModal);
    m_meilleurScores.show();
    m_lcdTemps->getTimer()->stop();
}

//  SLOT pour la fermeture de la fenêtre des meilleurs scores (croix rouge) afin de relancer le chronomètre
void DemineurFen::finMeilleursScores()
{
    m_meilleurScores.close();
    if(m_partieTerminee==false)
    {
        m_lcdTemps->getTimer()->start(1000 + m_lcdTemps->getTemps());
    }
}

//  SLOT permettant d'inscrire (ou non selon le cas échéant) le score dans les cinq meilleurs scores
void DemineurFen::finSaisiNom()
{
    m_nomJoueur.close();
    m_meilleurScores.actualisationHistorique(m_nomJoueur.getNomJoueur(),m_lcdTemps->getTemps(),getSelectionDifficulte());
}

//  SLOT permettant de mettre en pause le chronomètre
void DemineurFen::timerTempsPause()
{
    m_lcdTemps->getTimer()->stop();
}

//  SLOT permettant de relancer le chronomètre si la partie n'est pas terminée
void DemineurFen::timerTempsRedeclenche()
{
    if(m_partieTerminee==false)
    {
        m_lcdTemps->getTimer()->start(1000 + m_lcdTemps->getTemps());
    }
}

//  SLOT ré-implantant (closeEvent) lors de la fermeture de l'application (croix rouge)
void DemineurFen::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this,"Demineur",tr("Voulez-vous quitter?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        QSettings setting("scores.ini",QSettings::IniFormat);

        QVector<MScores> tabDebutant=m_meilleurScores.getScoreDebutant();
        QVector<MScores> tabIntermediaire=m_meilleurScores.getScoreIntermediaire();
        QVector<MScores> tabExpert=m_meilleurScores.getScoreExpert();

        //Debutant
        for(int i=0;i<tabDebutant.size();i++)
        {
            setting.setValue("Debutant/" + QString::number(i+1) + "/Nom",QString::fromStdString(tabDebutant[i].nom));
            setting.setValue("Debutant/" + QString::number(i+1) + "/Temps",tabDebutant[i].temps);
        }

        //Intermediaire
        for(int i=0;i<tabIntermediaire.size();i++)
        {
            setting.setValue("Intermediaire/" + QString::number(i+1) + "/Nom",QString::fromStdString(tabIntermediaire[i].nom));
            setting.setValue("Intermediaire/" + QString::number(i+1) + "/Temps",tabIntermediaire[i].temps);
        }

        //Expert
        for(int i=0;i<tabDebutant.size();i++)
        {
            setting.setValue("Expert/" + QString::number(i+1) + "/Nom" ,QString::fromStdString(tabExpert[i].nom));
            setting.setValue("Expert/" + QString::number(i+1) + "/Temps",tabExpert[i].temps);
        }

        while(tabDebutant.size()!=0)
        {
            tabDebutant.pop_back();
        }
        while(tabIntermediaire.size()!=0)
        {
            tabIntermediaire.pop_back();
        }
        while(tabExpert.size()!=0)
        {
            tabExpert.pop_back();
        }
        desempilementPileBombes();
        desempilement();
        m_meilleurScores.desempilement();
        desallocationdynamique();
        event->accept();
    }
}

//**********************************************************************************************************************************
//  METHODE d'initialisation des attributs et variables appelées dans le constructeur et pour toute nouvelle partie
void DemineurFen::init()
{
    int i(0),j(0),nb(0);
    m_tab->clear();

    nbBombesvsDifficulte();             //initialise le nombre de mines et la taille du tableau en fonction du mode de jeu
    m_tab->setRowCount(m_N);
    m_tab->setColumnCount(m_N);
    m_tab->resizeColumnsToContents();
    m_tab->resizeRowsToContents();
    m_tab->setMouseTracking(true);      //utilisé pour la méthode mouseMoveEvent

    m_nbBombesRestantDrapeau=m_nbBombes;
    m_lcdBombes->setSegmentStyle(QLCDNumber::Flat);
    m_lcdBombes->setFixedSize(80,30);
    m_lcdBombes->display(m_nbBombes);

    m_lcdTemps->setSegmentStyle(QLCDNumber::Flat);
    m_lcdTemps->setFixedSize(80,30);
    m_lcdTemps->setTemps(0);
    m_lcdTemps->getTimer()->start(1000);

    m_labelSmiley->setPixmap(QPixmap("smiley.jpeg"));
    m_nbCases=m_N * m_N;
    m_partieTerminee=false;

    allocationdynamique();
    desempilementPileBombes();
    placeBombes(m_tabcachee,m_nbBombes,m_N);    //placement aléatoire des mines dans le tableau (tabcachee)

    for (i=0; i<m_N; i++)
    {
        for (j=0; j<m_N; j++)
        {
            nb=nbBombesAlentour(m_tabcachee,m_N,i,j);   //placement des chiffres dans le tableau (tabcachee). m_tabcachee contient la solution
            if(m_tabcachee[i][j] != 64)
            {
                m_tabcachee[i][j]=nb;
            }
        }
    }

    QTableWidgetItem *item=NULL;
    for (i=0; i<m_N; i++)
    {
        for (j=0; j<m_N; j++)
        {

            item = new QTableWidgetItem();
            m_tab->setItem(i,j,item);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            item->setBackground(QBrush(Qt::lightGray));
         }
    }
    desempilement();
}

//  METHODE initialisant le nombre de mines et la taille du tableau en fonction du mode de jeu
void DemineurFen::nbBombesvsDifficulte()
{
    if(m_difficulte.getSelection_debutant())
    {
        m_nbBombes=10;
        m_N=10;
        m_tab->setFixedSize(420,270);
    }
    else if (m_difficulte.getSelection_intermediaire())
    {
        m_nbBombes=40;
        m_N=16;
        m_tab->setFixedSize(650,400);
    }
    else if (m_difficulte.getSelection_expert())
    {
        m_nbBombes=99;
        m_N=24;
        m_tab->setFixedSize(980,590);
    }
}

//  METHODE pour l'allocation dynamique du tableau à deux dimensions (m_tabcachee)
void DemineurFen::allocationdynamique()
{
    int i(0);
    m_tabcachee = new int *[m_N];
    for(i=0;i<m_N;i++)
    {
        m_tabcachee[i] = new int[m_N];
    }
}

//  METHODE pour la désallocation dynamique du tableau à deux dimensions (m_tabcachee)
void DemineurFen::desallocationdynamique()
{
    int i(0);
    for(i=0;i<m_N;i++)
    {
        delete [] m_tabcachee[i];
    }
    delete [] m_tabcachee;
}

//  METHODE pour le désempilement de tous les tableaux dynamiques  (QVector)
void DemineurFen::desempilement()
{
    while(TabZeroBombes.size()!=0)
    {
        TabZeroBombes.pop_back();
    }
    while(TabNonBombes.size()!=0)
    {
        TabNonBombes.pop_back();
    }
    while(TabPlacementDrapeau.size()!=0)
    {
        TabPlacementDrapeau.pop_back();
    }
}

//  METHODE algorithmique permettant d'afficher toutes les cases adjacentes lorsque la coordonnée sélectionnée est égale à zéro
void DemineurFen::setTabZeroBombes(int row, int column)
{
    QTableWidgetItem *item = NULL;
    Coord c;
    int i(row-1),j(column-1);

    //On teste les huit cases adjacentes de la coordonnée
    for (i=row-1; i<=row+1; i++)
    {
        for (j=column-1; j<=column+1; j++)
        {
            //On teste si les indices ne sortent pas du tableau
            if((i >= 0 && i < m_N) && (j >= 0 && j < m_N))
            {
                //Si parmis les huit coordonnées, une coordonnée n'a pas été selectionnée précédemment, on la stocke (dans TabNonBombes)
                if(!testCoord(TabNonBombes,i,j))
                {
                    c.row=i;
                    c.column=j;
                    TabNonBombes.push_back(c);
                    m_nbCases--;
                }

                item = new QTableWidgetItem();
                m_tab->setItem(i,j,item);
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                //Si parmi les huit coordonnées, une coordonnée contient un chiffre différent de zéro, on l'affiche
                if(m_tabcachee[i][j]!=0){
                    item->setTextColor(couleurNombre(m_tabcachee[i][j]));
                    item->setFont(QFont("Arial",9,QFont::Bold, true));
                    item->setText(QString::number(m_tabcachee[i][j]));
                    item->setTextAlignment(5);
                }
                //Si parmi les huit coordonnées, une coordonnée contient également un zéro, on stocke cette coordonnée (dans TabZeroBombes)
                if(m_tabcachee [i][j]==0)
                {
                    if(!testCoord(TabZeroBombes,i,j))
                    {
                        c.row=i;
                        c.column=j;
                        TabZeroBombes.push_back(c);
                    }
                }
            }
        }
    }
}

//  METHODE permettant de tester si une coordonnée est présente dans le tableau dynamique
bool DemineurFen::testCoord(QVector <Coord> tab,int row,int column)
{
    bool test=false;
    int i=0;
    Coord c;
    for(i=0;i<tab.size();i++)
    {
        c=tab[i];
        if(c.row==row && c.column==column)
        {
            test=true;
        }
    }
    return test;
}

//  METHODE renvoyant l'indice de la coordonnée contenue dans le tableau TabPlacementDrapeau
int  DemineurFen::indicePlacementDrapeau(int row,int column)
{
    int indice=0;
    int i=0;
    Coord c;
    for(i=0;i<TabPlacementDrapeau.size();i++)
    {
        c=TabPlacementDrapeau[i];
        if(c.row==row && c.column==column)
        {
            indice=i;
        }
    }
    return indice;
}

//  METHODE permettant de sélectionner la couleur en fonction d'un chiffre
QColor DemineurFen::couleurNombre(int nb)
{
    QColor cl;
    switch (nb){
        case 1: cl=Qt::blue;
                break;
        case 2: cl=Qt::darkGreen;
                break;
        case 3: cl=Qt::red;
                break;
        case 4: cl=Qt::darkBlue;
                break;
        case 5: cl=Qt::darkRed;
                break;
        case 6: cl=Qt::darkCyan;
                break;
        case 7: cl=Qt::magenta;
                break;
        case 8: cl=Qt::darkMagenta;
                break;
        default: cl=Qt::black;
                break;
    }
    return cl;
}

//  METHODE renvoyant une valeur entière selon le mode de jeu activé
int DemineurFen::getSelectionDifficulte()
{
    int nb=0;
    if(m_difficulte.getSelection_debutant())
    {
        nb=1;
    }
    else if (m_difficulte.getSelection_intermediaire())
    {
        nb=2;
    }
    else if (m_difficulte.getSelection_expert())
    {
        nb=3;
    }
    return nb;
}

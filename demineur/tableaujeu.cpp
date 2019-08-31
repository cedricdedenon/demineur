#include "tableaujeu.hpp"

//**********************************************************************************************************************************
//  CONSTRUCTEUR
TableauJeu::TableauJeu(QWidget *parent) :
    QTableWidget(parent)
{
}

//**********************************************************************************************************************************
//  SLOT ré-implantant (mousePressEvent) pour prendre en compte l'intéraction d'un click gauche et d'un click droit
void TableauJeu::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
    {
        emit rightClicked(this->currentRow(),this->currentColumn());
    }
      else if (e->button() == Qt::LeftButton){
        emit leftClicked(this->currentRow(),this->currentColumn());
      }
    else{
        QTableWidget::mousePressEvent(e);
    }
}

//  SLOT ré-implantant (mouseMoveEvent) pour émettre le signal mousePressEvent
void TableauJeu::mouseMoveEvent(QMouseEvent *e)
{
    emit TableauJeu::mousePressEvent(e);
}

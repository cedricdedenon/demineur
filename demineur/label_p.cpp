#include "label_p.hpp"

//**********************************************************************************************************************************
//  CONSTRUCTEUR
Label_p::Label_p(QWidget *parent) :
    QLabel(parent)
{
}

//**********************************************************************************************************************************
//  SLOT ré-implantant (mousePressEvent) pour prendre en compte l'intéraction d'un click gauche sur le QLabel
void Label_p::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        emit leftClicked();
    }
}

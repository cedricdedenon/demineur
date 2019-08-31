#ifndef TABLEAUJEU_HPP
#define TABLEAUJEU_HPP

#include <QtWidgets>
#include <QTableWidget>

//**********************************************************************************************************************************
//  CLASSE
class TableauJeu: public QTableWidget
{
Q_OBJECT

public:
    explicit TableauJeu(QWidget *parent = 0);

private slots:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:
    void rightClicked(int,int);
    void leftClicked(int,int);

};
#endif // TABLEAUJEU_HPP

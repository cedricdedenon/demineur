#ifndef LABEL_P_HPP
#define LABEL_P_HPP

#include <QApplication>
#include <QtWidgets>

//**********************************************************************************************************************************
//  CLASSE
class Label_p: public QLabel
{
Q_OBJECT

public:
    Label_p(QWidget *parent = 0);

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void leftClicked();
};

#endif // LABEL_P_HPP

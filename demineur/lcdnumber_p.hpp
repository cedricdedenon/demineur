#ifndef LCDNUMBER_P_HPP
#define LCDNUMBER_P_HPP

#include <QApplication>
#include <QtWidgets>

//**********************************************************************************************************************************
//  CLASSE
class LCDNumber_p: public QLCDNumber
{
Q_OBJECT

public:
    LCDNumber_p(QWidget *parent = 0);

public slots:
    void tick();

private:
    QTimer *m_timer;
    int m_temps;

public:
    QTimer* getTimer();
    int getTemps();
    void setTemps(int t);
};

#endif // LCDNUMBER_P_HPP

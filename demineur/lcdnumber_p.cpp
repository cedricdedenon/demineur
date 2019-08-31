#include "lcdnumber_p.hpp"

//**********************************************************************************************************************************
//  CONSTRUCTEUR
LCDNumber_p::LCDNumber_p(QWidget *parent) :
    QLCDNumber(parent)
{
    m_timer = new QTimer();
    m_temps = 0;
    this->display(m_temps);
}

//**********************************************************************************************************************************
//  SLOT incrémente la variable temps d'une unité et l'affiche sur le QLCDNumber
void LCDNumber_p::tick()
{
    m_temps++;
    this->display(m_temps);
}

//**********************************************************************************************************************************
//  METHODE accesseur sur l'attribut m_timer
QTimer* LCDNumber_p::getTimer()
{
    return m_timer;
}

//  METHODE accesseur (get) sur l'attribut m_temps
int LCDNumber_p::getTemps()
{
    return m_temps;
}

//  METHODE accesseur (set) sur l'attribut m_temps
void LCDNumber_p::setTemps(int t)
{
    m_temps=t;
}


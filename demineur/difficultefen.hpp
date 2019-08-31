#ifndef DIFFICULTEFEN_HPP
#define DIFFICULTEFEN_HPP

#include <QApplication>
#include <QtWidgets>

//**********************************************************************************************************************************
//  CLASSE
class DifficulteFen: public QWidget
{
Q_OBJECT

public:
    DifficulteFen();

private:
    QRadioButton *m_debutant;
    QRadioButton *m_intermediaire;
    QRadioButton *m_expert;
    QPushButton *m_boutonOK;

private slots:
    void closeEvent(QCloseEvent *event);

public:
    bool getSelection_debutant();
    bool getSelection_intermediaire();
    bool getSelection_expert();
    QPushButton* getboutonOK();

signals:
    void difficultefenferme();
};
#endif // DIFFICULTEFEN_HPP

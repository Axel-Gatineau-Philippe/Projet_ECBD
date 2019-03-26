#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>

#include "charger_csv.h"

class MaFenetre : public QMainWindow {
    Q_OBJECT

public slots:
    void setQuitter();
    void setPredire();

public:
    MaFenetre(QWidget *parent = 0);

private:
    QPushButton *m_bouton_quitter, *m_bouton_valider;
    QComboBox *m_fievre, *m_douleur, *m_toux;
    QLabel *m_label_nom, *m_label_prenom, *m_label_fievre, *m_label_douleur, *m_label_toux, *m_label_maladie;
    QLineEdit *m_edit_nom, *m_edit_prenom;
    CVString m_vet;
    CMatString m_mat;
    QString maladie, nom, prenom;
    QTableWidget *m_tab_maladie;
    void remplirListe(int colonne, QComboBox *com);
    double calculerConf(QString maladie, QString symptome, int colonne);
    double calculerFreq(QString maladie);
    double calculerScore(QString maladie);
    void remplirTableau();
};

#endif // MAFENETRE_H

#include <vector>
#include<algorithm>
#include "MaFenetre.h"


using namespace std;

MaFenetre::MaFenetre(QWidget *parent) : QMainWindow(parent) {
    // Lecture des données
    read_csv(m_mat, m_vet, "data.csv");

    setFixedSize(1000,800);
    setWindowTitle("Classification des patients - Prédiction");

    // Label nom et prénom
    m_label_nom = new QLabel("Nom :", this);
    m_label_nom->setGeometry(80, 50, 100, 30);

    m_label_prenom = new QLabel("Prenom :", this);
    m_label_prenom->setGeometry(80, 90, 100, 30);

    // LineEdit nom et prénom
    m_edit_nom = new QLineEdit(this);
    m_edit_nom->setGeometry(130, 50, 200, 30);

    m_edit_prenom = new QLineEdit(this);
    m_edit_prenom->setGeometry(130, 90, 200, 30);

    // Label des QComboBox pour l'affichages des symptômes
    m_label_fievre = new QLabel("Fièvre :", this);
    m_label_fievre->setGeometry(250, 150, 100, 30);

    m_label_douleur = new QLabel("Douleur :", this);
    m_label_douleur->setGeometry(350, 150, 100, 30);

    m_label_toux = new QLabel("Toux :", this);
    m_label_toux->setGeometry(450, 150, 100, 30);

    // Les trois QComboBox pour le choix des symptômes
    m_fievre = new QComboBox(this);
    m_fievre->setGeometry(250,200,100,30);

    m_douleur = new QComboBox(this);
    m_douleur->setGeometry(350,200,100,30);

    m_toux = new QComboBox(this);
    m_toux->setGeometry(450,200,100,30);

    remplirListe(0, m_fievre);
    remplirListe(1, m_douleur);
    remplirListe(2, m_toux);

    // Affichage de la prediction
    m_label_maladie = new QLabel(this);
    m_label_maladie->setFont(QFont("Arial", 10, true));
    m_label_maladie->setGeometry(250, 600, 800, 100.);


    // Affichage du tableau
    m_tab_maladie = new QTableWidget(m_mat.size() + 1, m_vet.size(), this);
    m_tab_maladie->setGeometry(220, 250, 425, 340);
    m_tab_maladie->resize(425,340);
    remplirTableau();


    // Bouton Quitter
    m_bouton_quitter = new QPushButton("Quitter", this);
    m_bouton_quitter->setGeometry(800,400,80,30);
    connect(m_bouton_quitter, SIGNAL(clicked()), this, SLOT(setQuitter()));

    // Bouton Prédire
    m_bouton_valider = new QPushButton("Prédire", this);
    m_bouton_valider->setGeometry(800,500,80,30);
    connect(m_bouton_valider, SIGNAL(clicked()), this, SLOT(setPredire()));
}

void MaFenetre::remplirListe(int colonne, QComboBox *com) {
    // Le premier élément est NULL
    string titre = "NULL";
    com->addItem(QString::fromUtf8(titre.c_str()));

    // Récupérer les valeurs de data.csv
    vector<string> valeurs;
    for(unsigned j = 0; j < m_mat.size(); ++j) {
        valeurs.push_back(m_mat[j][colonne]);
    }

    // Supprimer les doublons
    sort(valeurs.begin(), valeurs.end());
    valeurs.erase(unique(valeurs.begin(), valeurs.end()), valeurs.end());

    // Remplir les QComboBox
    for(string valeur : valeurs) {
        com->addItem(QString::fromUtf8(valeur.c_str()));
    }
}


double MaFenetre::calculerConf(QString maladie, QString symptome, int colonne){
    int cpt = 0;
    unsigned i = 0;
    while(i < 9){
        if(m_mat[i][3] == maladie.toStdString() && m_mat[i][colonne] == symptome.toStdString())
            ++cpt;
            ++i;
    }

    float conf = cpt/9.0;
    return conf;
}

double MaFenetre::calculerFreq(QString maladie){
    int cpt = 0;
    unsigned i = 0;
    while(i < 9){
        if(m_mat[i][3] == maladie.toStdString())
            ++cpt;
            ++i;
    }
    float freq = cpt/9.0;
    return freq;
}

double MaFenetre::calculerScore(QString maladie){
    QString fievre = m_fievre->currentText();
    QString douleur = m_douleur->currentText();
    QString toux = m_toux->currentText();

    double confFievre;
    double confDouleur;
    double confToux;
    double score;
    double freq;

    freq = calculerFreq(maladie);

    if (fievre == "NULL")
        confFievre = 1;
    else
        confFievre = calculerConf(maladie, fievre, 0)/freq;

    if (douleur == "NULL")
        confDouleur = 1;
    else
        confDouleur = calculerConf(maladie, douleur, 1)/freq;

    if(toux == "NULL")
        confToux = 1;
    else
        confToux = calculerConf(maladie, toux, 2)/freq;

    score = freq * confFievre * confDouleur * confToux;

    return score;
}

void MaFenetre::setQuitter() {
    // Arrêter l'application
    exit(0);
}

void MaFenetre::setPredire() {
    double scoreAppendicite;
    double scoreRhume;
    double scoreMalDeGorge;
    double scoreAucune;
    double scoreRefroidissement;

    scoreAppendicite = calculerScore("Appendicite");
    scoreRhume = calculerScore("Rhume");
    scoreMalDeGorge = calculerScore("Mal de gorge");
    scoreAucune = calculerScore("Aucune");
    scoreRefroidissement = calculerScore("Refroidissement");

    vector<float> scores;

    vector<QString> maladies;

    QString a = "Appendicite";
    QString b = "Rhume";
    QString c = "Mal de gorge";
    QString d = "Aucune";
    QString e = "Refroidissement";

    maladies.push_back(a);
    maladies.push_back(b);
    maladies.push_back(c);
    maladies.push_back(d);
    maladies.push_back(e);

    scores.push_back(scoreAppendicite);
    scores.push_back(scoreRhume);
    scores.push_back(scoreMalDeGorge);
    scores.push_back(scoreAucune);
    scores.push_back(scoreRefroidissement);

    float scoreMax = 0.0;
    for (unsigned i=0; i < scores.size(); ++i){
        if (scores[i] > scoreMax){
            scoreMax = scores[i];
            maladie = maladies[i];
        }
    }

    /*maladie = "";
    if(scoreMax == scoreAppendicite)
        maladie = a;
    else if(scoreMax == scoreRhume)
        maladie = b;
    else if (scoreMax == scoreMalDeGorge)
        maladie =c;
    else if (scoreMax == scoreRefroidissement)
        maladie = e;
    else
        maladie = d;*/

    prenom = m_edit_prenom->text();
    nom = m_edit_nom->text();
    m_label_maladie->setText("Le patient " + prenom + " " + nom + " à la maladie : " + "\n" + maladie);

}

void MaFenetre::remplirTableau()
{
    for(unsigned i = 0; i < m_vet.size(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(m_vet[i]), 0);
        m_tab_maladie->setItem(0, i, item);

        for(unsigned j = 0; j < m_mat.size(); ++j)
        {

            item = new QTableWidgetItem(QString::fromStdString(m_mat[j][i]), 0);
            m_tab_maladie->setItem(j+1, i, item);
        }
    }
}

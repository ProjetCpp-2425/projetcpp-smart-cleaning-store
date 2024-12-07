#include "dialogscore.h"
#include "conge.h"
#include "qdatetime.h"
#include "ui_dialogscore.h"
#include<QMessageBox>
Dialogscore::Dialogscore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogscore)
{
    ui->setupUi(this);
}

Dialogscore::~Dialogscore()
{
    delete ui;  // Free the memory
}



/*void Dialogscore::on_pushButton_calculerScore_clicked() {
    // Récupérer l'ID employé et l'année depuis les champs de saisie
    bool ok;
    int IDE = ui->lineEdit_IDE_score->text().toInt(&ok);
    if (!ok || IDE <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID employé valide.");
        return;
    }

    int annee = ui->lineEdit_Annee_score->text().toInt(&ok);
    if (!ok || annee < 1900 || annee > QDate::currentDate().year()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une année valide.");
        return;
    }

    // Calculer le score en utilisant la fonction de votre classe `conge`
    conge congeInstance;
    int score = congeInstance.calculerScoreEmploye(IDE, annee); // À implémenter dans votre classe `conge`

    if (score == -1) {
        QMessageBox::critical(this, "Erreur", "Impossible de calculer le score.");
        return;
    }

    // Générer un affichage d'étoiles basé sur le score
    QChar etoile = QChar(0x2B50);  // Unicode for star character '⭐'
    QString etoiles = QString(score, etoile); // Répète l'étoile selon le score
    ui->label_etoiles_score->setText(etoiles);  // Affiche les étoiles
}
*/


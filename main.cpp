#include "mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QtSerialPort>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Étape 1 : Définir le mot de passe correct
    int i=0;
    bool verif=false;
    QString correctPassword = "0000";  // Ce mot de passe est celui à vérifier
    while(i<3 && verif==false)
    {
    // Étape 2 : Afficher une boîte de dialogue pour demander le mot de passe
    bool ok;
    QString enteredPassword = QInputDialog::getText(
        nullptr,                           // Pas de parent car c'est avant MainWindow
        "Mot de passe requis",            // Titre de la boîte de dialogue
        "Entrez le mot de passe :",       // Texte descriptif
        QLineEdit::Password,              // Masquer la saisie
        "",                               // Valeur par défaut
        &ok                               // Vérifie si l'utilisateur clique sur OK ou Annuler
    );

    // Étape 3 : Vérifier si l'utilisateur a cliqué sur "Annuler"
    if (!ok) {
        return 0;  // Quitter l'application si "Annuler" est cliqué
    }
    // Étape 4 : Vérifier le mot de passe

        if (enteredPassword != correctPassword) {
        i++;
        QMessageBox::critical(
            nullptr, "Erreur", " veuillez réessayer !");
    }
        else { verif = true ;}
        if(i==3) {        QMessageBox::critical(
                        nullptr, "Erreur", "Mot de passe incorrect !");
                    return 0;  // Quitter si le mot de passe est incorrect }
    }
    }

    // Étape 5 : Connexion à la base de données
    Connection c;
    bool test = c.createconnection();
    if (!test) {
        QMessageBox::critical(
            nullptr, QObject::tr("Data is not open"),
            QObject::tr("Échec de connexion.\nCliquez sur Annuler pour quitter."),
            QMessageBox::Cancel);
        return 0;  // Quitter si la connexion échoue
    }

    QMessageBox::information(
        nullptr, QObject::tr("Data is open"),
        QObject::tr("Connecté avec succès.\nCliquez sur OK pour continuer."),
        QMessageBox::Ok);

    // Étape 6 : Afficher la fenêtre principale
    MainWindow w;
    w.show();
    return a.exec();
}

#include "conge.h"
#include "conge.h"
#include "qtextcursor.h"
#include "qtextdocument.h"
#include <QSqlQuery>
#include <QVariant>
#include<QSqlError>
#include<QMessageBox>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QDebug>
conge::conge(int IDE, int IDC, QString DATEDEB, QString TYPE, int PERIODE, QString STATUT)
{
    this->IDE=IDE;
    this->IDC=IDC;
    this->DATEDEB=DATEDEB;
    this->TYPE=TYPE;
    this->PERIODE=PERIODE;
    this->STATUT=STATUT;


}

//new
bool conge::ajouterConge(int IDC, int IDE, QString DATEDEB, QString TYPE, int PERIODE, QString STATUT) {
    // Exemple de code pour ajouter le congé
    // Vous pouvez ici ajouter le congé dans une base de données ou une structure de données.
    QSqlQuery query;
    query.prepare("INSERT INTO Conge (IDC, IDE, DATEDEB, TYPE, PERIODE, STATUT) VALUES (:IDC, :IDE, :DATEDEB, :TYPE, :PERIODE, :STATUT)");
    query.bindValue(":IDC", IDC);
    query.bindValue(":IDE", IDE);
    query.bindValue(":DATEDEB", DATEDEB);
    query.bindValue(":TYPE", TYPE);
    query.bindValue(":PERIODE", PERIODE);
    query.bindValue(":STATUT", STATUT);

    if (query.exec()) {
        return true; // Le congé a été ajouté avec succès
    } else {
        return false; // Échec de l'ajout du congé
    }
}
bool conge::mettreAJourStatut(int IDE, const QString &nouveauStatut) {
    // Vérifier le statut actuel de l'employé
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT STATUTE FROM employe WHERE IDE = :IDE");
    checkQuery.bindValue(":IDE", IDE);

    if (!checkQuery.exec()) {
        // Gérer l'erreur de la requête
        qDebug() << "Erreur lors de la vérification du statut de l'employé :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next()) {
        QString statutActuel = checkQuery.value(0).toString();
        if (statutActuel == "inactif") {
            // Si l'employé est déjà inactif, retourner un message d'erreur
            QMessageBox::warning(nullptr, "Erreur", "Employé déjà inactif, impossible d'attribuer un congé.");
            return false; // Indique que la mise à jour n'a pas été effectuée
        }
    } else {
        // Si aucun employé n'est trouvé avec cet ID
        QMessageBox::critical(nullptr, "Erreur", "Aucun employé trouvé avec cet ID.");
        return false;
    }

    // Si le statut n'est pas inactif, procéder à la mise à jour
    QSqlQuery query;
    query.prepare("UPDATE employe SET STATUTE = :nouveauStatut WHERE IDE = :IDE");
    query.bindValue(":nouveauStatut", nouveauStatut);
    query.bindValue(":IDE", IDE);

    return query.exec();
}


QSqlQueryModel * conge::afficher(){
        QSqlQueryModel * model =new QSqlQueryModel();
        model->setQuery("select * from CONGE");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDC"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("IDE"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATEDEB"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("PERIODE"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("TYPE"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("STATUT"));

        return model;
    }

/*int conge::calculerScore(int IDE, int annee)
{
    int joursConge = 0;  // Nombre total de jours de congé
    int joursTravailles = 0;  // Nombre total de jours travaillés (score)

    // Déterminer le nombre total de jours dans l'année (365 ou 366 selon l'année)
    QDate debutAnnee(annee, 1, 1);
    QDate finAnnee(annee, 12, 31);
    int totalJoursAnnee = debutAnnee.daysTo(finAnnee) + 1;  // Inclut le dernier jour de l'année

    // Initialiser les jours travaillés à tous les jours de l'année
    joursTravailles = totalJoursAnnee;

    // Récupérer les congés de l'employé pour l'année spécifiée
    QSqlQuery query;
    query.prepare("SELECT date_debut, periode FROM conge WHERE id_employe = :id "
                  "AND YEAR(DATEDEB) = :annee");  // Utilisation de YEAR(date_debut) pour obtenir l'année
    query.bindValue(":id", IDE);
    query.bindValue(":annee", annee);

    if (query.exec()) {
        while (query.next()) {
            // Récupérer la date de début du congé (en format string)
            QString DATEDEB = query.value(0).toString();  // Date de début sous forme de chaîne
            int periode = query.value(1).toInt();  // Période (nombre de jours de congé)

            // Convertir la chaîne de caractères en QDate
            QString DATEDEB = fromString(DATEDEB, "yyyy-MM-dd");

            // Vérifier si le congé tombe dans l'année spécifiée
            if (DATEDEB.year() == annee) {
                // Ajouter la période de congé au total des jours de congé
                joursConge += periode;

                // Soustraire les jours de congé du nombre de jours travaillés
                joursTravailles -= periode;
            }
        }
    } else {
        qDebug() << "Erreur lors de la récupération des congés:" << query.lastError().text();
    }

    // Retourner le nombre de jours travaillés (score)
    return joursTravailles;
}
*/

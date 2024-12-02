#include "client.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QDate>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

client::client() : cin(0), id(0), nom(""), num(0), date(QDate()), adresse("") {}

QMap<QString, int> client::statistiquesAdresse() {
    QMap<QString, int> statistiques;

    // Requête SQL pour compter les clients par adresse
    QSqlQuery query;
    query.prepare("SELECT adresse, COUNT(*) AS count FROM CLIENT GROUP BY adresse");

    if (query.exec()) {
        while (query.next()) {
            QString adresse = query.value("adresse").toString(); // Colonne "adresse"
            int count = query.value("count").toInt();           // Colonne "count"
            statistiques[adresse] = count;                     // Ajoute au QMap
        }
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
    }

    return statistiques;
}

client::client(int cin, int id, QString nom, int num, QDate date, QString adresse)
    : cin(cin), id(id), nom(nom), num(num), date(date), adresse(adresse) {}

bool client::ajouter() {
    QSqlQuery query;

       // Préparation de la requête SQL
    query.prepare("INSERT INTO CLIENT (CIN, ID, NOM, NUM, DATE_NAIS, ADRESSE) "
                  "VALUES (:CIN, :ID, :NOM, :NUM, TO_DATE(:DATE_NAIS, 'YYYY-MM-DD'), :ADRESSE)");


       // Liaison des valeurs aux paramètres
       query.bindValue(":CIN", cin);
       query.bindValue(":ID", id);
       query.bindValue(":NOM", nom);
       query.bindValue(":NUM", num);
       query.bindValue(":DATE_NAIS", date.toString("yyyy-MM-dd"));
       query.bindValue(":ADRESSE", adresse);

       // Exécuter la requête
       if (!query.exec()) {
           // Si l'exécution échoue, afficher l'erreur
           qDebug() << "Erreur lors de l'ajout du client: " << query.lastError().text();
           return false;
       }

       return true;
}


QSqlQueryModel* client::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();

    // Préparation de la requête sécurisée pour afficher les clients avec l'âge
    model->setQuery("SELECT CIN, ID, NOM, NUM, DATE_NAIS, ADRESSE FROM CLIENT");

    return model;
}

void client::rechercherParId(QTableWidget *tableRecherche, const QString &id) {
    // Préparer la requête SQL pour rechercher le client par ID
    QSqlQuery query;
    query.prepare("SELECT CIN, ID, NOM, NUM, DATE_NAIS, ADRESSE "
                  "FROM CLIENT WHERE ID = :id");
    query.bindValue(":id", id);

    // Exécuter la requête
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        return;
    }

    // Vider la table avant d'afficher les résultats
    tableRecherche->clearContents();
    tableRecherche->setRowCount(0);

    // Si des résultats ont été trouvés
    if (query.next()) {
        // Ajouter une ligne dans la table pour chaque résultat trouvé
        int row = tableRecherche->rowCount();
        tableRecherche->insertRow(row);

        // Insérer les données dans chaque cellule de la ligne
        tableRecherche->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));  // CIN
        tableRecherche->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));  // ID
        tableRecherche->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));  // NOM
        tableRecherche->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));  // NUM
        tableRecherche->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));  // AGE
        tableRecherche->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));  // ADRESSE
    } else {
        qDebug() << "Aucun résultat trouvé pour l'ID : " << id;
    }
}

bool client::supprimer(int id) {
    QSqlQuery query;

    // Préparation de la requête pour supprimer un client en fonction de son ID
    query.prepare("DELETE FROM CLIENT WHERE ID = :id");

    // Liaison de l'ID au paramètre ":id"
    query.addBindValue(id);

    return query.exec();
}

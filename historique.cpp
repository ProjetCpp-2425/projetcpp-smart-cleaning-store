#include "historique.h"

#include <QSqlError>


// Constructeur par défaut
Historique::Historique() {}

// Méthode pour ajouter une action à l'historique
bool Historique::ajouter(const QString &action, const QString &details) {
    QSqlQuery query;
    query.prepare("INSERT INTO HISTORIQUE (ACTION, DETAILS) VALUES (:action, :details)");
    query.bindValue(":action", action);
    query.bindValue(":details", details);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout à l'historique:" << query.lastError().text();
        return false;
    }
    return true;
}

// Méthode pour afficher l'historique
QSqlQueryModel* Historique::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM HISTORIQUE ORDER BY DATE_ACTION DESC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Action"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Détails"));

    return model;
}

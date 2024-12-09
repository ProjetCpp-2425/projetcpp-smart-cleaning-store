#include "produit.h"
#include "qsqlerror.h"
#include "connection.h"
#include "historique.h"
#include <QDebug>

// Debugging function to check database connection
bool checkDatabaseConnection() {
    QSqlDatabase db = QSqlDatabase::database(); // Get the default database connection
    if (!db.isOpen()) {
        if (!db.open()) {
            //qDebug() << "Failed to open database:" << db.lastError().text();
            //return false;
        }
    }
    return true;
}
//
// Définition du constructeur
Produit::Produit(int IDP, QString NomP, int prixP, QString quentit,QString dateP,QString categorie) {
    this->IDP = IDP;
    this->NomP = NomP;
    this->prixP = prixP;
    this->quentit = quentit;
    this->dateP = dateP;
    this->categorie = categorie;
}
bool Produit::ajouter() {
    // Vérifier que le prix est supérieur à zéro
    if (prixP <= 0) {
        qDebug() << "Erreur : Le prix doit être supérieur à zéro.";
        return false;
    }

    // Vérifier que la quantité est un nombre valide et non négatif
    bool ok;
    int quantiteInt = quentit.toInt(&ok); // Convertir la quantité en entier
    if (!ok || quantiteInt < 0) {
        qDebug() << "Erreur : La quantité doit être un nombre entier positif.";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO PRODUIT (IDP, NOMP, PRIXP , QUANTITE, DATEP , CATEGORIE) "
                  "VALUES (:IDP, :NomP, :prixP,  :quentit, :dateP, :categorie)");

    query.bindValue(":IDP", IDP);
    query.bindValue(":NomP", NomP);
    query.bindValue(":prixP", prixP);
    query.bindValue(":quentit", quentit);  // La quantité reste une chaîne dans la base de données
    query.bindValue(":dateP", dateP);
    query.bindValue(":categorie", categorie);

    if (!query.exec()) {
        qDebug() << "Erreur d'ajout de produit:" << query.lastError().text();
        return false;
    }
    // Enregistrement dans l'historique
        Historique historique;
        QString details = QString("Produit ajouté : ID=%1, Nom=%2, Prix=%3, Quantité=%4, Date=%5, Catégorie=%6")
                                  .arg(IDP).arg(NomP).arg(prixP).arg(quentit).arg(dateP).arg(categorie);
        historique.ajouter("Ajout", details);

        return true;
}


bool Produit::supprimer(int IDP) {
    if (!checkDatabaseConnection()) return false;

    QSqlQuery query;
    query.prepare("DELETE FROM PRODUIT WHERE IDP = :IDP");
    query.bindValue(":IDP", QString::number(IDP));

    if (!query.exec()) {
        qDebug() << "Error deleting product:" << query.lastError().text();
        return false;
    }
    // Enregistrement dans l'historique
        Historique historique;
        QString details = QString("Produit supprimé : ID=%1").arg(IDP);
        historique.ajouter("Suppression", details);

        return true;
}

QSqlQueryModel* Produit::afficher(QString data, int mode) {

    QSqlQueryModel* model=new QSqlQueryModel();
          if(mode==0)
              model->setQuery("SELECT * FROM PRODUIT");
           if(mode==1)
           {
               model->setQuery("select * from PRODUIT where IDP like '" + data + "%'");
           }
           model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDP"));
           model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOMP"));
           model->setHeaderData(2, Qt::Horizontal, QObject::tr("CATEGORIE"));
           model->setHeaderData(3, Qt::Horizontal, QObject::tr("dateP"));
           model->setHeaderData(4, Qt::Horizontal, QObject::tr("PRIX"));
           model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITE"));
        return model;


}

bool Produit::modifier(int IDP) {
    if (!checkDatabaseConnection()) return false;

    QSqlQuery query;
    query.prepare("UPDATE PRODUIT SET NOMP = :Nomp,  PRIXP = :prixP , QUANTITE = :quentit , DATEP = :dateP , CATEGORIE = :categorie "
                  "WHERE IDP = :IDP");

    query.bindValue(":IDP", QString::number(IDP));
    query.bindValue(":NomP", NomP);
    query.bindValue(":prixP", prixP);
    query.bindValue(":quentit", quentit);  // La quantité reste une chaîne dans la base de données
    query.bindValue(":dateP", dateP);
    query.bindValue(":categorie", categorie);
    if (!query.exec()) {
        qDebug() << "Error updating product:" << query.lastError().text();
        return false;
    }
    return true;
}
bool Produit::rechercher(int IDP) {
    if (!checkDatabaseConnection()) return false;

    QSqlQuery query;
    query.prepare("SELECT * FROM PRODUIT WHERE IDP = :IDP");
    query.bindValue(":IDP", IDP);

    if (!query.exec()) {
        qDebug() << "Error searching for product:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        // Assuming the columns are in the order: IDP, NOMP, PRIXP, QUANTITE, DATEP, CATEGORIE
        this->IDP = query.value("IDP").toInt();
        this->NomP = query.value("NOMP").toString();
        this->prixP = query.value("PRIXP").toInt();
        this->quentit = query.value("QUANTITE").toString();
        this->dateP = query.value("DATEP").toString();
        this->categorie = query.value("CATEGORIE").toString();
        return true;
    } else {
        qDebug() << "Product with IDP" << IDP << "not found.";
        return false;
    }
}

QSqlQueryModel* Produit::trierParPrix(bool ascending)
{
    // Check if the database connection is open
    if (!checkDatabaseConnection()) return nullptr;

    // Create a model to store the sorted results
    QSqlQueryModel* model = new QSqlQueryModel();

    // Set up the sorting order
    QString order = ascending ? "ASC" : "DESC";

    // Construct the SQL query to sort by PRIX, with specific attributes
    QString queryStr = "SELECT IDP, NOMP, PRIXP, QUANTITE, CATEGORIE ,DATEP FROM PRODUIT ORDER BY PRIXP " + order;

    // Execute the query
    model->setQuery(queryStr);

    // Check for errors
    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors du tri par prix : " << model->lastError().text();
    }

    // Return the model containing the sorted results
    return model;
}






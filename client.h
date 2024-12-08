#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QTableWidget>

class client
{
public:
    // Constructeur par défaut
    client();

    QMap<QString, int> statistiquesAdresse(); // Méthode pour obtenir les statistiques


    // Constructeur paramétré
    client(int cin, int id, QString nom, int num, QDate date, QString adresse);

    // Méthode pour ajouter un client à la base de données
    bool ajouter();
    // Méthode pour afficher les clients
    QSqlQueryModel* afficher();
    // Méthode pour supprimer un client de la base de données
    bool supprimer(int id);
    void rechercherParId(QTableWidget *tableRecherche, const QString &id);
private:
    int cin;
    int id;
    QString nom;
    int num;
    QDate date;
    QString adresse;
};

#endif // CLIENT_H

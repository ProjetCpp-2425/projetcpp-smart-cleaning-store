#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QTableWidget>
#include <QSerialPort>

class service {
public:
    service(int id, const QString &nom, const QString &description, float prix,
            const QDate &date_debut, const QDate &date_fin, const QString &statut,
            const QString &employe, const QString &categorie);

    bool ajouter();
    bool idExists();
    bool update();
    void rechercherParId(QTableWidget *table, const QString &id_recherche);
    bool supprimer(int id);
    void sendEmailWithAttachment(const QString &fileName, const QString &recipientEmail);
    void sendSMSNotification(const QString &phoneNumber);


private:
    void sendToLCD(const QString &name, const QString &price); // New method

    int id;
    QString nom;
    QString description;
    float prix;
    QDate date_debut;
    QDate date_fin;
    QString statut;
    QString employe;
    QString categorie;
};

#endif // SERVICE_H

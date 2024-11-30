#include "service.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QHeaderView>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>

service::service(int id, const QString &nom, const QString &description, float prix,
                 const QDate &date_debut, const QDate &date_fin, const QString &statut,
                 const QString &employe, const QString &categorie)
    : id(id), nom(nom), description(description), prix(prix),
      date_debut(date_debut), date_fin(date_fin), statut(statut),
      employe(employe), categorie(categorie) {}

bool service::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO SERVICE (ID_SERVICE, NOM, DESCRIPTION, PRIX, CATEGORIE, DATED, DATEF, STATUT, EMPLOYE) "
                  "VALUES (:ID_SERVICE, :NOM, :DESCRIPTION, :PRIX, :CATEGORIE, "
                  "TO_DATE(:DATED, 'DD-MM-YYYY'), TO_DATE(:DATEF, 'DD-MM-YYYY'), :STATUT, :EMPLOYE)");
    query.bindValue(":ID_SERVICE", id);
    query.bindValue(":NOM", nom);
    query.bindValue(":DESCRIPTION", description);
    query.bindValue(":PRIX", prix);
    query.bindValue(":CATEGORIE", categorie);
    query.bindValue(":DATED", date_debut.toString("dd-MM-yyyy"));
    query.bindValue(":DATEF", date_fin.toString("dd-MM-yyyy"));
    query.bindValue(":STATUT", statut);
    query.bindValue(":EMPLOYE", employe);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du service: " << query.lastError().text();
        return false;
    }
    return true;
}

bool service::idExists() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SERVICE WHERE ID_SERVICE = :id");
    query.bindValue(":id", this->id);
    if (query.exec()) {
        query.next();
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool service::update() {
    QSqlQuery query;
    query.prepare("UPDATE SERVICE SET NOM = :nom, DESCRIPTION = :description, PRIX = :prix, "
                  "CATEGORIE = :categorie, DATED = :dated, DATEF = :datef, STATUT = :statut, EMPLOYE = :employe "
                  "WHERE ID_SERVICE = :id");
    query.bindValue(":id", this->id);
    query.bindValue(":nom", this->nom);
    query.bindValue(":description", this->description);
    query.bindValue(":prix", this->prix);
    query.bindValue(":categorie", this->categorie);
    query.bindValue(":dated", this->date_debut);
    query.bindValue(":datef", this->date_fin);
    query.bindValue(":statut", this->statut);
    query.bindValue(":employe", this->employe);

    return query.exec();
}

void service::rechercherParId(QTableWidget *table, const QString &id_recherche) {
    QSqlQuery query;
    query.prepare("SELECT ID_SERVICE, NOM, DESCRIPTION, PRIX, CATEGORIE, DATED, DATEF, STATUT, EMPLOYE "
                  "FROM SERVICE WHERE ID_SERVICE = :ID_SERVICE");
    query.bindValue(":ID_SERVICE", id_recherche);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche du service: " << query.lastError().text();
        return;
    }

    table->setRowCount(0);
    while (query.next()) {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(query.value("ID_SERVICE").toString()));
        table->setItem(row, 1, new QTableWidgetItem(query.value("NOM").toString()));
        table->setItem(row, 2, new QTableWidgetItem(query.value("DESCRIPTION").toString()));
        table->setItem(row, 3, new QTableWidgetItem(query.value("PRIX").toString()));
        table->setItem(row, 4, new QTableWidgetItem(query.value("CATEGORIE").toString()));
        table->setItem(row, 5, new QTableWidgetItem(query.value("DATED").toDate().toString("yyyy-MM-dd")));
        table->setItem(row, 6, new QTableWidgetItem(query.value("DATEF").toDate().toString("yyyy-MM-dd")));
        table->setItem(row, 7, new QTableWidgetItem(query.value("STATUT").toString()));
        table->setItem(row, 8, new QTableWidgetItem(query.value("EMPLOYE").toString()));
    }

}


bool service::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM SERVICE WHERE ID_SERVICE = :ID_SERVICE");
    query.bindValue(":ID_SERVICE", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du service: " << query.lastError().text();
        return false;
    }
    return true;
}


void service::sendEmailWithAttachment(const QString &fileName, const QString &recipientEmail) {
    QFile file(fileName);
    if (!file.exists()) {
        QMessageBox::critical(nullptr, "File Error", "The file does not exist: " + fileName);
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "File Error", "Failed to open the file: " + fileName);
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    // Base64 encode the file content
    QString base64Encoded = fileData.toBase64();

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.sendinblue.com/v3/smtp/email"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("api-key", "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");

    QJsonObject emailData;
    emailData["sender"] = QJsonObject{{"email", "perrine@laou.fr"}};
    emailData["to"] = QJsonArray{{QJsonObject{{"email", recipientEmail}}}};
    emailData["subject"] = "Exported Services Data";
    emailData["htmlContent"] = "<h3>Please find the exported services data attached.</h3>";

    // Prepare attachment object
    QJsonObject attachment;
    attachment["content"] = base64Encoded;
    attachment["name"] = QFileInfo(fileName).fileName();

    emailData["attachment"] = QJsonArray{attachment};

    // Send request
    QNetworkReply *reply = manager->post(request, QJsonDocument(emailData).toJson());

    // Connect to handle reply
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error()) {
            QString errorMessage = "Failed to send email: " + reply->errorString();
            QMessageBox::critical(nullptr, "Email Error", errorMessage);

            // Log response for debugging
            QByteArray response = reply->readAll();
            qDebug() << "Response: " << response;
        } else {
            QByteArray response = reply->readAll();
            qDebug() << "Success response: " << response;
            QMessageBox::information(nullptr, "Success", "Email sent successfully.");
        }
        reply->deleteLater();
    });
}


void service::sendSMSNotification(const QString &phoneNumber) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.sendinblue.com/v3/transactionalSMS/sms"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("api-key", "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");

    QJsonObject smsData;
    smsData["sender"] = "Innovaclean";
    smsData["recipient"] = phoneNumber;
    smsData["content"] = "The PDF export has been completed and sent to your email successfully.";

    QNetworkReply *reply = manager->post(request, QJsonDocument(smsData).toJson());
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error()) {
            QMessageBox::critical(nullptr, "SMS Error", "Failed to send SMS: " + reply->errorString());
        } else {
            QMessageBox::information(nullptr, "Success", "SMS sent successfully.");
        }
        reply->deleteLater();
    });
}

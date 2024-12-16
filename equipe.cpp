#include "equipe.h"
#include <QFileDialog>
#include <QBuffer>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>

equipe::equipe()
{
    id=0;
    nom="";
    type="";
    etat="";
    QDate date(2024, 10, 23);
}
equipe::equipe(int id,bool dispo ,QString nom,QString type,QString etat,QDate date_achat,QByteArray image_data,QString image_name)
{
    this->id=id;
    this->nom=nom;
    this->type=type;
    this->etat=etat;
    this->dispo=dispo;
    this->date_achat=date_achat;
    this->image_data=image_data;
    this->image_name=image_name;

}
QSqlQueryModel* equipe::afficher()
{
    QByteArray imagedatafromdb;
    QPixmap image;
    QString imagename;
    QSqlQueryModel* model = new QSqlQueryModel();

    // Query all data from the "equippement" table
    model->setQuery("SELECT * FROM equippement");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("etat"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("disponibilite"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("date_dachat"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("image_data"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("image_name"));
    return model;
}

bool equipe::checkIfIdExists(int id)
{
    QSqlQuery query;
        query.prepare("SELECT id FROM equippement WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec() && query.next()) {

            return true;
        } else {
            //QMessageBox::warning(nullptr, "Database Error", "Error checking ID existence: " + query.lastError().text());
            return false;
        }
}
bool equipe::ajouter()
{
       if (checkIfIdExists(id)) {
           //QMessageBox::warning(nullptr, "Error", "Event with this ID already exists.");
           return false;
       }
    QSqlQuery query;
    query.prepare("insert into equippement (id,type,nom,etat,disponibilite,date_dachat,image_data,image_name)" "values (:id,:type,:nom,:etat,:disponibilite,:date_dachat,:image_data,:image_name)");
    query.bindValue(":id",id);
    query.bindValue(":type",type);
    query.bindValue(":nom",nom);
    query.bindValue(":etat",etat);
    query.bindValue(":disponibilite",dispo);
    query.bindValue(":date_dachat",date_achat);
    query.bindValue(":image_data",image_data);
    query.bindValue(":image_name",image_name);

    return query.exec();
}
equipe equipe::getEquipeById(int id)
{
    QSqlQuery query;
    equipe equip;

    query.prepare("SELECT * FROM equippement WHERE id= :id");
    query.bindValue(":id", id);


    if(query.exec() && query.next())
    {
        equip.set_id(query.value(0).toInt());
        equip.set_type(query.value(1).toString());
        equip.set_nom(query.value(2).toString());
        equip.set_etat(query.value(3).toString());
        equip.set_dispo(query.value(4).toString().toLower() == "true" || "false");
        equip.set_date(query.value(5).toDate());
    }

    return equip;
}
bool equipe::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("delete from equippement where id=:id");
    query.bindValue(":id",id);
    return query.exec();
}
bool equipe::modifier(int id,bool dispo,QString type,QString nom,QString etat,QDate date_achat)
{
    QSqlQuery query;


    query.prepare("update equippement set etat=:etat,type=:type,nom=:nom,disponibilite=:disponibilite,date_dachat=:date_dachat where id=:id");
    query.bindValue(":id",id);
    query.bindValue(":nom",nom);
    query.bindValue(":type",type);
    query.bindValue(":disponibilite",dispo);
    query.bindValue(":date_dachat",date_achat);
    query.bindValue(":etat",etat);



    return query.exec();
}
bool equipe::supprimer_all()
{
    QSqlQuery query;
    query.prepare("TRUNCATE TABLE equippement");
    return query.exec();
}
QSqlQueryModel *equipe::search(const QString &criteria)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;


    QString checkQuery = "SELECT COUNT(*) FROM equippement WHERE nom LIKE :criteria";
    query.prepare(checkQuery);
    query.bindValue(":criteria", criteria + "%");

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            return nullptr;
        }
    } else {
        return nullptr;
    }
    query.prepare("SELECT * FROM equippement WHERE nom LIKE :criteria");
    query.bindValue(":criteria", criteria + "%");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("type"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("etat"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("disponibilite"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("date_dachat"));


        return model;
    } else {
        return nullptr;
    }
}
QSqlQueryModel *equipe::sort(const QString &criteria)
{
    QSqlQueryModel *model = new QSqlQueryModel();
       QSqlQuery query;

       QString select = "SELECT * FROM equippement ORDER BY " + criteria + " ASC";
       query.prepare(select);

       if (query.exec()) {
           model->setQuery(query);
           model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
           model->setHeaderData(1, Qt::Horizontal, QObject::tr("type"));
           model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
           model->setHeaderData(3, Qt::Horizontal, QObject::tr("etat"));
           model->setHeaderData(4, Qt::Horizontal, QObject::tr("disponibilite"));
           model->setHeaderData(5, Qt::Horizontal, QObject::tr("date_dachat"));

           return model;
       } else {
           return nullptr;
       }
}
bool equipe::exportToPdf()
{
    QSqlQuery query("SELECT * FROM equippement");

    QTextDocument doc;
    QTextCursor cursor(&doc);

    QString htmlContent = "<h1>Equipe Report</h1>";
    htmlContent += "<table style='border-collapse: collapse; width: 100%;'>";
    htmlContent += "<thead><tr>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>id</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>nom</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>type</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>etat</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>date d'achat</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>disponibilite</th>"
                   "</tr></thead>";
    htmlContent += "<tbody>";


    while (query.next()) {
        htmlContent += "<tr>";
        for (int i = 0; i < query.record().count(); ++i) {
            htmlContent += "<td style='border: 1px solid #000; padding: 8px;'>" + query.value(i).toString() + "</td>";
        }
        htmlContent += "</tr>";
    }
htmlContent += "</tbody></table>";
    doc.setHtml(htmlContent);



    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);


    QString filePath = documentsPath + "/equipe.pdf";


    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);


    doc.print(&printer);

    if (doc.isEmpty()) {
        qDebug() << "Error printing to PDF:" ;
        return false;
    }

    qDebug() << "PDF exported successfully to:" << filePath;
    return true;
}
void equipe::highlightEquipeDates(QCalendarWidget *calendarWidget)
{
    QSqlQuery query;
    QString select = "SELECT date_dachat FROM equippement"; // Assuming "dure" column represents the duration of the event in minutes
    query.prepare(select);
    if (query.exec()) {
        while (query.next()) {
            QDate startDate = query.value(0).toDate();
            int durationMinutes = query.value(1).toInt(); // Accessing the second column for dure (duration in minutes)

            // Define text format for event start date
            QTextCharFormat startFormat;
            QColor startColor(67, 127, 247);
            startFormat.setBackground(startColor);

            // Calculate the end date based on the duration in minutes
            QDateTime endDateTime = QDateTime(startDate, QTime(0, 0)).addSecs(durationMinutes * 60);
            QDate endDate = endDateTime.date();

            // Define text format for event end date
            QTextCharFormat endFormat;
            QColor endColor(67, 127, 247); // Example RGB color (orange)
            endFormat.setBackground(endColor);

            // Apply the format to the start date
            calendarWidget->setDateTextFormat(startDate, startFormat);

            // Apply the format to the end date
            calendarWidget->setDateTextFormat(endDate, endFormat);
        }
    } else {
        qDebug() << "Error fetching event dates:" << query.lastError().text();
    }

    // Disable date editing
    calendarWidget->setDateEditEnabled(false);
}
QSqlQueryModel * equipe::afficher_calendar(const QDate &selectedDate)
{
    QSqlQueryModel * model= new QSqlQueryModel();

    QSqlQuery query;
    QString select = "select id,type,nom,etat,disponibilite from equippement where date_dachat like :date";
    query.prepare(select);
    query.bindValue(":date", selectedDate);

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("nom"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("etat"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("disponibilite"));
        return model;
    }else
        return nullptr;
}

QByteArray equipe::processImageWithDialog(QString& selectedImagePath)
{
        // Ouvre une boîte de dialogue pour sélectionner une image
        QString imagePath = QFileDialog::getOpenFileName(
            nullptr,
            QObject::tr("Select Image"),
            QCoreApplication::applicationDirPath(),
            QObject::tr("Image Files (*.jpg *.jpeg *.png)")
        );

        // Vérifiez si un fichier a été sélectionné
        if (imagePath.isEmpty()) {
            qWarning() << "No image selected.";
            return QByteArray();
        }

        // Mettre à jour le chemin sélectionné
        selectedImagePath = imagePath;

        // Charger l'image sélectionnée
        QPixmap image(imagePath);
        if (image.isNull()) {
            qWarning() << "Invalid image or unsupported format.";
            return QByteArray();
        }

        // Sauvegarder les données de l'image en Base64
        QBuffer imageBufferData;
        if (imageBufferData.open(QIODevice::ReadWrite)) {
            image.save(&imageBufferData, "JPG");
        } else {
            qWarning() << "Failed to open buffer for image processing.";
            return QByteArray();
        }

        QByteArray finalDataToSave = imageBufferData.buffer().toBase64();
        return finalDataToSave;
    }

#include "equipe.h"

equipe::equipe()
{
    id=0;
    nom="";
    type="";
    etat="";
    QDate date(2024, 10, 23);
}
equipe::equipe(int id,bool dispo ,QString nom,QString type,QString etat,QDate date_achat)
{
        this->id=id;
        this->nom=nom;
        this->type=type;
        this->etat=etat;
        this->dispo=dispo;
        this->date_achat=date_achat;
}
QSqlQueryModel * equipe::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("select * from equippement");


    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("etat"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("disponibilite"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("date_dachat"));

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
    QString res=QString::number(id);
    query.prepare("insert into equippement (id,type,nom,etat,disponibilite,date_dachat)" "values (:id,:type,:nom,:etat,:disponibilite,:date_dachat)");
    query.bindValue(":id",res);
    query.bindValue(":type",type);
    query.bindValue(":nom",nom);
    query.bindValue(":etat",etat);
    query.bindValue(":disponibilite",dispo);
    query.bindValue(":date_dachat",date_achat);

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
    QString res=QString::number(id);
    query.prepare("delete from equippement where id=:id");
    query.bindValue(":id",res);
    return query.exec();
}
bool equipe::modifier(int id,bool dispo,QString type,QString nom,QString etat,QDate date_achat)
{
    QSqlQuery query;
    QString res=QString::number(id);

    query.prepare("update equippement set etat=:etat,type=:type,nom=:nom,disponibilite=:disponibilite,date_dachat=:date_dachat where id=:id");
    query.bindValue(":id",res);
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

    // Fetch and format data into table rows
    while (query.next()) {
        htmlContent += "<tr>";
        for (int i = 0; i < query.record().count(); ++i) {
            htmlContent += "<td style='border: 1px solid #000; padding: 8px;'>" + query.value(i).toString() + "</td>";
        }
        htmlContent += "</tr>";
    }
htmlContent += "</tbody></table>";
    doc.setHtml(htmlContent);


    // Get path to user's Documents directory
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    // Set the output file path to the documents directory
    QString filePath = documentsPath + "/equipe.pdf";

    // Create a QPrinter
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Print the document to the printer
    doc.print(&printer);

    if (doc.isEmpty()) {
        qDebug() << "Error printing to PDF:" ;
        return false;
    }

    qDebug() << "PDF exported successfully to:" << filePath;
    return true;
}

#include "employe.h"
#include "qdatetime.h"
#include "qtextcursor.h"
#include "qtextdocument.h"
#include <QSqlQuery>
#include <QVariant>
#include<QSqlError>
#include <QDate>
#include <QDebug>
#include <QPrinter>

employe::employe(int IDE,QString CINE,QString NUME,QString NOME,QString PRENOME,QString MAILE,QString POSTEE,QString DATE_EMBE,QString STATUTE,QString GENREE,int AGEE)
{
    this->IDE=IDE;
    this->CINE=CINE;
    this->NUME=NUME;
    this->NOME=NOME;
    this->PRENOME=PRENOME;
    this->MAILE=MAILE;
    this->POSTEE=POSTEE;
    this->DATE_EMBE=DATE_EMBE;
    this->STATUTE=STATUTE;
    this->GENREE=GENREE;
    this->AGEE=AGEE;
}

//new
/*int employe::calculerScore(int IDE)
{
    // Récupérer la date d'embauche de l'employé depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT DATEDEB FROM employe WHERE IDE = :IDE");
    query.bindValue(":IDE", IDE);
    query.exec();

    if (query.next()) {
        QString DATE_EMBE = query.value(0).toString();  // Récupérer la date d'embauche sous forme de string

        // Convertir la chaîne de caractères en QDate, en spécifiant le format de la date
        QDate DATE_EMBE = QDate::fromString(DATE_EMBE, "yyyy-MM-dd");  // Ajustez le format si nécessaire

        if (DATE_EMBE.isValid()) {
            // Calculer l'ancienneté en années
            QDate currentDate = QDate::currentDate();  // Date actuelle
            int anciennete = dateEmbauche.yearsTo(currentDate);  // Calcul de l'ancienneté en années

            // Le score est basé sur l'ancienneté (par exemple, 1 point par année d'ancienneté)
            return anciennete;
        } else {
            qDebug() << "La date d'embauche n'est pas valide";
            return 0;  // En cas d'erreur, retourner 0
        }
    } else {
        qDebug() << "Aucun employé trouvé avec cet ID";
        return 0;  // En cas d'erreur, retourner 0
    }
}

*/

bool employe ::ajouter(){
    QSqlQuery query;
    query.prepare("INSERT INTO employe (IDE,CINE,NUME,NOME ,PRENOME,MAILE,POSTEE,DATE_EMBE,STATUTE,GENREE,AGEE)"
                  "VALUES( :IDE, :CINE, :NUME, :NOME, :PRENOME, :MAILE, :POSTEE, :DATE_EMBE, :STATUTE, :GENREE, :AGEE)");
   query.bindValue(":IDE",IDE);
   query.bindValue(":CINE",CINE);
   query.bindValue(":NUME",NUME);
   query.bindValue(":NOME",NOME);
   query.bindValue(":PRENOME",PRENOME);
   query.bindValue(":MAILE",MAILE);
   query.bindValue(":POSTEE",POSTEE);
   query.bindValue(":DATE_EMBE",DATE_EMBE);
   query.bindValue(":STATUTE",STATUTE);
   query.bindValue(":GENREE",GENREE);
   query.bindValue(":AGEE",AGEE);

   return query.exec();
}

QSqlQueryModel * employe ::afficher(){
        QSqlQueryModel * model =new QSqlQueryModel();
        model->setQuery("select * from employe");
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("IDE"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("CINE"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("NUME"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("NOME"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("PRENOME"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("MAILE"));
        model->setHeaderData(6,Qt::Horizontal,QObject::tr("POSTEE"));
        model->setHeaderData(7,Qt::Horizontal,QObject::tr("DATE_EMBE"));
        model->setHeaderData(8,Qt::Horizontal,QObject::tr("STATUTE"));
        model->setHeaderData(9,Qt::Horizontal,QObject::tr("GENREE"));
        model->setHeaderData(10,Qt::Horizontal,QObject::tr("AGEE"));

        return model;
    }

bool employe::checkIfIdExists(int IDE)
{
    QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM employe WHERE IDE = :IDE");
        query.bindValue(":IDE", IDE);


        if (query.exec()) {
                if (query.next() && query.value(0).toInt() > 0) {
                    return true; // L'ID existe
                }
            }

            // Si la requête échoue ou si l'ID n'existe pas
            return false;
}

employe employe ::getEmployeById(int IDE)
{
    QSqlQuery query;
    employe employe;

    query.prepare("SELECT * FROM employe WHERE IDE= :IDE");
    query.bindValue(":IDE", IDE);


    if(query.exec() && query.next())
    {
        employe.setID(query.value(0).toInt());
        employe.setCIN(query.value(1).toString());
        employe.setNUM(query.value(2).toString());
        employe.setNom(query.value(3).toString());
        employe.setPrenom(query.value(4).toString());
        employe.setMail(query.value(5).toString());
        employe.setPoste(query.value(2).toString());
        employe.setdate_emb(query.value(3).toString());
        employe.setstatut(query.value(4).toString());
        employe.setGENRE(query.value(5).toString());
        employe.setAGE(query.value(0).toInt());

    }

    return employe;
}

bool employe::supprimer(int IDE)
{
    QSqlQuery query;
    QString res=QString::number(IDE);
    query.prepare("delete from employe where IDE=:IDE");
    query.bindValue(":IDE",res);
    return query.exec();
}

bool employe::loadEmployeById(int IDE) {
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE IDE = :IDE");
    query.bindValue(":IDE", IDE);

    if (query.exec() && query.next()) {
        // Charger les valeurs des colonnes
        this->IDE = query.value("IDE").toInt();
        CINE = query.value("CINE").toString();
        NUME = query.value("NUME").toString();
        NOME = query.value("NOME").toString();
        PRENOME = query.value("PRENOME").toString();
        MAILE = query.value("MAILE").toString();
        POSTEE = query.value("POSTEE").toString();
        DATE_EMBE = query.value("DATE_EMBE").toString();
        STATUTE = query.value("STATUTE").toString();
        GENREE = query.value("GENREE").toString();
        AGEE = query.value("AGEE").toInt();
        return true;
    } else {
        // Correction : Ajoutez une trace de l'erreur pour faciliter le débogage si `query.exec()` échoue.
        qDebug() << "Erreur lors du chargement de l'employé :" << query.lastError().text();
        return false; // Retourne `false` si l'employé n'est pas trouvé ou si la requête échoue.
    }
}

bool employe::modifier(int IDE) {
    QSqlQuery query;
    query.prepare("UPDATE employe SET CINE = :CINE, NUME = :NUME, NOME = :NOME, PRENOME = :PRENOME, "
                  "MAILE = :MAILE, POSTEE = :POSTEE, DATE_EMBE = :DATE_EMBE, STATUTE = :STATUTE, "
                  "GENREE = :GENREE, AGEE = :AGEE WHERE IDE = :IDE");

    query.bindValue(":CINE", CINE);
    query.bindValue(":NUME", NUME);
    query.bindValue(":NOME", NOME);
    query.bindValue(":PRENOME", PRENOME);
    query.bindValue(":MAILE", MAILE);
    query.bindValue(":POSTEE", POSTEE);
    query.bindValue(":DATE_EMBE", DATE_EMBE);
    query.bindValue(":STATUTE", STATUTE);
    query.bindValue(":GENREE", GENREE);
    query.bindValue(":AGEE", AGEE);
    query.bindValue(":IDE", IDE);

    if (!query.exec()) {
           qDebug() << "Erreur lors de la modification de l'employé :" << query.lastError().text();
           return false;
       }
       return true;
}

QSqlQueryModel *employe::chercher(const QString &critere)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;


    QString checkQuery = "SELECT COUNT(*) FROM employe WHERE NOME LIKE :critere";
    query.prepare(checkQuery);
    query.bindValue(":critere", critere + "%");

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            return nullptr;
        }
    }
    query.prepare("SELECT * FROM employe WHERE NOME LIKE :critere");
    query.bindValue(":critere", critere + "%");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("IDE"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("CINE"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("NUME"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("NOME"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("PRENOME"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("MAILE"));
        model->setHeaderData(6,Qt::Horizontal,QObject::tr("POSTEE"));
        model->setHeaderData(7,Qt::Horizontal,QObject::tr("DATE_EMBE"));
        model->setHeaderData(8,Qt::Horizontal,QObject::tr("STATUTE"));
        model->setHeaderData(9,Qt::Horizontal,QObject::tr("GENREE"));
        model->setHeaderData(10,Qt::Horizontal,QObject::tr("AGEE"));
        return model;
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête de recherche :" << query.lastError().text();
        return nullptr;
    }
}

QSqlQueryModel* employe::trier(const QString& critere)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr;

    // Définir la requête SQL en fonction du critère de tri
    if (critere == "Date d'embauche") {
        queryStr = "SELECT * FROM EMPLOYE ORDER BY DATE_EMBE";
    } else if (critere == "poste") {
        queryStr = "SELECT * FROM EMPLOYE ORDER BY POSTEE";
    } else if (critere == "statut") {
        queryStr = "SELECT * FROM EMPLOYE ORDER BY STATUTE";
    }

    // Exécuter la requête
    QSqlQuery query;
    if (query.exec(queryStr)) {
        model->setQuery(query);
    } else {
        qDebug() << "Erreur de tri : " << query.lastError();
    }

    return model;
}

QSqlQueryModel* employe::getEquipeStatistics() {
    QSqlQueryModel *model = new QSqlQueryModel();

    // Example SQL query to get statistics
    QSqlQuery query;
    query.prepare("SELECT * FROM employe");  // Adjust your query

    if (query.exec()) {
        model->setQuery(query);
    } else {
        // Handle query execution error
    }

    return model;
}

bool employe::exportToPdf()
{
    QSqlQuery query("SELECT * FROM employe");

    QTextDocument doc;
    QTextCursor cursor(&doc);

    QString htmlContent = "<h1>Equipe Report</h1>";
    htmlContent += "<table style='border-collapse: collapse; width: 100%;'>";
    htmlContent += "<thead><tr>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>id</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>CINE</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>NUME</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>NOME</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>PRENOME</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>MAILE</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>POSTEE</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>DATE_EMBE</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>GENREE</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>STATUTE</th>"
                   "<th style='border: 1px solid #000; padding: 8px; text-align: left;font-weight: bold;'>AGEE</th>"


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


    QString filePath = documentsPath + "/employe.pdf";


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

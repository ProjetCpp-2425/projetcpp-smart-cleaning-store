#include "mainwindow.h"
#include <QCompleter>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QString>
#include "qprocess.h"
#include <QPainter>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "dialogmodifier.h"
#include <QtCharts/QPieSeries>
#include <QChart>
#include <QChartView>
#include <QGraphicsScene>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTextStream>
#include <QMessageBox>
#include <QAuthenticator>
#include <QEventLoop>
#include <QRegularExpression>
#include "employe.h"
#include "conge.h"
#include "dialogscore.h"
#include <QtSerialPort>
#include "smtp.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("employe");
    model->select();
    ui->tableView->setModel(Etmp.afficher());
    setupEmployeeStatsChart();

    connect(ui->pushButton_Modifier, &QPushButton::clicked, this, &MainWindow::on_pushButton_Modifier_clicked);
    //connect(ui->refresh, &QPushButton::clicked, this, &::MainWindow::on_refresh_Clicked);
    connect(ui->pushButton_approuver, &QPushButton::clicked, this, &MainWindow::on_pushButton_approuver_clicked);
    connect(ui->pushButton_refuser, &QPushButton::clicked, this, &MainWindow::on_pushButton_refuser_clicked);
    connect(ui->pushButton_goToScorePage, &QPushButton::clicked, this, &MainWindow::on_pushButton_goToScorePage_clicked);

    /*conge conge;
    conge.afficherDansTableView(ui->tableView_2);*/
    model->setTable("CONGE");
    model->select();
    ui->tableView_2->setModel(c.afficher());
    int ret=a.connect_arduino();
    switch(ret)
    {
    case(0):qDebug()<<"arduino is available and connected to : "<<a.getarduino_port_name();
        break;
    case(1):qDebug()<<"arduino is available and not connected to : "<<a.getarduino_port_name();
        break;
    case(-1):qDebug()<<"arduino is not available";
        break;
    }
    QObject::connect(a.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
    // Read data from Arduino and verify it
        QObject::connect(a.getserial(), &QSerialPort::readyRead, [&]() {
            static QByteArray buffer; // Buffer to accumulate data

            data = a.read_from_arduino();
            buffer.append(data); // Append new data to the buffer
            if (buffer.contains("\r\n"))
            {
                QString completeMessage = QString::fromUtf8(buffer); // Convert to QString
                completeMessage = completeMessage.trimmed(); // Remove trailing newline characters
                completeMessage = completeMessage.remove(" ");

                qDebug() << "Complete data:" << completeMessage;

                QString uid = QString::fromUtf8(data).trimmed();
                //qDebug() << "Card UID: " << data;

                // Check if the UID is in the database
                bool idValid = a.checkUIDInDatabase(completeMessage);
                if (idValid) {
                    qDebug() << "Card UID is valid!";
                    ui->led->setText("valid");
                } else {
                    qDebug() << "Card UID is invalid!";
                    ui->led->setText("invalid");
                }
                buffer.clear(); // Clear the buffer after processing

            }

        });
 }


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_goToScorePage_clicked()
{
    // Create and show the DialogScore
    Dialogscore *dialog = new Dialogscore(this);  // Create the dialog (with MainWindow as parent)
    dialog->exec();  // Show the dialog as a modal popup
}

QSqlQueryModel* MainWindow::getEmployeeStats() {
    QSqlQueryModel *model = new QSqlQueryModel;

    // Update the query to use your actual table and column names
    model->setQuery("SELECT POSTEE, COUNT(*) as num_employees FROM employe GROUP BY POSTEE;");

    // Debugging: check if the query has valid data
    if (model->rowCount() == 0) {
        qDebug() << "No data found for employee stats!";
    } else {
        qDebug() << "Data found!";
    }

    return model;
}

bool MainWindow::setupEmployeeStatsChart() {
   QSqlQueryModel* model = getEmployeeStats();

    if (model->rowCount() == 0) {
        qDebug() << "No employee statistics available!";
        return false;  // Retourne false si aucune donnée n'est disponible
    }

    // Crée une série de barres pour le graphique
    QBarSeries *barSeries = new QBarSeries();

    // Vecteurs pour stocker les QBarSet et les catégories des postes
    QVector<QBarSet*> barSets;
    QVector<QString> categories;

    // Parcourt les données pour créer les ensembles de barres
    for (int row = 0; row < model->rowCount(); ++row) {
        QString position = model->data(model->index(row, 0)).toString();  // Récupère la position
        int numEmployees = model->data(model->index(row, 1)).toInt();     // Récupère le nombre d'employés

        // Crée un QBarSet pour chaque poste
        QBarSet *set = new QBarSet(position);
        *set << numEmployees;
        barSets.append(set);
        categories.append(position);
    }

    // Ajoute les ensembles de barres à la série
    for (QBarSet *set : barSets) {
        barSeries->append(set);
    }

    // Crée le graphique et configure ses propriétés
    QChart *chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle("Number of Employees per Position");

    // Configure l'axe Y (nombre d'employés)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Number of Employees");
    axisY->setRange(0, 20);  // Ajustez la plage selon vos besoins

    // Configure l'axe X (noms des postes)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);

    // Ajoute les axes au graphique
    chart->addAxis(axisX, Qt::AlignBottom);  // Ajouter un axe X au graphique
    chart->addAxis(axisY, Qt::AlignLeft);    // Ajouter un axe Y au graphique
    barSeries->attachAxis(axisX);             // Attacher l'axe X à la série
    barSeries->attachAxis(axisY);             // Attacher l'axe Y à la série

    // Crée la vue du graphique et active l'anticrénelage
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Nettoie le contenu existant de l'onglet avant d'ajouter le nouveau graphique
    QLayout *oldLayout = ui->tab_4->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();  // Supprime les widgets
            delete item;  // Supprime les items
        }
        delete oldLayout;
    }

    // Ajoute le nouveau graphique au layout de l'onglet
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->tab_4->setLayout(layout);

    return true;  // Retourne true si le graphique est correctement mis à jour
}

void MainWindow::on_refresh_Clicked() {
    qDebug() << "Refreshing statistics...";
    // Appeler la fonction de mise à jour des statistiques
    bool success = setupEmployeeStatsChart();  // Modifiez `setupEmployeeStatsChart()` pour qu'elle retourne true/false

        if (success) {
            QMessageBox::information(this, "Rafraîchissement", "Les statistiques ont été mises à jour avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la mise à jour des statistiques. Veuillez réessayer.");
        }

    }

void MainWindow::on_pushButton_ajouter_clicked()
{
    // Récupération des valeurs des champs
    int IDE = ui->lineEdit_idE->text().toInt();
    QString CINE = ui->lineEdit_cinE->text();
    QString NUME = ui->lineEdit_numE->text();
    QString NOME = ui->lineEdit_nomE->text();
    QString PRENOME = ui->lineEdit_prenomE->text();
    QString MAILE = ui->lineEdit_mailE->text();
    QString POSTEE = ui->lineEdit_posteE->text();
    QString DATE_EMBE = ui->lineEdit_date_embE->text();
    QString STATUTE = ui->lineEdit_statutE->text();
    QString GENREE = ui->lineEdit_genreE->text();
    int AGEEE = ui->lineEdit_ageE->text().toInt();

    // Contrôles de saisie
    if (NOME.isEmpty() || PRENOME.isEmpty() || MAILE.isEmpty() || POSTEE.isEmpty() || STATUTE.isEmpty() || GENREE.isEmpty() || AGEEE <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), QObject::tr("Veuillez remplir tous les champs obligatoires et vérifier l'âge."), QMessageBox::Ok);
        return;
    }

    // Vérification que l'ID est un entier positif
    if (IDE <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), QObject::tr("L'ID doit être un entier positif."), QMessageBox::Ok);
        return;
    }

    // Vérification que le CIN et NUME sont des entiers à 8 chiffres
    QString CIN_str = ui->lineEdit_cinE->text();
    QString NUME_str = ui->lineEdit_numE->text();
    QRegularExpression re("[0-9]{8}");  // Expression régulière pour un entier de 8 chiffres
    if (!re.match(CIN_str).hasMatch()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), QObject::tr("Le CIN doit être un entier à 8 chiffres."), QMessageBox::Ok);
        return;
    }

    if (!re.match(NUME_str).hasMatch()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), QObject::tr("Le NUME doit être un entier à 8 chiffres."), QMessageBox::Ok);
        return;
    }

    // Vérification du statut (actif ou inactif)
    if (STATUTE != "actif" && STATUTE != "inactif") {
        QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), QObject::tr("Le statut doit être soit 'actif' soit 'inactif'."), QMessageBox::Ok);
        return;
    }

    // Vérification du genre (femme ou homme)
    if (GENREE != "femme" && GENREE != "homme") {
        QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), QObject::tr("Le genre doit être soit 'femme' soit 'homme'."), QMessageBox::Ok);
        return;
    }

    // Vérification que l'âge est supérieur à 18
    if (AGEEE < 18) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur d'âge"), QObject::tr("L'employé doit avoir plus de 18 ans."), QMessageBox::Ok);
        return;
    }

    // Validation de l'email avec une expression régulière
    QRegularExpression emailPattern("^[a-zA-Z0-9_+&*-]+(?:\\.[a-zA-Z0-9_+&*-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,7}$");
    QRegularExpressionMatch emailMatch = emailPattern.match(MAILE);
    if (!emailMatch.hasMatch()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), QObject::tr("L'adresse e-mail n'est pas valide."), QMessageBox::Ok);
        return;
    }

    // Vérification du format de la date (ex: YYYY-MM-DD)
    QRegularExpression datePattern("^\\d{4}-\\d{2}-\\d{2}$");
    QRegularExpressionMatch dateMatch = datePattern.match(DATE_EMBE);
    if (!dateMatch.hasMatch()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), QObject::tr("Le format de la date d'embauche n'est pas valide (attendu : YYYY-MM-DD)."), QMessageBox::Ok);
        return;
    }

    // Création de l'employé
    employe E(IDE, CINE, NUME, NOME, PRENOME, MAILE, POSTEE, DATE_EMBE, STATUTE, GENREE, AGEEE);

    // Tentative d'ajout de l'employé
    bool test = E.ajouter();
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Ajout réussi"), QObject::tr("L'employé a été ajouté avec succès."), QMessageBox::Ok);
        ui->tableView->setModel(E.afficher());  // Actualiser la vue
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("L'ajout de l'employé a échoué. Vérifiez les données ou la connexion à la base de données."), QMessageBox::Ok);
    }

}

void MainWindow::on_pushButton_supprimer_clicked()
{
    int IDE = ui->lineEdit_idEsupp->text().toInt(); // Assuming lineEdit_IdpSupp is where the ID is entered

    // Call the supprimer() method on a Produit object or Etmp (an instance of Produit)
    employe e;
    bool test = e.supprimer(IDE); // Attempt to delete the product with the specified ID

    // Check the result of the deletion and display appropriate message
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("suppression réussite"), QObject::tr("L'employé a été suuprimé avec succès."), QMessageBox::Ok);
        ui->tableView->setModel(e.afficher());  // Actualiser la vue
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("La suppression de l'employé a échouée."), QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_Modifier_clicked()
{
    // Retrieve the employee ID from the lineEdit (assuming the employee ID is entered in a QLineEdit)
    int IDE = ui->lineEdit_ID->text().toInt();

    if (IDE <= 0) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer un ID valide."));
        return;
    }

    // Create and open the modification dialog for the given employee ID
    Dialogmodifier *dialog = new Dialogmodifier(this, IDE);
    connect(dialog, &Dialogmodifier::employeModified, this, &MainWindow::onEmployeModified);
    dialog->exec();
}

void MainWindow::onEmployeModified()
{
    // Refresh or update the view after modification
    // You might want to refresh the employee list, table view, or perform any other updates
    ui->tableView->setModel(Etmp.afficher());
    QMessageBox::information(this, tr("Succès"), tr("L'employé a été modifié avec succès."));
}

void MainWindow::on_chercher_textChanged(const QString &arg1)
{
 QSqlQueryModel *searchModel = Etmp.chercher(arg1);
 if (searchModel!=nullptr) {

     ui->tableView->setModel(searchModel);
 } else{
     QMessageBox::critical(this,"Erreur", "Recherche échouée. Aucun résultat trouvé.");
 }
}

void MainWindow::on_tri_clicked()
{
    QString critere = ui->comboBoxTri->currentText();
    QSqlQueryModel *model = Etmp.trier(critere);

    if (model) {
        ui->tableView->setModel(model);
        QMessageBox::information(this, "Tri", "Les résultats ont été triés avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de trier les résultats.");
    }
}

void MainWindow::on_resetButton_clicked() {
    // Créer une nouvelle requête pour récupérer toutes les données sans tri
    QSqlQueryModel *model = new QSqlQueryModel();

    // Requête pour récupérer tous les employés sans tri
    QString queryStr = "SELECT * FROM EMPLOYE";  // Remplace par le nom de ta table si nécessaire
    QSqlQuery query;

    if (query.exec(queryStr)) {
        model->setQuery(std::move(query));  // Utilise un move pour éviter la copie
         // Met à jour le modèle avec les résultats
        ui->tableView->setModel(model);  // Applique le modèle à la table view
        QMessageBox::information(this, "Réinitialisation", "La vue a été réinitialisée.");
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de réinitialiser la vue.");
        qDebug() << "Erreur de réinitialisation : " << query.lastError();
    }
}



bool MainWindow::validerSaisie() {
    // Récupération et nettoyage des données depuis les champs de l'interface utilisateur
    QString strIDC = ui->IDC->text().trimmed();
    QString strIDE = ui->IDE->text().trimmed();
    QString DATEDEB = ui->DATEDEB->text().trimmed();
    QString TYPE = ui->TYPE->text().trimmed();
    QString PERIODE = ui->PERIODE->text().trimmed();

    // Vérification des champs vides
    if (strIDC.isEmpty() || strIDE.isEmpty() || DATEDEB.isEmpty() || TYPE.isEmpty() || PERIODE.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis.");
        return false;
    }

    // Vérification que l'ID congé est un entier valide
    bool okIDC;
    int IDC = strIDC.toInt(&okIDC);
    if (!okIDC || IDC <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID congé doit être un entier valide.");
        return false;
    }

    // Vérification que l'ID employé est un entier valide
    bool okIDE;
    int IDE = strIDE.toInt(&okIDE);
    if (!okIDE || IDE <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID employé doit être un entier valide.");
        return false;
    }

    // Vérification de l'existence de l'employé
    employe employe; // Création de l'objet employé
    if (!employe.checkIfIdExists(IDE)) {
        QMessageBox::critical(this, "Erreur", "Aucun employé trouvé avec cet ID.");
        return false;
    }

    // Vérification du format de la date
    QDate date = QDate::fromString(DATEDEB, "yyyy-MM-dd");
    if (!date.isValid()) {
        QMessageBox::warning(this, "Erreur", "La date doit être au format AAAA-MM-JJ.");
        return false;
    }

    // Vérification de la période (optionnelle, selon vos besoins)
    bool okPeriode;
    int periode = PERIODE.toInt(&okPeriode);
    if (!okPeriode || periode <= 0) {
        QMessageBox::warning(this, "Erreur", "La période doit être un entier valide et supérieur à 0.");
        return false;
    }

    return true; // Si toutes les validations sont passées
}

void MainWindow::on_pushButton_approuver_clicked()
{
    if (!validerSaisie()) return;

    int IDC= ui->IDC->text().toInt();
      int IDE = ui->IDE->text().toInt();
      QString DATEDEB = ui->DATEDEB->text().trimmed();
      QString TYPE = ui->TYPE->text().trimmed();
      int PERIODE = ui->PERIODE->text().toInt();
      employe e;
      conge conge;

      // Ajouter le congé avec statut "Approuvé"
      if (conge.ajouterConge(IDC,IDE, DATEDEB,TYPE,PERIODE, "Approuve")){
          qDebug()<<"aaaa";
          QString from = "akarray276@gmail.com";
          QString to = "roue.hamemi@esprit.tn"; // Change to the actual recipient's email
          QString subject = "Congé Approuvé";
          QString body = QString("Votre congé a été approuvé.\n\nDétails:\nID Congé: %1\nType: %2\nDate Début: %3\nPériode: %4 jours")
                             .arg(IDC)
                             .arg(TYPE)
                             .arg(DATEDEB)
                             .arg(PERIODE);

          Smtp *smtp = new Smtp(from, to, subject, body);
          connect(smtp, SIGNAL(status(QString)), this, SLOT(handleEmailStatus(QString)));
          // Mettre à jour le statut de l'employé à "Inactif"
          if (conge.mettreAJourStatut(IDE, "inactif")) {
              QMessageBox::information(this, "Succès", "Congé approuvé et statut employé mis à jour.");
               ui->tableView->setModel(e.afficher());
          } else {
              QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du statut de l'employé.");
          }

      }
      else {
          QMessageBox::critical(this, "Erreur", "Échec de l'approbation du congé.");
      }
    ui->tableView_2->setModel(c.afficher());
}
void MainWindow::handleEmailStatus(const QString &status)
{
    QMessageBox::information(this, "Email Status", status);
}
void MainWindow::on_pushButton_refuser_clicked()
{
    if (!validerSaisie()) return;

    int IDC= ui->IDC->text().toInt();
    int IDE = ui->IDE->text().toInt();
    QString DATEDEB = ui->DATEDEB->text().trimmed();
    QString TYPE = ui->TYPE->text().trimmed();
    int PERIODE = ui->PERIODE->text().toInt();


       conge congeref;
       // Ajouter le congé avec statut "Refusé"
       if (congeref.ajouterConge(IDC,IDE, DATEDEB,TYPE,PERIODE, "Refuse")) {
           QMessageBox::information(this, "Succès", "Le congé a été refusé.");
       } else {
           QMessageBox::critical(this, "Erreur", "Échec du refus du congé.");
       }
       ui->tableView_2->setModel(c.afficher());
   }

void MainWindow::on_exporterEnPDF_clicked()
{
    if (Etmp.exportToPdf()==true) {
               QMessageBox::information(this, "Done", "PDF exported successfully!");
           } else {
               QMessageBox::critical(this, "Error", "Failed to export PDF!");
           }
}


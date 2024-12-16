#include "employewindow.h"
#include "qprocess.h"
#include "dialogmodifier.h"
#include "servicewindow.h"
#include "clientwindow.h"
#include "produitwindow.h"
#include "equipement.h"
#include "./ui_employewindow.h"
#include "./ui_servicewindow.h"
#include "./ui_clientwindow.h"
#include "./ui_produitwindow.h"
#include "./ui_equipement.h"
#include "employe.h"
#include "conge.h"
#include "dashboarddialog.h"
#include "ui_dashboarddialog.h"
#include <QCompleter>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QString>
#include <QPainter>
#include <QMessageBox>
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
#include <QtSerialPort>
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

EmployeWindow::EmployeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmployeWindow)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("employe");
    model->select();
    ui->tableView->setModel(Etmp.afficher());
    setupEmployeeStatsChart();

    connect(ui->pushButton_Modifier, &QPushButton::clicked, this, &EmployeWindow::on_pushButton_Modifier_clicked);
    connect(ui->refresh, &QPushButton::clicked, this, &::EmployeWindow::on_refresh_Clicked);
    connect(ui->pushButton_approuver, &QPushButton::clicked, this, &EmployeWindow::on_pushButton_approuver_clicked);
    connect(ui->pushButton_refuser, &QPushButton::clicked, this, &EmployeWindow::on_pushButton_refuser_clicked);
    connect(ui->pushButton_goToScorePage, &QPushButton::clicked, this, &EmployeWindow::on_pushButton_goToScorePage_clicked);

    /*conge conge;
    conge.afficherDansTableView(ui->tableView_2);*/
   /* model->setTable("CONGE");
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

        });*/
 }


EmployeWindow::~EmployeWindow() {
    delete ui;
}

void EmployeWindow::on_pushButton_goToScorePage_clicked()
{
    // Create and show the DialogScore
    DashboardDialog *dialog = new DashboardDialog(this);  // Create the dialog (with MainWindow as parent)
    dialog->exec();  // Show the dialog as a modal popup
}

QSqlQueryModel* EmployeWindow::getEmployeeStats() {
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

bool EmployeWindow::setupEmployeeStatsChart() {
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
    QLayout *oldLayout = ui->tableView_3->layout();
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
    ui->tableView_3->setLayout(layout);

    return true;  // Retourne true si le graphique est correctement mis à jour
}

void EmployeWindow::on_refresh_Clicked() {
    qDebug() << "Refreshing statistics...";
    // Appeler la fonction de mise à jour des statistiques
    bool success = setupEmployeeStatsChart();  // Modifiez `setupEmployeeStatsChart()` pour qu'elle retourne true/false

        if (success) {
            QMessageBox::information(this, "Rafraîchissement", "Les statistiques ont été mises à jour avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la mise à jour des statistiques. Veuillez réessayer.");
        }

    }

void EmployeWindow::showErrorMessage(const QString &message) {
    QMessageBox::warning(nullptr, QObject::tr("Erreur de saisie"), message, QMessageBox::Ok);
}

void EmployeWindow::on_pushButton_ajouter_clicked() {
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
    int SALAIREE = ui->SALAIRE->text().toInt();

    // Contrôles de saisie
    if (NOME.isEmpty() || PRENOME.isEmpty() || MAILE.isEmpty() || POSTEE.isEmpty() || STATUTE.isEmpty() || GENREE.isEmpty() || AGEEE <= 0) {
        showErrorMessage(QObject::tr("Veuillez remplir tous les champs obligatoires et vérifier l'âge."));
        return;
    }

    // Vérification que l'ID est un entier positif
    if (IDE <= 0) {
        showErrorMessage(QObject::tr("L'ID doit être un entier positif."));
        return;
    }

    // Vérification que le CIN et NUME sont des entiers à 8 chiffres
    QRegularExpression re("[0-9]{8}");
    if (!re.match(CINE).hasMatch()) {
        showErrorMessage(QObject::tr("Le CIN doit être un entier à 8 chiffres."));
        return;
    }

    if (!re.match(NUME).hasMatch()) {
        showErrorMessage(QObject::tr("Le NUME doit être un entier à 8 chiffres."));
        return;
    }

    // Vérification du statut (actif ou inactif)
    if (STATUTE != "actif" && STATUTE != "inactif") {
        showErrorMessage(QObject::tr("Le statut doit être soit 'actif' soit 'inactif'."));
        return;
    }

    // Vérification du genre (femme ou homme)
    if (GENREE != "femme" && GENREE != "homme") {
        showErrorMessage(QObject::tr("Le genre doit être soit 'femme' soit 'homme'."));
        return;
    }

    // Vérification que l'âge est supérieur à 18
    if (AGEEE < 18) {
        showErrorMessage(QObject::tr("L'employé doit avoir plus de 18 ans."));
        return;
    }

    // Validation de l'email
    QRegularExpression emailPattern("^[a-zA-Z0-9_+&*-]+(?:\\.[a-zA-Z0-9_+&*-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,7}$");
    if (!emailPattern.match(MAILE).hasMatch()) {
        showErrorMessage(QObject::tr("L'adresse e-mail n'est pas valide."));
        return;
    }

    // Vérification du format de la date
    QRegularExpression datePattern("^\\d{4}-\\d{2}-\\d{2}$");
    if (!datePattern.match(DATE_EMBE).hasMatch()) {
        showErrorMessage(QObject::tr("Le format de la date d'embauche n'est pas valide (attendu : YYYY-MM-DD)."));
        return;
    }

    // Création et ajout de l'employé
    employe E(IDE, CINE, NUME, NOME, PRENOME, MAILE, POSTEE, DATE_EMBE, STATUTE, GENREE, AGEEE,60,SALAIREE);
    if (E.ajouter()) {
        QMessageBox::information(nullptr, QObject::tr("Ajout réussi"), QObject::tr("L'employé a été ajouté avec succès."), QMessageBox::Ok);
        ui->tableView->setModel(E.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("L'ajout de l'employé a échoué. Vérifiez les données ou la connexion à la base de données."), QMessageBox::Ok);
    }
}

void EmployeWindow::on_pushButton_supprimer_clicked()
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

void EmployeWindow::on_pushButton_Modifier_clicked()
{
    // Retrieve the employee ID from the lineEdit (assuming the employee ID is entered in a QLineEdit)
    int IDE = ui->lineEdit_ID->text().toInt();

    if (IDE <= 0) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer un ID valide."));
        return;
    }

    // Create and open the modification dialog for the given employee ID
    Dialogmodifier *dialog = new Dialogmodifier(this, IDE);
    connect(dialog, &Dialogmodifier::employeModified, this, &EmployeWindow::onEmployeModified);
    dialog->exec();
}

void EmployeWindow::onEmployeModified()
{
    // Refresh or update the view after modification
    // You might want to refresh the employee list, table view, or perform any other updates
    ui->tableView->setModel(Etmp.afficher());
    QMessageBox::information(this, tr("Succès"), tr("L'employé a été modifié avec succès."));
}

void EmployeWindow::on_chercher_textChanged(const QString &arg1)
{
 QSqlQueryModel *searchModel = Etmp.chercher(arg1);
 if (searchModel!=nullptr) {

     ui->tableView->setModel(searchModel);
 } else{
     QMessageBox::critical(this,"Erreur", "Recherche échouée. Aucun résultat trouvé.");
 }
}

void EmployeWindow::on_tri_clicked()
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

void EmployeWindow::on_resetButton_clicked() {
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



bool EmployeWindow::validerSaisie() {
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


void EmployeWindow::on_exporterEnPDF_clicked()
{
    if (Etmp.exportToPdf()==true) {
               QMessageBox::information(this, "Done", "PDF exported successfully!");
           } else {
               QMessageBox::critical(this, "Error", "Failed to export PDF!");
           }
}


void EmployeWindow::on_pushButton_approuver_clicked()
{
    // Valider les saisies
    if (!validerSaisie()) return;

    int IDC = ui->IDC->text().toInt();
    int IDE = ui->IDE->text().toInt();
    QString DATEDEB = ui->DATEDEB->text().trimmed();  // Utilisation de QDate
    QString TYPE = ui->TYPE->text().trimmed();
    int PERIODE = ui->PERIODE->text().toInt();

    conge c;
    employe e;
    QSqlQuery query;
     ui->tableView->setModel(e.afficher());
     // Requête pour récupérer le statut de l'employé
     query.prepare("SELECT STATUTE FROM EMPLOYE WHERE IDE = :IDE");
     query.bindValue(":IDE", IDE);

     if (!query.exec()) {
         // Si la requête échoue, afficher une erreur
         qDebug() << "Erreur lors de l'exécution de la requête pour récupérer le statut : " << query.lastError().text();
         QMessageBox::critical(this, "Erreur", "Impossible de vérifier le statut de l'employé.");
         return;
     }

     if (query.next()) {
         // Lire le statut depuis le résultat de la requête
         QString STATUTE = query.value(0).toString().trimmed();
         qDebug() << "Statut actuel de l'employé (IDE:" << IDE << "):" << STATUTE;

         if (STATUTE.toLower() != "actif") { // Vérifier si le statut est "actif"
             QMessageBox::warning(this, "Erreur", "L'employé est inactif. Impossible d'approuver le congé.");
             c.ajouterConge(IDC, IDE, DATEDEB, TYPE, PERIODE, "Refuse"); // Enregistrer le congé refusé
             ui->tableView_2->setModel(c.afficher()); // Actualiser la vue
             return;
         }
     } else {
         // Aucun résultat trouvé pour l'IDE donné
         QMessageBox::critical(this, "Erreur", "L'employé avec cet ID n'existe pas.");
         return;
     }


    // Vérification du solde avant d'ajouter le congé
    int soldeActuel = e.getSoldeConges(IDE);  // Récupérer le solde actuel

    if (soldeActuel < 0) {
        QMessageBox::warning(this, "Erreur", "Le solde de congé est invalide.");
        return;
    }

    if (soldeActuel < PERIODE) {
        QMessageBox::warning(this, "Erreur", "Solde de congé insuffisant pour cette période.");
        return;
    }

    // Ajouter le congé avec statut "Approuvé"
    if (c.ajouterConge(IDC, IDE, DATEDEB, TYPE, PERIODE, "Approuve")) {
        int nouveauSolde = soldeActuel - PERIODE;  // Calcul du nouveau solde après déduction

        if (c.mettreAJourSolde(IDE, nouveauSolde)) {
            qDebug() << "Solde de congé mis à jour avec succès.";
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la mise à jour du solde de congé.");
            return;
        }
        if (c.mettreAJourStatut(IDE, "Inactif")) {
                    QMessageBox::information(this, "Succès", "Congé approuvé et statut employé mis à jour.");
                    ui->tableView->setModel(e.afficher());  // Réactualiser la vue des employés
                } else {
                    QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du statut de l'employé.");
                }

        ui->tableView_2->setModel(c.afficher());

    }
    else {
        QMessageBox::critical(this, "Erreur", "Échec de l'approbation du congé.");
    }
}


void EmployeWindow::on_pushButton_refuser_clicked()
{
    if (!validerSaisie()) return;

    int IDC = ui->IDC->text().toInt();
    int IDE = ui->IDE->text().toInt();
    QString DATEDEB = ui->DATEDEB->text().trimmed();  // Utilisation de QDate
    QString TYPE = ui->TYPE->text().trimmed();
    int PERIODE = ui->PERIODE->text().toInt();

    conge c;

    // Ajouter le congé avec statut "Refusé"
    if (c.ajouterConge(IDC, IDE, DATEDEB, TYPE, PERIODE, "Refuse")) {
        QMessageBox::information(this, "Succès", "Congé refusé.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec du refus du congé.");
    }

    // Mise à jour du tableau des congés
    ui->tableView_2->setModel(c.afficher());
}

void EmployeWindow::on_pushButton_8_clicked()
{
    clientWindow = new ClientWindow(); // Create a new instance of ClientWindow
    clientWindow->show();              // Show the ClientWindow
    this->close();
}


void EmployeWindow::on_pushButton_10_clicked()
{
    serviceWindow = new ServiceWindow(); // Create a new instance of ClientWindow
    serviceWindow->show();              // Show the ClientWindow
    this->close();
}


void EmployeWindow::on_pushButton_11_clicked()
{
    produitWindow = new ProduitWindow(); // Create a new instance of ClientWindow
    produitWindow->show();              // Show the ClientWindow
    this->close();
}


void EmployeWindow::on_pushButton_12_clicked()
{
    Equipement = new equipement(); // Create a new instance of ServiceWindow
    Equipement->show();               // Show the ServiceWindow
    this->close();
}


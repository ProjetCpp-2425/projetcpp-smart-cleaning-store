#include "mainwindow.h"
#include "produit.h"
#include "ui_produit.h"
#include <QMessageBox>
#include "connection.h"
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QFileDialog>
#include "curvewidget.h"
#include "historique.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    loadProduitData();
    qDebug() << "Available database drivers: " << QSqlDatabase::drivers();
    // Connexion du bouton "Statistiques" avec la fonction showStatistics
    connect(ui->pushButton_Statistiques, &QPushButton::clicked, this, &MainWindow::showStatistics);
    //////
    serialbuffer="";

        QString portName;

           foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {

               if (info.portName() == "COM4") {

                   portName = info.portName();

                   break;

               }

           }



           if (portName.isEmpty()) {

               qDebug() << "COM4 not found.";

           }

              serialPort = new QSerialPort(portName);

              serialPort->setBaudRate(QSerialPort::Baud9600);

              serialPort->setDataBits(QSerialPort::Data8);

              serialPort->setParity(QSerialPort::NoParity);

              serialPort->setStopBits(QSerialPort::OneStop);

             connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readarduino);

              if (!
                      serialPort->open(QIODevice::ReadWrite)) {

                  qDebug() << "Failed to open the serial port.";

                  delete serialPort;



              }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouter_produit_clicked()
{
    int IDP = ui->lineEdit_IDP->text().toInt(); // Accessing UI elements correctly
    QString NomP = ui->lineEdit_NomP->text();
    int prixP = ui->lineEdit_prixP->text().toInt(); // Accessing UI elements correctly
    QString quentit = ui->lineEdit_quentit->text();
    QString categorie = ui->lineEdit_quentit_2->text();
    QString dateP = ui->lineEdit_dateP->text();

    Produit P(IDP, NomP, prixP, quentit, categorie, dateP);
    bool test = P.ajouter();
    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"), QObject::tr("Ajout effectué\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Ajout non effectué.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    int IDP = ui->lineEdit_IDP->text().toInt(); // Assuming lineEdit_IdpSupp is where the ID is entered

    // Call the supprimer() method on a Produit object or Etmp (an instance of Produit)
    Produit P;
    bool test = P.supprimer(IDP); // Attempt to delete the product with the specified ID

    // Check the result of the deletion and display appropriate message
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
}
void MainWindow::loadProduitData()
{
    // Create a new QSqlQueryModel
    QSqlQueryModel *model = new QSqlQueryModel(this);

    // Set the SQL query to fetch produit data
    model->setQuery("SELECT IDP, NOMP, PRIXP , QUANTITE, DATEP , CATEGORIE FROM PRODUIT");

    // Check if there was an error in the query
    if (model->lastError().isValid()) {
        qDebug() << "Error loading data from database: " << model->lastError().text();
        return;
    }

    // Set the model to the QTableView to display the data
    ui->tableView->setModel(model);

    // Resize the columns to fit the data
    ui->tableView->resizeColumnsToContents();
}
void MainWindow::on_Modifier_produit_clicked()
{
    // Get the updated product data from the input fields
    int IDP = ui->lineEdit_IDP->text().toInt(); // Accessing UI elements correctly
    QString NomP = ui->lineEdit_NomP->text();
    int prixP = ui->lineEdit_prixP->text().toInt(); // Accessing UI elements correctly
    QString quentit = ui->lineEdit_quentit->text();
    QString dateP = ui->lineEdit_dateP->text();// Get selected category from combo box
    QString categorie = ui->lineEdit_dateP->text();

    // Instantiate a Produit object with the updated data
    Produit P(IDP, NomP, prixP, quentit, categorie, dateP);

    // Call the modifier() method
    bool test = P.modifier(IDP); // Update the product with the specified Idp

    // Check the result of the update operation and show an appropriate message
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Modification effectuée\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
        loadProduitData(); // Refresh the table view after modification
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Modification non effectuée.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
}
void MainWindow::on_rechercherProduit_clicked()
{
    // Get the IDP value from the input field (assuming lineEdit_IDP is where the IDP is entered)
    int IDP = ui->lineEdit_IDP->text().toInt();

    // Instantiate a temporary Produit object to search for the product
    Produit produit;

    // Call the rechercher function
    if (produit.rechercher(IDP)) {
        // If the product was found, update the UI fields with product details
        QString NomP = ui->lineEdit_NomP->text();
        QString quentit = ui->lineEdit_quentit->text();
        QString categorie = ui->lineEdit_dateP->text();
        QString dateP = ui->lineEdit_dateP->text();

        QMessageBox::information(this, tr("Recherche Réussie"), tr("Produit trouvé et détails affichés."));
    } else {
        // If the product was not found, show an error message
        QMessageBox::warning(this, tr("Erreur de Recherche"), tr("Produit non trouvé. Veuillez vérifier l'ID."));
    }
}
/*void MainWindow::on_rechercherProduit_clicked()
{
    int IDP = ui->lineEdit_IDP->text().trimmed(); // Récupère l'ID du produit depuis l'interface utilisateur
    QSqlQueryModel* model = nullptr; // Initialise le modèle pour afficher les résultats

    // Crée un objet Produit pour gérer les interactions avec la base de données
    Produit produit;

    if (IDP.isEmpty()) {
        // Si le champ de recherche est vide, récupère tous les produits
        model = Produit.fetchAll();
    } else {
        bool isNumeric;
        int id = idText.toInt(&isNumeric); // Convertit l'entrée utilisateur en entier de manière sécurisée
        if (!isNumeric) {
            QMessageBox::critical(this, "Erreur", "L'ID saisi n'est pas valide. Veuillez entrer un nombre.");
            return;
        }
        model = P.rechercher(id); // Recherche un produit spécifique par ID
    }

    if (!model) {
        // Gestion des erreurs si le modèle est invalide
        QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de la recherche.");
        return;
    }

    // Associe le modèle à la vue (QTableView)
    ui->tableViewProduit->setModel(model);
    ui->tableViewProduit->resizeColumnsToContents(); // Ajuste les colonnes automatiquement

    if (model->rowCount() > 0) {
        if (!idText.isEmpty()) {
            // Si une recherche a été effectuée avec succès, ajoutez l'entrée dans
}*/

void MainWindow::on_trier_produit_clicked()
{
    // Instantiate a Produit object to access the trierParPrix method
    Produit P;

    // Call the trierParPrix function to sort by price in ascending or descending order
    bool ascending = true;  // Change to false if you want descending order
    QSqlQueryModel* sortedModel = P.trierParPrix(ascending);

    // Check if sorting succeeded
    if (sortedModel == nullptr || sortedModel->lastError().isValid()) {
        qDebug() << "Erreur lors du tri par prix : " << (sortedModel ? sortedModel->lastError().text() : "Erreur inconnue");
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri par prix.");
        return;
    }

    // Set the sorted model to the table view
    ui->tableView->setModel(sortedModel);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_exporterEnPDF_clicked()
{
    // Choose file path to save the PDF
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save as PDF"), "", tr("PDF Files (*.pdf);;All Files (*)"));
    if (filePath.isEmpty()) {
        return;  // User canceled
    }

    // Create the QPdfWriter object
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);  // Set resolution

    // Set up QPainter to write to the PDF
    QPainter painter(&pdfWriter);
    QFont font("Arial", 12);
    painter.setFont(font);
    painter.setPen(Qt::black);

    // Title
    painter.drawText(100, 100, "Liste des Produits");

    int yOffset = 150;  // Initial Y position for data
    int i = 1;  // Product count

    // Execute the query to retrieve products
    QSqlQuery query;
    if (!query.exec("SELECT IDP, NOMP, PRIXP, QUANTITE, DATEP, CATEGORIE FROM PRODUIT")) {
        QMessageBox::warning(this, tr("Query Error"), tr("Failed to execute query. Error: %1").arg(query.lastError().text()));
        return;
    }

    // Iterate over each product and write details in a structured format
    while (query.next()) {
        // Product header
        painter.drawText(50, yOffset, "Produit " + QString::number(i) + ":");
        i++;  // Increment count
        yOffset += 70;

        // Print each attribute
        painter.drawText(100, yOffset, "IDP: " + query.value("IDP").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Nom: " + query.value("NOMP").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Prix: " + query.value("PRIXP").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Quantité: " + query.value("QUANTITE").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Date: " + query.value("DATEP").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Catégorie: " + query.value("CATEGORIE").toString());

        // Add some space between each product
        yOffset += 140;
    }

    // Finalize the PDF
    painter.end();

    // Show a confirmation message
    QMessageBox::information(this, tr("PDF Generated"), tr("The product data has been saved as a PDF."));
}
//QChart *chart = new QChart();

/*void MainWindow::showStatistics()
{
    // Création de la série pour le graphique à secteurs
    QPieSeries *series = new QPieSeries();

    // Exécution de la requête pour obtenir le nombre de produits par catégorie
    QSqlQuery query;
    if (!query.exec("SELECT CATEGORIE, COUNT(*) as count FROM PRODUIT GROUP BY CATEGORIE")) {
        QMessageBox::warning(this, tr("Query Error"), tr("Failed to execute query. Error: %1").arg(query.lastError().text()));
        return;
    }

    // Remplir la série avec les données de la base de données
    while (query.next()) {
        QString categorie = query.value("CATEGORIE").toString();
        int count = query.value("count").toInt();
        series->append(categorie, count);
    }

    // Création et configuration du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Produits par Catégorie");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Configuration des couleurs et des labels pour chaque secteur
    for (auto slice : series->slices()) {
        slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));
    }

    // Création de la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Affichage du graphique dans une nouvelle fenêtre
    QMainWindow *statsWindow = new QMainWindow(this);
    statsWindow->setCentralWidget(chartView);
    statsWindow->resize(500, 400);
    statsWindow->show();
}*/
/*void MainWindow::showStatistics() {
    // Vérification si la connexion à la base de données est ouverte
    if (!db.isOpen()) {
        QMessageBox::warning(this, tr("Database Error"), tr("Database is not connected."));
        return;
    }

    // Créez la requête SQL pour obtenir les statistiques
    QSqlQuery query;
    query.prepare("SELECT CATEGORIE, COUNT(*) as count FROM PRODUIT GROUP BY CATEGORIE");

    if (!query.exec()) {
        // En cas d'erreur d'exécution de la requête
        QMessageBox::warning(this, tr("Query Error"), tr("Failed to execute query. Error: %1").arg(query.lastError().text()));
        return;
    }

    // Créez un modèle pour afficher les résultats
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(std::move(query));

    // Vérifiez si des données sont retournées
    if (model->rowCount() == 0) {
        QMessageBox::information(this, tr("No Data"), tr("No data found for the statistics."));
        return;
    }

    // Affichez les données dans le QTableView
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents(); // Redimensionne les colonnes pour s'adapter aux données
}*/


void MainWindow::showStatistics()
{
    CurveWidget *curveWidget = new CurveWidget();
    curveWidget->setWindowTitle("Courbe : Prix vs Quantité");
    curveWidget->resize(800, 600);
    curveWidget->show();
}
void MainWindow::on_afficherHistorique_clicked() {
    Historique historique;
    QSqlQueryModel *model = historique.afficher();

    // Assurez-vous que 'tableViewHistorique' est bien défini dans votre .ui
    ui->tableViewHistorique->setModel(model);
    ui->tableViewHistorique->resizeColumnsToContents();
}

/*void MainWindow::readarduino()
{

    int lastIndex;
    QString serialBuffer;
    QByteArray data = serialPort->readAll();
    serialBuffer += QString::fromUtf8(data);
    QStringList lines = serialBuffer.split('\n', Qt::SkipEmptyParts);

    if (!lines.isEmpty()) {
        serialBuffer = "";
        QString lastLine = lines.last();
        if (lastLine.startsWith("Received:")) {
            lastLine = lastLine.mid(9).trimmed();
        }
        bool conversionOk;
        lastIndex = lastLine.toInt(&conversionOk);
        if (conversionOk) {
            qDebug() << "---------------->" << lastIndex;
            ui->lineEdit_quentit->setText(QString::number(lastIndex));
            //make the modifications into the database here
        } else {
            qDebug() << "Conversion failed for line:" << lastLine;
        }
    } else {
        qDebug() << "No complete lines received yet.";
    }


}*/
void MainWindow::readarduino()
{
    int lastIndex;
    QString serialBuffer;
    QByteArray data = serialPort->readAll();
    serialBuffer += QString::fromUtf8(data);
    QStringList lines = serialBuffer.split('\n', Qt::SkipEmptyParts);

    if (!lines.isEmpty()) {
        serialBuffer = "";
        QString lastLine = lines.last();
        if (lastLine.startsWith("Received:")) {
            lastLine = lastLine.mid(9).trimmed();
        }

        bool conversionOk;
        lastIndex = lastLine.toInt(&conversionOk);

        if (conversionOk) {
            qDebug() << "---------------->" << lastIndex;
            ui->lineEdit_quentit->setText(QString::number(lastIndex));

            // Retrieve the product ID from the UI
            int IDP = ui->lineEdit_IDP->text().toInt();

            // Create a product object and set the updated quantity
            Produit P(IDP, "", 0, QString::number(lastIndex), "", "");

            // Update the database
            bool test = P.modifier(IDP);

            // Check the result and log an appropriate message
            if (test) {
                QMessageBox::information(nullptr, QObject::tr("OK"),
                                         QObject::tr("Quantity updated successfully\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
                loadProduitData(); // Refresh the table view after modification
            } else {
                QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                      QObject::tr("Quantity update failed.\n"
                                                  "Click Cancel to exit."), QMessageBox::Cancel);
            }
        } else {
            qDebug() << "Conversion failed for line:" << lastLine;
        }
    } else {
        qDebug() << "No complete lines received yet.";
    }
}




void MainWindow::on_pushButton_3_clicked()
{
    QString serialBuffer;
    QByteArray tosd = QByteArray::number(ui->lineEdit_quentit->text().toInt()) + '\n';
    qDebug() << "Sending: " << tosd;
    serialPort->write(tosd);
}


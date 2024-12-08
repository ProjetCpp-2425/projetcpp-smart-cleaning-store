#include "servicewindow.h"
#include "service.h"
#include "clientwindow.h"
#include "./ui_servicewindow.h"
#include "./ui_clientwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QInputDialog>
#include <QRegularExpression>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>


ServiceWindow::ServiceWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServiceWindow)
{
    ui->setupUi(this);
}

void ServiceWindow::on_btn_ajt_clicked() {
    int id = ui->id->text().toInt();
    QString nom = ui->nom->text();
    QString description = ui->description->text();
    float prix = ui->prix->text().toFloat();
    QDate date = ui->dated->date();
    QDate datef = ui->datef->date();
    QString statut = ui->statutbox->currentText();
    QString employe = ui->employe->text();
    QString categorie = ui->categorie->text();

    if (nom.isEmpty() || description.isEmpty() || prix <= 0 || date.isNull() || datef.isNull() ||
        statut.isEmpty() || employe.isEmpty() || categorie.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    service s(id, nom, description, prix, date, datef, statut, employe, categorie);

    QString message = QString("Êtes-vous sûr de vouloir %1 les données suivantes ?\n\n")
                        .arg(s.idExists() ? "mettre à jour" : "ajouter") +
                      QString("ID: %1\nNom: %2\nDescription: %3\nPrix: %4\nCatégorie: %5\nDate Début: %6\nDate Fin: %7\nStatut: %8\nEmployé: %9")
                        .arg(id)
                        .arg(nom)
                        .arg(description)
                        .arg(prix)
                        .arg(categorie)
                        .arg(date.toString("dd-MMM-yy"))
                        .arg(datef.toString("dd-MMM-yy"))
                        .arg(statut)
                        .arg(employe);

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Confirmation",
                                                              message,
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (s.idExists()) {
            if (s.update()) {
                QMessageBox::information(nullptr, "Succès", "Service mis à jour avec succès.");
            } else {
                QMessageBox::critical(nullptr, "Erreur", "La mise à jour du service a échoué.");
            }
        } else {
            if (s.ajouter()) {
                QMessageBox::information(nullptr, "Succès", "Service ajouté avec succès.");
            } else {
                QMessageBox::critical(nullptr, "Erreur", "L'ajout du service a échoué.");
            }
        }
    } else {
        return;
    }
}

void ServiceWindow::on_btn_recherche_clicked() {
    QString idRecherche = ui->idRecherche->text();

    if (idRecherche.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID pour la recherche.");
        service s(0, "", "", 0.0, QDate(), QDate(), "", "", "");
        s.sendToLCD("", "");
        return;
    }

    service s(0, "", "", 0.0, QDate(), QDate(), "", "", "");
    s.rechercherParId(ui->tableRecherche, idRecherche);

    if (ui->tableRecherche->rowCount() == 0) {
        QMessageBox::information(this, "Aucun résultat", "Aucun service trouvé pour l'ID donné.");

        s.sendToLCD("", "");
        return;
    }

    ui->tableRecherche->resizeColumnsToContents();

    ui->table_service->resizeColumnsToContents();

    QHeaderView* header = ui->tableRecherche->horizontalHeader();
    for (int i = 0; i < ui->tableRecherche->columnCount(); ++i) {
        header->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    ui->table_service->setStyleSheet("QTableWidget::item {"
                                    "background-color: #E5F3FD;"
                                    "border: 0px solid black;"
                                    "}");

    ui->table_service->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


void ServiceWindow::on_btn_pdf_clicked() {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database is not open:" << db.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }
    QSqlQuery query;
    if (!query.prepare("SELECT ID_SERVICE, NOM, DESCRIPTION, PRIX, CATEGORIE, DATED, DATEF, STATUT, EMPLOYE FROM SERVICE")) {
        qDebug() << "Failed to prepare query:" << query.lastError().text();
        QMessageBox::critical(this, "Query Error", "Failed to prepare query: " + query.lastError().text());
        return;
    }
    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
        QMessageBox::critical(this, "Query Error", "Failed to retrieve service data: " + query.lastError().text());
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(40, 40, 40, 40));

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "PDF Error", "Failed to create PDF!");
        return;
    }

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    int titleHeight = 350;
    painter.drawText(QRect(0, 50, pdfWriter.width(), titleHeight), Qt::AlignCenter, "Services Data Export");

    painter.setFont(QFont("Arial", 12));
    painter.drawText(QRect(0, 350 + titleHeight, pdfWriter.width(), 250), Qt::AlignCenter,
                     "Exported on: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    painter.setFont(QFont("Arial", 10));
    int y = 1500 + titleHeight;  // Starting position for the service data

    while (query.next()) {
        QString serviceData = QString(
            "ID: %1\n"
            "Name: %2\n"
            "Description: %3\n"
            "Price: %4\n"
            "Category: %5\n"
            "Start Date: %6\n"
            "End Date: %7\n"
            "Status: %8\n"
            "Employee: %9\n"
            "------------------------")
            .arg(query.value("ID_SERVICE").toString())
            .arg(query.value("NOM").toString())
            .arg(query.value("DESCRIPTION").toString())
            .arg(query.value("PRIX").toString())
            .arg(query.value("CATEGORIE").toString())
            .arg(query.value("DATED").toDate().toString("yyyy-MM-dd"))
            .arg(query.value("DATEF").toDate().toString("yyyy-MM-dd"))
            .arg(query.value("STATUT").toString())
            .arg(query.value("EMPLOYE").toString());

        painter.drawText(0, y, pdfWriter.width(), 2000, Qt::TextWordWrap, serviceData);
        y += 2400;  // Increase vertical spacing between service entries

        // Check if we need to start a new page
        if (y > pdfWriter.height() - 300) {
            pdfWriter.newPage();
            y = 100;  // Reset y position for the next page
        }
    }

    painter.end();
    QMessageBox::information(this, "Success", "PDF exported successfully to: " + fileName);

    // Now ask user to send the PDF via email
    QString email = QInputDialog::getText(this, "Recipient Email", "Enter recipient email address:");
    if (!email.isEmpty()) {
        // Ensure email format is correct (simple check)
        QRegularExpression emailRegex("[\\w-]+(\\.[\\w-]+)*@[\\w-]+(\\.[\\w-]+)+");
        QRegularExpressionMatch match = emailRegex.match(email);
        if (!match.hasMatch()) {
            QMessageBox::critical(this, "Invalid Email", "Please enter a valid email address.");
        } else {
            service s(0, "", "", 0.0, QDate(), QDate(), "", "", "");
            s.sendEmailWithAttachment(fileName, email);
        }
    }

    // Ask for SMS notification
    QString phoneNumber = QInputDialog::getText(this, "Recipient Phone Number", "Enter recipient phone number:");
    if (!phoneNumber.isEmpty()) {
        // Validate phone number format (basic validation)
        QRegularExpression phoneRegex("^\\+?[0-9]{10,15}$");
        QRegularExpressionMatch match = phoneRegex.match(phoneNumber);
        if (!match.hasMatch()) {
            QMessageBox::critical(this, "Invalid Phone Number", "Please enter a valid phone number.");
        } else {
            service s(0, "", "", 0.0, QDate(), QDate(), "", "", "");
            s.sendSMSNotification(phoneNumber);
        }
    }
}


void ServiceWindow::on_btn_aff_clicked() {
    int id = ui->id->text().toInt();
    QString nom = ui->nom->text();
    QString description = ui->description->text();
    float prix = ui->prix->text().toFloat();
    QDate date = ui->dated->date();
    QDate datef = ui->datef->date();
    QString statut = ui->statutbox->currentText();
    QString employe = ui->employe->text();
    QString categorie = ui->categorie->text();

    service s(id, nom, description, prix, date, datef, statut, employe, categorie);
    QSqlQuery query;

    QString queryStr = "SELECT * FROM SERVICE";

    query.prepare(queryStr);
    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to load service data.");
        return;
    }

    ui->table_service->setRowCount(0);

    while (query.next()) {
        int row = ui->table_service->rowCount();
        ui->table_service->insertRow(row);

        ui->table_service->setItem(row, 0, new QTableWidgetItem(query.value("ID_SERVICE").toString()));
        ui->table_service->setItem(row, 1, new QTableWidgetItem(query.value("NOM").toString()));
        ui->table_service->setItem(row, 2, new QTableWidgetItem(query.value("DESCRIPTION").toString()));
        ui->table_service->setItem(row, 3, new QTableWidgetItem(query.value("PRIX").toString()));
        ui->table_service->setItem(row, 4, new QTableWidgetItem(query.value("CATEGORIE").toString()));
        QString formattedDate = query.value("DATED").toDate().toString("yyyy-MM-dd");
        ui->table_service->setItem(row, 5, new QTableWidgetItem(formattedDate));

        QString formattedDatef = query.value("DATEF").toDate().toString("yyyy-MM-dd");
        ui->table_service->setItem(row, 6, new QTableWidgetItem(formattedDatef));

        ui->table_service->setItem(row, 7, new QTableWidgetItem(query.value("STATUT").toString()));
        ui->table_service->setItem(row, 8, new QTableWidgetItem(query.value("EMPLOYE").toString()));

    }

    ui->table_service->resizeColumnsToContents();

    QHeaderView* header = ui->table_service->horizontalHeader();
    for (int i = 0; i < ui->table_service->columnCount(); ++i) {
        header->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    ui->table_service->setStyleSheet("QTableWidget::item {"
                                    "background-color: #E5F3FD;"
                                    "border: 0px solid black;"
                                    "}");

    ui->table_service->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if (ui->comboBox->currentIndex() == 1) {
        ui->table_service->sortItems(0, Qt::AscendingOrder);
    } else if (ui->comboBox->currentIndex() == 2) {
        ui->table_service->sortItems(0, Qt::DescendingOrder);
    }
}

void ServiceWindow::on_btn_supprimer_clicked() {
    int id = ui->idservice->text().toInt();

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SERVICE WHERE ID_SERVICE = :ID_SERVICE");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification de l'ID.");
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(nullptr, "Erreur", "Le service avec cet ID n'existe pas.");
        return;
    }

    // Get the data of the service to confirm deletion
    query.prepare("SELECT NOM, DESCRIPTION, PRIX, CATEGORIE, DATED, DATEF, STATUT, EMPLOYE FROM SERVICE WHERE ID_SERVICE = :ID_SERVICE");
    query.addBindValue(id);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la récupération des données du service.");
        return;
    }

    query.next();
    QString nom = query.value("NOM").toString();
    QString description = query.value("DESCRIPTION").toString();
    float prix = query.value("PRIX").toFloat();
    QString categorie = query.value("CATEGORIE").toString();
    QDate date = query.value("DATED").toDate();
    QDate datef = query.value("DATEF").toDate();
    QString statut = query.value("STATUT").toString();
    QString employe = query.value("EMPLOYE").toString();

    // Prepare the confirmation message
    QString message = QString("Êtes-vous sûr de vouloir supprimer le service suivant ?\n\n") +
                      QString("ID: %1\nNom: %2\nDescription: %3\nPrix: %4\nCatégorie: %5\nDate Début: %6\nDate Fin: %7\nStatut: %8\nEmployé: %9")
                        .arg(id)
                        .arg(nom)
                        .arg(description)
                        .arg(prix)
                        .arg(categorie)
                        .arg(date.toString("dd-MMM-yy"))
                        .arg(datef.toString("dd-MMM-yy"))
                        .arg(statut)
                        .arg(employe);

    // Ask for confirmation
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Confirmation",
                                                              message,
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        service s(id, "", "", 0.0, QDate(), QDate(), "", "", "");
        bool success = s.supprimer(id);

        if (success) {
            QMessageBox::information(nullptr, "Succès", "Le service a été supprimé.");
        } else {
            QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la suppression du service.");
        }
    } else {
        // If user clicks No, do nothing
        return;
    }
}

void ServiceWindow::on_btn_statut_clicked() {
    // Query to get the count of services by statut
    QSqlQuery query;
    query.prepare("SELECT STATUT, COUNT(*) FROM SERVICE GROUP BY STATUT");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la récupération des données.");
        return;
    }

    // Create a QMap to hold the count of each statut
    QMap<QString, int> statutCounts;

    while (query.next()) {
        QString statut = query.value(0).toString(); // Change to column index for STATUT
        int count = query.value(1).toInt();
        statutCounts[statut] = count;
    }

    // Check if there is any data to display
    if (statutCounts.isEmpty()) {
        QMessageBox::information(nullptr, "Aucun Donnée", "Aucune donnée à afficher pour les statistiques.");
        return;
    }

    // Clear the existing layout and widgets in chartwidget
    QLayout *layout = ui->chartwidget->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget(); // Delete widget from layout
            delete item; // Delete layout item
        }
        delete layout;
    }

    // Create a chart widget
    QChartView *chartView = new QChartView();
    QChart *chart = new QChart();
    chartView->setChart(chart);

    // Add a bar series to the chart
    QBarSeries *series = new QBarSeries();
    for (auto it = statutCounts.begin(); it != statutCounts.end(); ++it) {
        QBarSet *set = new QBarSet(it.key());
        *set << it.value();
        series->append(set);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Statistiques des Services par Statut");

    // Set the chart view as the central widget of the chartWidget
    QVBoxLayout *newLayout = new QVBoxLayout();
    newLayout->addWidget(chartView);
    ui->chartwidget->setLayout(newLayout);
}

ServiceWindow::~ServiceWindow() {
    delete ui;
}



void ServiceWindow::on_pushButton_7_clicked()
{
    clientWindow = new ClientWindow(); // Create a new instance of ClientWindow
    clientWindow->show();              // Show the ClientWindow
    this->close();
}


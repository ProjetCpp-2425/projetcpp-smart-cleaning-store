#include "mainwindow.h"
#include "client.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , serialPort(nullptr) // Initialise l'objet à nullptr
    {
        ui->setupUi(this);

        // Configuration de l'Arduino
        QString portName;
        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
            if (info.portName() == "COM4") {
                portName = info.portName();
                break;
            }
        }

        if (portName.isEmpty()) {
            qDebug() << "Port COM4 non trouvé.";
            return;
        }

        serialPort = new QSerialPort(portName, this);
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);


        connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readarduino);

        if (!serialPort->open(QIODevice::ReadWrite)) {
            qDebug() << "Impossible d'ouvrir le port série.";
            delete serialPort;
            serialPort = nullptr;
        }
    }



   /* if (serialPort && serialPort->isOpen()) {
        serialPort->close();
    }
    delete ui;

*/
//arduino ...........
void MainWindow::on_btn_ajt_clicked() {
    // Vérification des champs vides
    if (ui->cin->text().isEmpty() || ui->id->text().isEmpty() || ui->num->text().isEmpty() ||
        ui->nom->text().isEmpty() || ui->adresse->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }


    // Récupération des informations saisies
    int cin = ui->cin->text().toInt();
    int id = ui->id->text().toInt();
    QString nom = ui->nom->text();
    int num = ui->num->text().toInt();
    QString adresse = ui->adresse->text();
    QDate date = ui->date->date();

    // Création de l'objet client
    client c(cin, id, nom, num, date, adresse);
    QMessageBox::information(nullptr, "Vérification",
                             "CIN: " + QString::number(cin) +
                             "\nID: " + QString::number(id) +
                             "\nNom: " + nom +
                             "\nNuméro: " + QString::number(num) +
                             "\nDate: " + date.toString("yyyy-MM-dd") +
                             "\nAdresse: " + adresse);
    // Ajout du client
    if (c.ajouter()) {
        QMessageBox::information(nullptr, "Succès", "Client ajouté avec succès.");
    } else {
        QMessageBox::critical(nullptr, "Erreur", "L'Ajout du Client a échoué.");
    }
}

void MainWindow::on_btn_recherche_clicked() {
    // Récupérer l'ID saisi par l'utilisateur
    QString idRecherche = ui->idRecherche->text();

    if (idRecherche.isEmpty()) {
        qDebug() << "Veuillez entrer un ID pour la recherche.";
        return;
    }

    // Créer un objet de type client et appeler la méthode de recherche
    client c;
    c.rechercherParId(ui->tableRecherche, idRecherche);  // Passer le QTableWidget et l'ID
    ui->tableRecherche->resizeColumnsToContents();

}



void MainWindow::on_btn_pdf_clicked()
{
    // Choisir le chemin pour enregistrer le PDF
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save as PDF"), "", tr("PDF Files (*.pdf);;All Files (*)"));
    if (filePath.isEmpty()) {
        return;  // L'utilisateur a annulé
    }

    // Créer l'objet QPdfWriter
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);  // Définir la résolution

    // Configurer QPainter pour écrire dans le PDF
    QPainter painter(&pdfWriter);
    QFont font("Times New Roman", 14);
    painter.setFont(font);
    painter.setPen(Qt::black);

    // Titre
    painter.drawText(100, 100, "Liste des Clients");

    int yOffset = 150;  // Position Y initiale pour les données
    int i = 1;  // Compteur de clients

    // Exécuter la requête pour récupérer les clients
    QSqlQuery query;
    if (!query.exec("SELECT CIN, ID, NOM, NUM, TRUNC(MONTHS_BETWEEN(SYSDATE, DATE_NAIS) / 12) AS AGE, ADRESSE FROM CLIENT")) {
        QMessageBox::warning(this, tr("Query Error"), tr("Failed to execute query. Error: %1").arg(query.lastError().text()));
        return;
    }

    // Parcourir chaque client et écrire les détails dans un format structuré
    while (query.next()) {
        yOffset += 70;
        // En-tête pour chaque client
        painter.drawText(50, yOffset, "Client " + QString::number(i) + ":");
        i++;  // Incrémenter le compteur
        yOffset += 70;

        // Imprimer chaque attribut
        painter.drawText(100, yOffset, "CIN: " + query.value("CIN").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "ID: " + query.value("ID").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Nom: " + query.value("NOM").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Numéro: " + query.value("NUM").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Âge: " + query.value("AGE").toString());
        yOffset += 70;
        painter.drawText(100, yOffset, "Adresse: " + query.value("ADRESSE").toString());

        // Ajouter un espace entre chaque client
        yOffset += 140;

        // Vérifier si on dépasse la page actuelle
        if (yOffset > pdfWriter.height() - 100) {
            pdfWriter.newPage();  // Ajouter une nouvelle page si nécessaire
            yOffset = 100;  // Réinitialiser la position Y
        }
    }

    // Finaliser le PDF
    painter.end();

    // Afficher un message de confirmation
    QMessageBox::information(this, tr("PDF Generated"), tr("Les données des clients ont été enregistrées dans un fichier PDF."));
}


void MainWindow::on_btn_annuler_clicked() {
    // Réinitialiser tous les champs de saisie
    ui->cin->clear();
    ui->id->clear();
    ui->nom->clear();
    ui->num->clear();
    ui->adresse->clear();
    ui->date->setDate(QDate::currentDate()); // Optionnel : définir la date actuelle par défaut

}


void MainWindow::on_btn_aff_clicked() {
    client c;
    QSqlQuery query;

    // Requête SQL incluant la date de naissance
    QString queryStr = "SELECT CIN, ID, NOM, NUM, DATE_NAIS, ADRESSE FROM CLIENT";

    // Ajouter le tri en fonction de l'option sélectionnée dans le comboBox
    if (ui->comboBox->currentIndex() == 1) {
        queryStr += " ORDER BY NOM ASC";  // Tri croissant par NOM
    } else if (ui->comboBox->currentIndex() == 2) {
        queryStr += " ORDER BY NOM DESC";  // Tri décroissant par NOM
    }

    // Préparer et exécuter la requête
    query.prepare(queryStr);
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        return;
    }

    // Vider le QTableWidget avant d'ajouter les nouvelles données
    ui->table_client->setRowCount(0);  // Supprimer les lignes existantes

    // Ajouter les données dans le QTableWidget
    while (query.next()) {
        int row = ui->table_client->rowCount();  // Obtenir le nombre actuel de lignes
        ui->table_client->insertRow(row);  // Ajouter une nouvelle ligne

        // Remplir les cellules de la ligne avec les données
        ui->table_client->setItem(row, 0, new QTableWidgetItem(query.value("CIN").toString()));
        ui->table_client->setItem(row, 1, new QTableWidgetItem(query.value("ID").toString()));
        ui->table_client->setItem(row, 2, new QTableWidgetItem(query.value("NOM").toString()));
        ui->table_client->setItem(row, 3, new QTableWidgetItem(query.value("NUM").toString()));

        // Formater la date en jj/mm/aaaa
        QDate dateNaiss = query.value("DATE_NAIS").toDate();
        QString formattedDate = dateNaiss.toString("dd/MM/yyyy");
        ui->table_client->setItem(row, 4, new QTableWidgetItem(formattedDate));

        ui->table_client->setItem(row, 5, new QTableWidgetItem(query.value("ADRESSE").toString()));
    }

    // Optionnel : ajuster la largeur des colonnes pour correspondre au contenu
    ui->table_client->resizeColumnsToContents();
}


void MainWindow::on_btn_supprimer_clicked() {
    // Récupérer l'ID du client sélectionné (par exemple depuis un champ de texte)
    int id = ui->idclient->text().toInt();  // Par exemple, si l'ID est dans un champ de texte

    QSqlQuery query;

    // Vérifier si l'ID existe dans la base de données
    query.prepare("SELECT COUNT(*) FROM CLIENT WHERE ID = :id");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification de l'ID.");
        return;  // Si une erreur survient, on quitte la fonction
    }

    query.next();
    int count = query.value(0).toInt();

    // Si l'ID n'existe pas, afficher un message d'erreur
    if (count == 0) {
        QMessageBox::warning(nullptr, "Erreur", "Le client avec cet ID n'existe pas.");
        return;  // Ne pas continuer si l'ID n'existe pas
    }

    // Créer un objet client et appeler la méthode supprimer()
    client c;
    bool success = c.supprimer(id);

    if (success) {
        QMessageBox::information(nullptr, "Succès", "Client supprimé avec succès.");
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression du client.");
    }
}



void MainWindow::on_statistique_clicked() {
    // Obtenez les statistiques des adresses depuis l'instance client
    QMap<QString, int> statistiques = clientInstance.statistiquesAdresse();

    // Vérifiez si les statistiques sont vides
    if (statistiques.isEmpty()) {
        QMessageBox::information(this, tr("Aucune donnée"), tr("Aucune donnée statistique trouvée."));
        return;
    }

    // Préparer les données pour le graphique
    QVector<QPair<QString, int>> chartData;
    QMap<QString, QColor> adresseColorMap;

    // Palette de couleurs par défaut
    QVector<QColor> colorPalette = {
        QColor(255, 99, 71),  // Tomato red
        QColor(70, 130, 180), // Steel blue
        QColor(34, 139, 34),  // Forest green
        QColor(255, 215, 0),  // Gold
        QColor(255, 99, 132), // Light pink
        QColor(0, 255, 255),  // Cyan
        QColor(138, 43, 226), // Blue violet
        QColor(255, 69, 0),   // Red orange
        QColor(32, 178, 170), // Light sea green
        QColor(128, 0, 128),  // Purple
    };

    // Assurez-vous que chaque adresse ait une couleur unique
    int colorIndex = 0;
    for (auto it = statistiques.begin(); it != statistiques.end(); ++it) {
        chartData.append(qMakePair(it.key(), it.value()));
        adresseColorMap[it.key()] = colorPalette[colorIndex % colorPalette.size()];
        colorIndex++;
    }

    // Calculer le total des clients
    int total = 0;
    for (const auto &item : chartData) {
        total += item.second;
    }

    // Vérifier si le total est zéro
    if (total == 0) {
        QMessageBox::information(this, tr("Aucune donnée"), tr("Aucune adresse trouvée."));
        return;
    }

    // --- Génération du graphique circulaire ---
    const int pieChartWidth = 400;
    const int pieChartHeight = 400;
    QImage pieChartImage(pieChartWidth, pieChartHeight, QImage::Format_ARGB32);
    pieChartImage.fill(Qt::white);

    QPainter pieChartPainter(&pieChartImage);
    pieChartPainter.setRenderHint(QPainter::Antialiasing);

    QRectF pieRect(50, 50, 300, 300);
    int startAngle = 0;

    for (const auto &item : chartData) {
        int spanAngle = (item.second * 360) / total;
        QColor color = adresseColorMap[item.first]; // Utilisez la couleur de l'adresse
        pieChartPainter.setBrush(color);

        pieChartPainter.drawPie(pieRect, startAngle * 16, spanAngle * 16);

        // Affichage du pourcentage et de l'adresse
        double percentage = (static_cast<double>(item.second) / total) * 100.0;
        int midAngle = startAngle + spanAngle / 2;
        QPointF textPosition(
            pieRect.center().x() + 120 * qCos(midAngle * M_PI / 180),
            pieRect.center().y() - 120 * qSin(midAngle * M_PI / 180)
        );

        pieChartPainter.setPen(Qt::black);
        pieChartPainter.drawText(textPosition, QString("%1 (%2%)").arg(item.first).arg(QString::number(percentage, 'f', 1)));

        startAngle += spanAngle;
    }

    QPixmap pieChartPixmap = QPixmap::fromImage(pieChartImage);

    // Affichage du graphique circulaire dans tableView_3
    QStandardItemModel *pieChartModel = new QStandardItemModel(1, 1, this);
    QStandardItem *pieChartItem = new QStandardItem();
    pieChartItem->setData(pieChartPixmap, Qt::DecorationRole);
    pieChartModel->setItem(0, 0, pieChartItem);
    ui->tableView_3->setModel(pieChartModel);
    ui->tableView_3->resizeColumnsToContents();
    ui->tableView_3->resizeRowsToContents();

    // --- Génération du graphique à barres ---
    const int barChartWidth = 500;
    const int barChartHeight = 300;
    QImage barChartImage(barChartWidth, barChartHeight, QImage::Format_ARGB32);
    barChartImage.fill(Qt::white);

    QPainter barChartPainter(&barChartImage);
    barChartPainter.setRenderHint(QPainter::Antialiasing);

    int barWidth = 50;
    int gap = 20;
    int xOffset = 40;
    int maxBarHeight = barChartHeight - 60;

    int maxValue = 0;
    for (const auto &item : chartData) {
        maxValue = qMax(maxValue, item.second);
    }

    float scaleFactor = static_cast<float>(maxBarHeight) / maxValue;

    for (const auto &item : chartData) {
        int barHeight = static_cast<int>(item.second * scaleFactor);
        QRectF barRect(xOffset, barChartHeight - barHeight - 30, barWidth, barHeight);

        QColor barColor = adresseColorMap[item.first]; // Utilisez la couleur de l'adresse
        barChartPainter.setBrush(barColor);
        barChartPainter.drawRect(barRect);

        // Affichage du nombre et du pourcentage au-dessus de chaque barre
        double percentage = (static_cast<double>(item.second) / total) * 100.0;
        barChartPainter.setPen(Qt::black);
        barChartPainter.drawText(
            QPointF(barRect.x() + barWidth / 4, barRect.y() - 5),
            QString("%1 (%2%)").arg(item.second).arg(QString::number(percentage, 'f', 1))
        );

        // Affichage de l'adresse sous chaque barre
        barChartPainter.drawText(
            QPointF(barRect.x() + barWidth / 4, barChartHeight - 10),
            item.first
        );

        xOffset += barWidth + gap;
    }

    QPixmap barChartPixmap = QPixmap::fromImage(barChartImage);

    // Affichage du graphique à barres dans tableView_4
    QStandardItemModel *barChartModel = new QStandardItemModel(1, 1, this);
    QStandardItem *barChartItem = new QStandardItem();
    barChartItem->setData(barChartPixmap, Qt::DecorationRole);
    barChartModel->setItem(0, 0, barChartItem);
    ui->tableView_4->setModel(barChartModel);
    ui->tableView_4->resizeColumnsToContents();
    ui->tableView_4->resizeRowsToContents();
}


MainWindow::~MainWindow()
{
    delete ui;
}
//arduino...........
void MainWindow::readarduino()
{
    if (!serialPort->isOpen()) {
        qDebug() << "Erreur d'ouverture du port série";
        return;
    }

    static QString serialBuffer; // Buffer persistant pour les données série
    static QDateTime lastAlertTime; // Dernière heure d'affichage d'une boîte de dialogue
    const int alertInterval = 10000; // Intervalle minimal entre deux alertes (en millisecondes)

    QByteArray data = serialPort->readAll(); // Lire toutes les nouvelles données

    if (data.isEmpty()) {
        qDebug() << "Aucune donnée reçue";
        return;
    }

    qDebug() << "Données reçues: " << data;
    serialBuffer += QString::fromUtf8(data); // Ajouter au buffer

    // Traiter les lignes complètes uniquement
    while (serialBuffer.contains('\n')) {
        QString line = serialBuffer.section('\n', 0, 0).trimmed(); // Extraire la première ligne
        serialBuffer = serialBuffer.section('\n', 1); // Supprimer la ligne traitée du buffer

        qDebug() << "Ligne lue: " << line;

        if (line == "Detected") {
            // Vérifier l'intervalle depuis la dernière alerte
            QDateTime currentTime = QDateTime::currentDateTime();
            if (lastAlertTime.isNull() || lastAlertTime.msecsTo(currentTime) > alertInterval) {
                qDebug() << "Client détecté par le capteur !";
                QMessageBox::information(this, "Détection de client", "Un client a été détecté !");
                lastAlertTime = currentTime; // Mettre à jour l'heure de la dernière alerte

                // Envoyer une commande pour activer le buzzer
                QByteArray command = "ACTIVATE_BUZZER\n"; // Commande pour activer le buzzer
                serialPort->write(command);
                qDebug() << "Commande 'ACTIVATE_BUZZER' envoyée à l'Arduino.";
            } else {
                qDebug() << "Alerte ignorée pour éviter les doublons.";
            }
        }
    }
}

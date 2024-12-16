#include "equipement.h"
#include "servicewindow.h"
#include "clientwindow.h"
#include "produitwindow.h"
#include "employewindow.h"
#include "./ui_equipement.h"
#include "./ui_servicewindow.h"
#include "./ui_clientwindow.h"
#include "./ui_produitwindow.h"
#include "./ui_employewindow.h"
#include <QDate>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QBuffer>
#include <QByteArray>
#include <QStyledItemDelegate>

class ImageDelegate : public QStyledItemDelegate
{
public:
    ImageDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        if (index.column() == 6) // Adjust column index to your image_data column
        {
            QByteArray imageData = index.data().toByteArray();
            QPixmap pixmap;
            pixmap.loadFromData(QByteArray::fromBase64(imageData));

            if (!pixmap.isNull())
            {
                QRect rect = option.rect;
                QSize size = pixmap.size().scaled(rect.size(), Qt::KeepAspectRatio);
                QPoint center = rect.center() - QPoint(size.width() / 2, size.height() / 2);

                painter->drawPixmap(QRect(center, size), pixmap);
                return;
            }
        }

        QStyledItemDelegate::paint(painter, option, index);
    }
};
equipement::equipement(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::equipement)
{
    ui->setupUi(this);
    QDate currentDate = QDate::currentDate();

    ui->dateEdit->setDate(currentDate);
    ui->tableView->setModel(etmp.afficher());
        ImageDelegate* imageDelegate = new ImageDelegate(this);
        ui->tableView->setItemDelegateForColumn(6, imageDelegate);

    /*int ret=a.connect_arduino();
    switch(ret)
    {
    case(0):qDebug()<<"arduino is available and connected to : "<<a.getarduino_port_name();
        break;
    case(1):qDebug()<<"arduino is available and not connected to : "<<a.getarduino_port_name();
        break;
    case(-1):qDebug()<<"arduino is not available";
        break;
    }
    QObject::connect(a.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));*/
    connect(ui->tableView, &QTableView::doubleClicked, this, &equipement::on_tableView_doubleClicked);

}
void equipement::update_label()
{
    /*static QByteArray buffer; // Buffer to accumulate data
    QByteArray new_data = a.read_from_arduino(); // Read new data from Arduino
    buffer.append(new_data); // Append new data to the buffer

    // Check if the buffer contains a complete message (e.g., ends with '\n')
    if (buffer.contains("\r\n"))
    {
        QString completeMessage = QString::fromUtf8(buffer); // Convert to QString
        completeMessage = completeMessage.trimmed(); // Remove trailing newline characters

        qDebug() << "Complete data:" << completeMessage;

        ui->led->setText(completeMessage); // Update the label with the complete message
        buffer.clear(); // Clear the buffer after processing
    }
*/
}

equipement::~equipement()
{
    delete ui;
}

void equipement::on_pushButton_2_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (!select->hasSelection()) {
        QMessageBox::critical(this, "Error", "Please select a row to delete.");
        return;
    }

    // Get the selected row index and extract the ID from the model
    int row = select->currentIndex().row();
    int id = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();
    equipe e=etmp.getEquipeById(id);
    if (e.get_id()!=id){
       QMessageBox::critical(this, "Error", "This ID doesn't exist!");
    }else
    {
        bool test=etmp.supprimer(id);
        if (test)
        {
            ui->tableView->setModel(etmp.afficher());
            QMessageBox::information(nullptr, QObject::tr("ok"),
                        QObject::tr("suppression effectue.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
        }
        else
            QMessageBox::critical(nullptr, QObject::tr("not ok"),
                        QObject::tr("suppression non effectue.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
        ui->stackedWidget->setCurrentIndex(0);
        ui->tableView->setModel(etmp.afficher());


    }
}


void equipement::on_pushButton_8_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (!select->hasSelection()) {
        QMessageBox::critical(this, "Error", "Please select a row to delete.");
        return;
    }

    int row = select->currentIndex().row();
    int id = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();
    equipe e=etmp.getEquipeById(id);
    if (e.get_id()!=id){
       QMessageBox::critical(this, "Error", "This ID doesn't exist!");
    }else
    {
        bool test=etmp.supprimer(id);
        if (test)
        {
            ui->tableView->setModel(etmp.afficher());
            QMessageBox::information(nullptr, QObject::tr("ok"),
                        QObject::tr("suppression effectue.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
        }
        else
            QMessageBox::critical(nullptr, QObject::tr("not ok"),
                        QObject::tr("suppression non effectue.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
        ui->stackedWidget->setCurrentIndex(0);
        ui->tableView->setModel(etmp.afficher());


    }
}
void equipement::on_tableView_doubleClicked(const QModelIndex &index)
{
    // Vérifiez si la colonne correspond à celle contenant les images
    if (index.column() != 6) { // Colonne 6 contient l'image
        return;
    }

    // Récupérer les données de l'image
    QByteArray imageData = index.data().toByteArray();
    QPixmap pixmap;
    if (!pixmap.loadFromData(QByteArray::fromBase64(imageData))) {
        QMessageBox::critical(this, "Erreur", "Impossible de charger l'image.");
        return;
    }

    // Créer un QDialog pour afficher l'image
    QDialog imageDialog(this);
    imageDialog.setWindowTitle("Agrandir l'image");
    imageDialog.resize(800, 600); // Taille de la fenêtre

    QLabel *imageLabel = new QLabel(&imageDialog);
    imageLabel->setPixmap(pixmap.scaled(imageDialog.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(imageLabel);
    imageDialog.setLayout(layout);

    imageDialog.exec(); // Affiche la fenêtre
}


void equipement::on_pushButton_3_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (!select->hasSelection()) {
        QMessageBox::critical(this, "Error", "Please select a row to modify.");
        return;
    }

    int row = select->currentIndex().row();
    int id = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();
    if (!etmp.checkIfIdExists(id)) {
            QMessageBox::warning(this, "ID Not Found", "The ID does not exist. Please enter a valid ID.");
            return;
        }
        ui->stackedWidget->setCurrentIndex(1);
        equipe e = etmp.getEquipeById(id);
        ui->idm_2->setText(QString::number(e.get_id()));
        ui->nomm->setText(e.get_nom());
        ui->typem->setText(e.get_type());
        ui->datem->setDate(e.get_date());
        ui->etatm->setText(e.get_etat());
}



void equipement::on_pushButton_11_clicked()
{

    QDate currentDate = QDate::currentDate();

    int dispo;
    if (ui->true_2->isChecked()) {
        ui->dispom1->setChecked(true);
    } else if (ui->false_2->isChecked()) {
        ui->dispom2->setChecked(false);
    }
    int id=ui->idm_2->text().toInt();
        QString nom=ui->nomm->text();
        QString type=ui->typem->text();
        QString etat=ui->etatm->text();
        QDate date=ui->datem->date();
        if (ui->dispom1->isChecked())
            dispo=1;
        else if(ui->dispom2->isChecked())
            dispo=0;
    equipe even = etmp.getEquipeById(id);
    if (even.get_id() == id) {
               // QMessageBox::critical(this, "Error", "wait a bit !");
        }else if (id <= 0) {
            QMessageBox::critical(this, "Error", "The ID cannot be negative!");
        }else if (nom.isEmpty()){
            QMessageBox::critical(this, "Error", "The name cannot be empty!");
        }else if (etat.isEmpty()){
            QMessageBox::critical(this, "Error", "The etat cannot be empty!");
        }else if (type.isEmpty()){
            QMessageBox::critical(this, "Error", "The type cannot be empty!");
        }else if (date < currentDate){
            QMessageBox::critical(this, "Error", "The date cannot be in the past.");
        }
            bool test=etmp.modifier(id,dispo,type,nom,etat,date);
            if (test)
            {
                ui->tableView->setModel(etmp.afficher());
                QMessageBox::information(nullptr, QObject::tr("ok"),
                            QObject::tr("modification effectue.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
            }
            else
                QMessageBox::critical(nullptr, QObject::tr("not ok"),
                            QObject::tr("modification non effectue.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
            ui->stackedWidget->setCurrentIndex(0);


}


void equipement::on_ajoutbutton_clicked()
{
    int dispo=-1;
    if (ui->true_2->isChecked()) {
        dispo = 1;
    } else if (ui->false_2->isChecked()) {
        dispo = 0;
    }
    QDate currentDate = QDate::currentDate();

    int id=ui->id->text().toInt();
    QString nom=ui->nom->text();
    QString type=ui->type->text();
    QString etat=ui->etat->text();
    QDate date=ui->dateEdit->date();
    //image
    QString selectedImagePath;
    QByteArray imageData = etmp.processImageWithDialog(selectedImagePath);

    // Afficher l'image dans le QLabel
    if (!selectedImagePath.isEmpty() && ui->image_label != nullptr) {
        QPixmap pixmap(selectedImagePath);
            if (!pixmap.isNull()) {
                ui->image_label->setPixmap(pixmap.scaled(ui->image_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    QFileInfo FileInfo(selectedImagePath);
    QString imagename =FileInfo.fileName();
    //
    equipe even = etmp.getEquipeById(id);
    if (even.get_id() == id) {
                QMessageBox::critical(this, "Error", "This ID exists already!");
        }else if (id <= 0) {
            QMessageBox::critical(this, "Error", "The ID cannot be negative!");
        }else if (nom.isEmpty()){
            QMessageBox::critical(this, "Error", "The name cannot be empty!");
        }else if (etat.isEmpty()){
            QMessageBox::critical(this, "Error", "The etat cannot be empty!");
        }else if (type.isEmpty()){
            QMessageBox::critical(this, "Error", "The type cannot be empty!");
        }else if (date < currentDate){
            QMessageBox::critical(this, "Error", "The date cannot be in the past.");
        }
    else if (dispo==-1){
        QMessageBox::critical(this, "Error", "The disponibilite cannot be empty!");

    }
    else {
    equipe E(id,dispo,type,nom,etat,date,imageData,imagename);
    bool test=E.ajouter();
    if (test)
    {
        ui->tableView->setModel(E.afficher());
        QMessageBox::information(nullptr, QObject::tr("ok"),
                    QObject::tr("ajout effectue.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        ui->etat->clear();
        ui->nom->clear();
        ui->id->clear();
        ui->type->clear();


    }
    else{
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                    QObject::tr("ajout non effectue.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ui->stackedWidget->setCurrentIndex(0);
    }
}


void equipement::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void equipement::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}


void equipement::on_search_textChanged(const QString &arg1)
{
 QSqlQueryModel *searchModel = etmp.search(arg1);
 if (searchModel!=nullptr) {

     ui->tableView->setModel(searchModel);
 } else{
     QMessageBox::critical(this, "Error", "Search failed. No results found.");
 }
}



void equipement::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1=="id"){
            ui->tableView->setModel(etmp.sort("id"));
        }else if (arg1=="nom"){
            ui->tableView->setModel(etmp.sort("nom"));
        }else if (arg1=="type"){
            ui->tableView->setModel(etmp.sort("type"));
        }else if (arg1=="etat"){
            ui->tableView->setModel(etmp.sort("etat"));
        }else if (arg1=="disponibilite"){
            ui->tableView->setModel(etmp.sort("disponibilite"));
        }else if (arg1=="date d'achat"){
            ui->tableView->setModel(etmp.sort("date_dachat"));
        }
}


void equipement::on_pushButton_6_clicked()
{
    if (etmp.exportToPdf()==true) {
               QMessageBox::information(this, "Done", "PDF exported successfully!");
           } else {
               QMessageBox::critical(this, "Error", "Failed to export PDF!");
           }
}


void equipement::on_del_all_clicked()
{
    bool test=etmp.supprimer_all();

            if (test){
                ui->tableView->setModel(etmp.afficher());
                QMessageBox::information(nullptr, QObject::tr("ok"),
                            QObject::tr("Eveything's been deleted.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);

            }else
                QMessageBox::critical(nullptr, QObject::tr("not ok"),
                            QObject::tr("Error.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
}

QSqlQueryModel* equipe::getEquipeStatistics()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT type, COUNT(ID) as equipe_count FROM equippement GROUP BY type");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("type"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("equipe_count"));
        return model;
    } else {
        qDebug() << "Error retrieving equipe statistics:" << query.lastError().text();
        return nullptr;
    }
}

void equipement::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
        equipe myEquipe;
           QSqlQueryModel *model = myEquipe.getEquipeStatistics();
           qreal tot = 0;
           QMap<QString, qreal> typeCounts;
           QMapIterator<QString, qreal> it(typeCounts);

           if (model) {
               // Calculate totals and populate typeCounts
                  for (int i = 0; i < model->rowCount(); ++i) {
                      QString gouvernorat = model->data(model->index(i, 0)).toString();
                      qreal count = model->data(model->index(i, 1)).toDouble();
                      typeCounts.insert(gouvernorat, count);
                      tot += count; // Accumulate total count
                  }

                  // Create series based on typeCounts
                  QPieSeries *seriesC = new QPieSeries();
                  QMapIterator<QString, qreal> it(typeCounts);
                  while (it.hasNext()) {
                      it.next();
                      qreal percentage = (it.value() / tot) * 100;
                      QString label = it.key() + " (" + QString::number(percentage, 'f', 2) + "%)";
                      seriesC->append(label, it.value());

                  }

               QChart *chartC = new QChart();
               chartC->addSeries(seriesC);
               chartC->setTitle("Statistique des Event");
               chartC->setTheme(QChart::ChartThemeLight);
               chartC->legend()->setVisible(true);
               chartC->legend()->setAlignment(Qt::AlignRight);
               QChartView *chartViewC = new QChartView(chartC);
               chartC->setBackgroundBrush(QBrush(QColor(87, 169, 209)));

               QGraphicsScene *scene = new QGraphicsScene();
               scene->addWidget(chartViewC);

               QColor colors(87, 169, 209);
               QBrush brush(colors);
               scene->setBackgroundBrush(brush);

               chartViewC->setFixedSize(700, 700);
               scene->setSceneRect(QRectF(chartViewC->rect()));

               QPixmap pixmap(scene->sceneRect().size().toSize());
               pixmap.fill(Qt::transparent);
               QPainter painter(&pixmap);
               chartViewC->render(&painter);


               ui->label_stat_equipe->setPixmap(pixmap);

               delete model;
           }
}


void equipement::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}


void equipement::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    etmp.highlightEquipeDates(ui->calendarWidget);
}


void equipement::on_calendarWidget_selectionChanged()
{
    //QString x=ui->calendarWidget->selectedDate().toString("dd/MM/yyyy 00:00");
    ui->lineEdit_calendar_affichage->setText(ui->calendarWidget->selectedDate().toString());
    ui->tableView_calendar->setModel(etmp.afficher_calendar(ui->calendarWidget->selectedDate()));
}


void equipement::on_pushbutton_client_clicked()
{
    clientWindow = new ClientWindow(); // Create a new instance of ClientWindow
    clientWindow->show();              // Show the ClientWindow
    this->close();
}


void equipement::on_pushButton_produit_clicked()
{
    produitWindow = new ProduitWindow(); // Create a new instance of ServiceWindow
    produitWindow->show();               // Show the ServiceWindow
    this->close();
}


void equipement::on_pushButton_service_clicked()
{
    serviceWindow = new ServiceWindow(); // Create a new instance of ServiceWindow
    serviceWindow->show();               // Show the ServiceWindow
    this->close();
}


void equipement::on_pushButton_employe_clicked()
{
    employeWindow = new EmployeWindow(); // Create a new instance of ServiceWindow
    employeWindow->show();               // Show the ServiceWindow
    this->close();
}


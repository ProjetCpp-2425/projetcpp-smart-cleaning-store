#include "equipement.h"
#include "ui_equipement.h"
#include <QDate>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QGraphicsScene>
equipement::equipement(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::equipement)
{
    ui->setupUi(this);
    QDate currentDate = QDate::currentDate();

    ui->dateEdit->setDate(currentDate);

    ui->tableView->setModel(etmp.afficher());
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


void equipement::on_pushButton_3_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (!select->hasSelection()) {
        QMessageBox::critical(this, "Error", "Please select a row to modify.");
        return;
    }

    // Get the selected row index and extract the ID from the model
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


void equipement::on_pushButton_19_clicked()
{
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
    equipe E(id,dispo,type,nom,etat,date);
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
        qDebug() << "Error retrieving event statistics:" << query.lastError().text();
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


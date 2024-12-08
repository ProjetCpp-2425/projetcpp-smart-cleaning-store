#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "conge.h"
#include "employe.h"
#include <QMainWindow>
#include <QCompleter>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QString>

#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "dialogscore.h"
#include "arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_Modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_chercher_textChanged(const QString &arg1);
    void on_tri_clicked();
    void on_resetButton_clicked();
    void on_exporterEnPDF_clicked();
    bool setupEmployeeStatsChart();
    void on_refresh_Clicked();
    void on_pushButton_approuver_clicked();  // Approve button clicked
    void on_pushButton_refuser_clicked();    // Reject button clicked
    void onEmployeModified();
    void on_pushButton_goToScorePage_clicked();




public slots:
   /* void handleEmailStatus(const QString &status);*/

private:
    Ui::MainWindow *ui;
    QSqlQueryModel* getEmployeeStats();
    employe Etmp;
     bool validerSaisie();
     void showErrorMessage(const QString &message);
     conge c;
    arduino a;
    QByteArray data;

    //new

};






#endif // MAINWINDOW_H

#ifndef PRODUITWINDOW_H
#define PRODUITWINDOW_H
#include <QMainWindow>
#include "produit.h"
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<qpieseries.h>
#include<QtCharts>

class ClientWindow;
class ServiceWindow;
class equipement;
class EmployeWindow;

namespace Ui {
class ProduitWindow;
}

class ProduitWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ProduitWindow(QWidget *parent = nullptr);
    ~ProduitWindow();
public slots:
    void readarduino();

private slots:
    void on_pushButton_6_clicked();
    void loadProduitData();
    void on_ajouter_produit_clicked();
    void on_Modifier_produit_clicked();
    void on_rechercherProduit_clicked();
    void on_exporterEnPDF_clicked();
    void on_trier_produit_clicked();
    void showStatistics();
    void on_afficherHistorique_clicked();
    void on_pushButton_3_clicked();
    void logProductToFile(const QString &idp, const QString &nomp);
    void on_pushButton_Statistiques_clicked();
    void displayProductsFromFile(QWidget *parent);
    void on_pushButton_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

private:
    Ui::ProduitWindow *ui;
    ClientWindow *clientWindow;
    ServiceWindow *serviceWindow;
    EmployeWindow *employeWindow;
    equipement *Equipement;
    QSqlDatabase db;
    Produit Etmp;
    QSerialPort *serialPort;
    QByteArray serialData;
    QString serialbuffer;

    Produit P;
};

#endif // PRODUITWINDOW_H

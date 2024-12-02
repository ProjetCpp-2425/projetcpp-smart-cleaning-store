#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "produit.h"
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void readarduino();


private slots:
    void on_pushButton_6_clicked();
    void loadProduitData();
    void on_ajouter_produit_clicked();
    void on_Modifier_produit_clicked();
    void on_rechercherProduit_clicked();
    /*void on_trierProduit_clicked();*/
    void on_exporterEnPDF_clicked();
    void on_trier_produit_clicked();
    void showStatistics();
    void on_afficherHistorique_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
     QSqlDatabase db;
    Produit Etmp;
    QSerialPort *serialPort;
    QByteArray serialData;
    QString serialbuffer;

    Produit P;
};

#endif // MAINWINDOW_H

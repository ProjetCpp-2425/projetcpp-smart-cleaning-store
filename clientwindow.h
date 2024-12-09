#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <client.h>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include "equipement.h"
QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ServiceWindow;
class MainWindow;
class equipement;

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

public slots:
    void readarduino();


private slots:
    void on_btn_ajt_clicked();
    void on_btn_aff_clicked();
    void on_btn_supprimer_clicked();
    void on_btn_recherche_clicked();
    void on_btn_pdf_clicked();
    void on_btn_annuler_clicked();
    void on_statistique_clicked();
    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    ServiceWindow *serviceWindow;
    Ui::ClientWindow *ui;
    client clientInstance;
    QString serialbuffer;
    QSerialPort *serialPort;
    MainWindow *mainwindow;
    equipement *equippement;

};
#endif // CLIENTWINDOW_H

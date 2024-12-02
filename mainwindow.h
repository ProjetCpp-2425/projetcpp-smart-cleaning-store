#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <client.h>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
private:
    Ui::MainWindow *ui;
    client clientInstance; // Déclarez une instance de votre classe Client
    QString serialbuffer;
    QSerialPort *serialPort;

};
#endif // MAINWINDOW_H

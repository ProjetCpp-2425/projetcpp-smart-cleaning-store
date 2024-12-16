#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H
#include "equipe.h"
#include "arduino.h"
#include <QMainWindow>
#include <QtSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class equipement; }
QT_END_NAMESPACE

class ClientWindow;
class EmployeWindow;
class ServiceWindow;
class ProduitWindow;

class equipement : public QMainWindow
{
    Q_OBJECT

public:
    equipement(QWidget *parent = nullptr);
    ~equipement();

private slots:

    void on_pushButton_2_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_11_clicked();
    void on_ajoutbutton_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_clicked();
    void on_search_textChanged(const QString &arg1);
    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_pushButton_6_clicked();
    void on_del_all_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_5_clicked();
    void on_calendarWidget_selectionChanged();
    void update_label();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushbutton_client_clicked();

    void on_pushButton_produit_clicked();

    void on_pushButton_service_clicked();

    void on_pushButton_employe_clicked();

private:
    ClientWindow *clientWindow;
    ProduitWindow *produitWindow;
    ServiceWindow *serviceWindow;
    EmployeWindow *employeWindow;
    Ui::equipement *ui;
    equipe etmp;
    QByteArray data;
};
#endif // EQUIPEMENT_H

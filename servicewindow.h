#ifndef SERVICEWINDOW_H
#define SERVICEWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ServiceWindow; }
QT_END_NAMESPACE

class ClientWindow;
class ProduitWindow;
class equipement;
class EmployeWindow;

class ServiceWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServiceWindow(QWidget *parent = nullptr);
    ~ServiceWindow();

private slots:
    void on_btn_ajt_clicked();
    void on_btn_aff_clicked();
    void on_btn_supprimer_clicked();
    void on_btn_recherche_clicked();
    void on_btn_pdf_clicked();
    void on_btn_statut_clicked();
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    ClientWindow *clientWindow;
    ProduitWindow *produitWindow;
    EmployeWindow *employeWindow;
    equipement *Equipement;
    Ui::ServiceWindow *ui;
};
#endif // SERVICEWINDOW_H

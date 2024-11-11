#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H
#include "equipe.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class equipement; }
QT_END_NAMESPACE

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

    void on_pushButton_19_clicked();

    void on_pushButton_11_clicked();

    void on_ajoutbutton_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_clicked();

    void on_search_textChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_del_all_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::equipement *ui;
    equipe etmp;
};
#endif // EQUIPEMENT_H

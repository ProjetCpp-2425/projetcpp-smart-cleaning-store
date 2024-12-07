#ifndef DIALOGMODIFIER_H
#define DIALOGMODIFIER_H

#include "employe.h"
#include <QDialog>

namespace Ui {
class Dialogmodifier;
}

class Dialogmodifier : public QDialog
{
    Q_OBJECT

public:
    // Constructor that accepts the employee ID (IDE) and loads the employee data
    explicit Dialogmodifier(QWidget *parent = nullptr, int IDE = -1);
    ~Dialogmodifier();

    // Set the employee ID
    void setidE(int IDE);

signals:
    void employeModified();  // Signal emitted when the employee is modified

private slots:
    // Slot to handle Modify button click
    void on_pushButton_Modifier_clicked();
    // Slot to handle Cancel button click
    void on_pushButton_Annuler_clicked();

private:
    Ui::Dialogmodifier *ui;
    int IDE;  // Employee ID to identify the employee to modify

    // Method to load the employee data into the dialog fields
    void loadEmployeData(int IDE);
};

#endif // DIALOGMODIFIER_H

#include "dashboarddialog.h"
#include "ui_dashboarddialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include<QMessageBox>

DashboardDialog::DashboardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DashboardDialog) {
    ui->setupUi(this);

    // Set default date range to the current month
    QDate currentDate = QDate::currentDate();
    ui->dateEditStart->setDate(currentDate.addMonths(-1));  // Default to 1 month before
    ui->dateEditEnd->setDate(currentDate);                 // Default to today

    // Load data initially
    loadDashboardData();

    // Connect the Load button to refresh the dashboard data
    connect(ui->buttonLoad, &QPushButton::clicked, this, &DashboardDialog::loadDashboardData);

    // Connect close button
    connect(ui->buttonClose, &QPushButton::clicked, this, &DashboardDialog::close);
}

DashboardDialog::~DashboardDialog() {
    delete ui;
}

void DashboardDialog::loadDashboardData() {
    QDate startDate = ui->dateEditStart->date();
    QDate endDate = ui->dateEditEnd->date();

    QSqlQuery query;

    // Prepare the SQL query
    QString sqlQuery =
        "SELECT "
        "  (SELECT COUNT(*) FROM EMPLOYE WHERE DATE_EMBE BETWEEN :startDate AND :endDate) AS totalEmployees, "
        "  (SELECT COUNT(*) FROM EMPLOYE WHERE GENREE = 'homme' AND DATE_EMBE BETWEEN :startDate AND :endDate) AS maleCount, "
        "  (SELECT COUNT(*) FROM EMPLOYE WHERE GENREE = 'femme' AND DATE_EMBE BETWEEN :startDate AND :endDate) AS femaleCount, "
        "  (SELECT AVG(AGEE) FROM EMPLOYE WHERE DATE_EMBE BETWEEN :startDate AND :endDate) AS averageAge, "
        "  (SELECT AVG(SALAIREE) FROM EMPLOYE WHERE DATE_EMBE BETWEEN :startDate AND :endDate) AS averageSalary, "
        "  (SELECT COUNT(*) FROM CONGE WHERE DATEDEB BETWEEN :startDate AND :endDate AND statut = 'Approuvé') AS approvedLeaves, "
        "  (SELECT COUNT(*) FROM CONGE WHERE DATEDEB BETWEEN :startDate AND :endDate) AS totalLeaves "
        "FROM DUAL"; // Ensure you're selecting from a valid table (e.g., "DUAL" for Oracle)

    // Debugging: Print the generated SQL to verify it's correct
    qDebug() << "SQL Query: " << sqlQuery;

    query.prepare(sqlQuery);

    // Bind the parameters (startDate and endDate as strings in 'YYYY-MM-DD' format)
    query.bindValue(":startDate", startDate.toString("yyyy-MM-dd"));
    query.bindValue(":endDate", endDate.toString("yyyy-MM-dd"));

    // Execute the query and process the results
    if (query.exec() && query.next()) {
        int totalEmployees = query.value("totalEmployees").toInt();
        int maleCount = query.value("maleCount").toInt();
        int femaleCount = query.value("femaleCount").toInt();
        double averageAge = query.value("averageAge").toDouble();
        double averageSalary = query.value("averageSalary").toDouble();
        int approvedLeaves = query.value("approvedLeaves").toInt();
        int totalLeaves = query.value("totalLeaves").toInt();

        // Calculate percentages for male and female
        double malePercentage = (totalEmployees > 0) ? (maleCount * 100.0 / totalEmployees) : 0.0;
        double femalePercentage = (totalEmployees > 0) ? (femaleCount * 100.0 / totalEmployees) : 0.0;

        // Update the UI with the retrieved data
        ui->labelTotalEmployees->setText(QString::number(totalEmployees));
        ui->labelMalePercentage->setText(QString::number(malePercentage, 'f', 2) + "%");
        ui->labelFemalePercentage->setText(QString::number(femalePercentage, 'f', 2) + "%");
        ui->labelAverageAge_->setText(QString::number(averageAge, 'f', 2) + " ans");
        ui->labelAverageSalary_->setText(QString::number(averageSalary, 'f', 2) + " DT");
        ui->labelApprovedLeaves->setText(QString::number(approvedLeaves) + " / " + QString::number(totalLeaves));
    } else {
        // Display an error message if the query fails
        QMessageBox::warning(this, tr("Error"), tr("Failed to load data: %1").arg(query.lastError().text()));
    }
}


void DashboardDialog::on_buttonLoad_clicked()
{

        loadDashboardData();
}


#include "dialogmodifier.h"
#include "ui_dialogmodifier.h"
#include <QMessageBox>

// Constructor that accepts the employee ID (IDE) and loads the employee data
Dialogmodifier::Dialogmodifier(QWidget *parent, int IDE) :
    QDialog(parent),
    ui(new Ui::Dialogmodifier),
    IDE(IDE)  // Initialize the employee ID
{
    ui->setupUi(this);
    loadEmployeData(IDE);  // Load the employee data using the ID
    connect(ui->Modifier, &QPushButton::clicked, this, &Dialogmodifier::on_pushButton_Modifier_clicked);
    connect(ui->Annuler, &QPushButton::clicked, this, &Dialogmodifier::on_pushButton_Annuler_clicked);
}

// Destructor to clean up
Dialogmodifier::~Dialogmodifier()
{
    delete ui;
}

// Set the employee ID
void Dialogmodifier::setidE(int IDE)
{
    this->IDE = IDE;
    loadEmployeData(IDE);  // Load the employee data after setting the ID
}

// Load the employee data into the dialog fields
void Dialogmodifier::loadEmployeData(int IDE)
{
    employe e;
    bool found = e.loadEmployeById(IDE);  // Load the employee data based on the ID

    if (!found) {
        QMessageBox::critical(this, tr("Erreur"), tr("Employé non trouvé"));
        return;
    }

    // Assuming the employee class has getters for each field:
    ui->CIN->setText(e.getCIN());
    ui->NUM->setText(e.getNUM());
    ui->NOM->setText(e.getNom());
    ui->PRENOM->setText(e.getPrenom());
    ui->MAIL->setText(e.getMail());
    ui->POSTE->setText(e.getPoste());
    ui->DATE_EMB->setText(e.getdate_emb());
    ui->STATUT->setText(e.getstatut());
    ui->GENRE->setText(e.getgenre());
    ui->AGE->setText(QString::number(e.getage()));
    ui->SALAIRE->setText(QString::number(e.getSALAIRE()));
}


void Dialogmodifier::on_pushButton_Modifier_clicked()
{
    QString CINE = ui->CIN->text();
    QString NUME = ui->NUM->text();
    QString NOME = ui->NOM->text();
    QString PRENOME = ui->PRENOM->text();
    QString MAILE = ui->MAIL->text();
    QString POSTEE = ui->POSTE->text();
    QString DATE_EMBE = ui->DATE_EMB->text();
    QString STATUTE = ui->STATUT->text();
    QString GENREE = ui->GENRE->text();
    int AGEE = ui->AGE->text().toInt();
    int SALAIREE = ui->SALAIRE->text().toInt();

    qDebug() << "SALAIREE Value before modification:" << SALAIREE;

    // Retrieve current employee data
    employe e;
    bool found = e.loadEmployeById(IDE);

    if (!found) {
        QMessageBox::critical(this, tr("Erreur"), tr("Employé non trouvé"));
        return;
    }

    // Only update fields if the user has provided new values
    if (!CINE.isEmpty()) e.setCIN(CINE);
    if (!NUME.isEmpty()) e.setNUM(NUME);
    if (!NOME.isEmpty()) e.setNom(NOME);
    if (!PRENOME.isEmpty()) e.setPrenom(PRENOME);
    if (!MAILE.isEmpty()) e.setMail(MAILE);
    if (!POSTEE.isEmpty()) e.setPoste(POSTEE);
    if (!DATE_EMBE.isEmpty()) e.setdate_emb(DATE_EMBE);
    if (!STATUTE.isEmpty()) e.setstatut(STATUTE);
    if (!GENREE.isEmpty()) e.setGENRE(GENREE);
    if (AGEE > 18) e.setAGE(AGEE);
    if (SALAIREE > 0) {
           e.setSALAIRE(SALAIREE);  // Set the new salary value
       } else {
           qDebug() << "SALAIREE not valid. It remains 0.";
       }

   // Validate age before updating

    // Save the updated employee information
    bool test = e.modifier(IDE);  // Pass the ID to modify the employee

    if (test) {
        // Display success message
        QMessageBox::information(this, tr("Succès"), tr("Employé modifié avec succès."));
        close();  // Close the dialog
        emit employeModified();  // Emit signal indicating employee was modified
    } else {
        // Display failure message if modification fails
        QMessageBox::critical(this, tr("Échec"), tr("Échec de la modification de l'employé."));
    }
}

// Slot for handling the "Cancel" button click
void Dialogmodifier::on_pushButton_Annuler_clicked()
{
    close();  // Close the dialog without modifying the employee
}

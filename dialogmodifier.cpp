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
    loadEmployeData(IDE);
}

void Dialogmodifier::loadEmployeData(int IDE)
{
    employe e;
    bool found = e.loadEmployeById(IDE);

    if (!found) {
        QMessageBox::critical(this, tr("Erreur"), tr("Employé non trouvé"));
        return;
    }

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

    employe e;
    bool found = e.loadEmployeById(IDE);

    if (!found) {
        QMessageBox::critical(this, tr("Erreur"), tr("Employé non trouvé"));
        return;
    }

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
           e.setSALAIRE(SALAIREE);
       } else {
           qDebug() << "SALAIREE not valid. It remains 0.";
       }

    bool test = e.modifier(IDE);

    if (test) {

        QMessageBox::information(this, tr("Succès"), tr("Employé modifié avec succès."));
        close();
        emit employeModified();
    } else {
        QMessageBox::critical(this, tr("Échec"), tr("Échec de la modification de l'employé."));
    }
}

void Dialogmodifier::on_pushButton_Annuler_clicked()
{
    close();
}

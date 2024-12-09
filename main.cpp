#include "servicewindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//
    Connection c;
    bool test = c.createconnection();
    if (!test) {
        QMessageBox::critical(
            nullptr, QObject::tr("Data is not open"),
            QObject::tr("Échec de connexion.\nCliquez sur Annuler pour quitter."),
            QMessageBox::Cancel);
        return 0;
    }

    QMessageBox::information(
        nullptr, QObject::tr("Data is open"),
        QObject::tr("Connecté avec succès.\nCliquez sur OK pour continuer."),
        QMessageBox::Ok);
    //
    ServiceWindow w;
    w.show();
    return a.exec();
}

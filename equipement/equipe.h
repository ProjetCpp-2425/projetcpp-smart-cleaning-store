#ifndef EQUIPE_H
#define EQUIPE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QMessageBox>
#include <QSqlError>
#include <QPdfWriter>
#include <QTextDocument>
#include <QTextCursor>
#include <QSqlRecord>
#include <QPrinter>
#include <QStandardPaths>
class equipe
{
private:
   QString nom,type,etat;
   QDate date_achat;
   bool dispo;
   int id;

public:
    equipe();
    equipe(int,bool,QString,QString,QString,QDate);
    QString get_type(){return type;}
    QString get_nom(){return nom;}
    QString get_etat(){return etat;}
    int get_id(){return id;}
    QDate get_date(){return date_achat;}
    bool get_dispo(){return dispo;}

    void set_type(QString t){type=t;}
    void set_nom(QString n){nom=n;}
    void set_etat(QString e){etat=e;}
    void set_id(int id){this->id=id;}
    void set_date(QDate d){this->date_achat=d;}
    void set_dispo(bool dispo){this->dispo=dispo;}

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier(int,bool,QString,QString,QString,QDate);
    bool checkIfIdExists(int id);
    equipe getEquipeById(int id);
    bool supprimer_all();
    QSqlQueryModel *search(const QString &criteria);
    QSqlQueryModel *sort(const QString &criteria);
    bool exportToPdf();
    QSqlQueryModel* getEquipeStatistics();






};

#endif // EQUIPE_H

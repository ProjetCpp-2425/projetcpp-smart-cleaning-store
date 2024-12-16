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
#include <QCalendarWidget>
#include <QPixmap>
#include <QFile>
#include <QByteArray>
class equipe
{
private:
   QString nom,type,etat,image_name;
   QDate date_achat;
   bool dispo;
   int id;
   QByteArray image_data;

public:
    equipe();
    equipe(int,bool,QString,QString,QString,QDate,QByteArray,QString);
    QString get_type(){return type;}
    QString get_nom(){return nom;}
    QString get_etat(){return etat;}
    int get_id(){return id;}
    QDate get_date(){return date_achat;}
    bool get_dispo(){return dispo;}
    QString get_image_name(){return image_name;}
    QString get_image_data(){return image_data;}

    void set_type(QString t){type=t;}
    void set_nom(QString n){nom=n;}
    void set_etat(QString e){etat=e;}
    void set_id(int id){this->id=id;}
    void set_date(QDate d){this->date_achat=d;}
    void set_dispo(bool dispo){this->dispo=dispo;}
    void set_image_name(QString i){image_name=i;}
    void set_image_data(QByteArray d){this->image_data=d;}

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
    void highlightEquipeDates(QCalendarWidget *calendarWidget);

    QSqlQueryModel * afficher_calendar(const QDate &selectedDate);

    QByteArray processImageWithDialog(QString& selectedImagePath);



};

#endif // EQUIPE_H

#ifndef EMPLOYE_H
#define EMPLOYE_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QSqlError>
#include <QPdfWriter>
#include <QTextDocument>
#include <QTextCursor>
#include <QSqlRecord>
#include <QStandardPaths>
#include <QPainter>

class employe
{
private:
    int IDE,AGEE;
    QString NOME,PRENOME,MAILE,POSTEE,DATE_EMBE,STATUTE,GENREE,CINE,NUME;


public:
    employe(){}
    employe(int,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    int getID(){return IDE;}
    QString getCIN(){return CINE;}
    QString getNUM(){return NUME;}
    QString getNom(){return NOME;}
    QString getPrenom(){return PRENOME;}
    QString getMail(){return MAILE;}
    QString getPoste(){return POSTEE;}
    QString getdate_emb(){return DATE_EMBE;}
    QString getstatut(){return STATUTE;}
    QString getgenre(){return GENREE;}
    int getage(){return AGEE;}


    void setID(int id){this ->IDE=id;}
    void setCIN(QString cin){this ->CINE=cin;}
    void setNUM(QString num){this ->NUME=num;}
    void setNom(QString n){NOME=n;}
    void setPrenom(QString pr){PRENOME=pr;}
    void setMail(QString m){MAILE=m;}
    void setPoste(QString p){POSTEE=p;}
    void setdate_emb(QString date){DATE_EMBE=date;}
    void setstatut(QString s ){STATUTE=s;}
    void setGENRE(QString g){GENREE=g;}
    void setAGE(int a){this ->AGEE=a;}

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int);
    bool loadEmployeById(int IDE); // Charge les données de l'employé par ID
    bool modifier(int IDE);
    bool checkIfIdExists(int IDE);
    employe getEmployeById(int IDE);
    //new
    QSqlQueryModel* trier(const QString& critere);
    QSqlQueryModel *chercher(const QString &critere);
    QSqlQueryModel* getEquipeStatistics();
    bool exportToPdf();

    // Method declaration

   };


//new




#endif // EMPLOYE_H

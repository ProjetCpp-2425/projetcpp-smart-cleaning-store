#ifndef PRODUIT_H
#define PRODUIT_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>

class Produit
{
    QString  NomP, quentit, categorie, dateP ;
    int IDP ,prixP ;
public:
    //constructeur
    Produit(){}
    Produit(int, QString, int , QString, QString,QString);
    //getters
    QString getNonP(){return NomP;}
    int getprixP(){return prixP;}
    QString getquentit(){return quentit;}
    QString getcategorie(){return categorie;}
    QString getdateP(){return dateP;}
    int getIDP(){return IDP;}
    //setters
    void setNonP(QString n){NomP=n;}
    void setprixP(int pr){prixP=pr;}
    void setquentit(QString qu){quentit=qu;}
    void setcategorie(QString ca){categorie=ca;}
    void setdateP(QString dt){dateP=dt;}
    void setIDP(int id){this->IDP=id;}
    // fonctionnalités de base relatives a l'entité produit
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int IdP);
    bool modifier(int IDP);
    bool rechercher(int IDP);
    bool trierParPrix();
    QSqlQueryModel* trierParPrix(bool ascending);
   /* bool trier(const QString& column, const QString& order);*/
};

#endif // PRODUIT_H

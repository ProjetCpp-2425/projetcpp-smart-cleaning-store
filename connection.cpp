#include "connection.h"
Connection::Connection(){}
bool Connection::createconnection()
{
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
bool test=false;
db.setHostName("DESKTOP-N5FUK8J");
db.setPort(1521);
db.setDatabaseName("Source_Projet2A");
db.setUserName("root");
db.setPassword("0000");

if (db.open()) test=true;
    return  test;
}
/*
bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("2a30source");//inserer le nom de la source de données
db.setUserName("projetcpp2a30");//inserer nom de l'utilisateur
db.setPassword("esprit24");//inserer mot de passe de cet utilisateur


}*/
//void Connection::closeConnection(){db.close();}


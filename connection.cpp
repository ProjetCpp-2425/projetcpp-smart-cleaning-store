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
//void Connection::closeConnection(){db.close();}


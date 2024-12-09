#include "connection.h"
Connection::Connection(){}
bool Connection::createconnection()
{
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
bool test=false;
db.setHostName("DESKTOP-N5FUK8J");
db.setPort(1521);
db.setDatabaseName("2a30source");
db.setUserName("roue");
db.setPassword("esprit");
if (db.open()) test=true;
    return  test;
}
//void Connection::closeConnection(){db.close();}


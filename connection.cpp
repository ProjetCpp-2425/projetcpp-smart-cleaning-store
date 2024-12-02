#include "connection.h"
Connection::Connection(){}
bool Connection::createconnection()
{
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
bool test=false;
db.setHostName("DESKTOP-N5FUK8J");
db.setPort(1521);
db.setDatabaseName("CPP_project");
db.setUserName("KHALIL");
db.setPassword("oracle");
if (db.open()) test=true;
    return  test;
}
//void Connection::closeConnection(){db.close();}


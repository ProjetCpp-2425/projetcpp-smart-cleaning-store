#include "connexion.h"

connexion::connexion()
{

}
bool connexion::createconnect()
{
    bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("ahmed");
    db.setUserName("ahmed");
    db.setPassword("ahmed");

    if (db.open())
        test=true;
    return  test;
}

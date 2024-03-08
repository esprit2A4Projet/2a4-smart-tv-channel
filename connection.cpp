#include "connection.h"

connection::connection()
{

}
bool connection::createconnect()
{bool test=false;


QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A4");
db.setUserName("system");//inserer nom de l'utilisateur
db.setPassword("ismail02");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

    return  test;
}

void connection :: closeConnection(){db.close();}

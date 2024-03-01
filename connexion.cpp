#include "connexion.h"

Connection::Connection()
{

}
bool Connection::createconnect()
{bool test=false;


QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A4");
db.setUserName("system");//inserer nom de l'utilisateur
db.setPassword("Islemsallouma1");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

    return  test;
}

void Connection :: closeConnection(){db.close();}

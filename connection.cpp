#include "connection.h"
#include <QtSql>
Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A4");
db.setUserName("amine");//inserer nom de l'utilisateur
db.setPassword("amine");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

    return  test;
}
bool Connection::insertData(int id, const QString &nom, const QString &type, const QString &etat, const QString &quantite, const QString &date)
{
    QSqlQuery query;
    query.prepare("INSERT INTO MATERIAUX (ID_MATERIEL, NOM, TYPE, ETAT, QUANTITE, DATE_MISE) VALUES (:id, :nom, :type, :etat, :quantite, :date)");
    query.bindValue(":id", id);
        query.bindValue(":nom", nom);
        query.bindValue(":type", type);
        query.bindValue(":etat", etat);
        query.bindValue(":quantite", quantite);
        query.bindValue(":date", date);

    return query.exec();
}

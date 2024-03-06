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

//void Connection :: closeConnection(){db.close();}

bool Connection::insertData(const QString &nom, const QString &budget, const QString &pack, const QString &date_deb, const QString &date_fin, const QString &telephone)
{
    QSqlQuery query;
    query.prepare("INSERT INTO SPONSOR (NOM, BUDGET, PACK, DATE_DEB, DATE_FIN, TELEPHONE) VALUES (:nom, :budget, :pack, :date_deb, :date_fin, :telephone)");
        query.bindValue(":nom", nom);
        query.bindValue(":budget", budget);
        query.bindValue(":pack", pack);
        query.bindValue(":date_deb", date_deb);
        query.bindValue(":date_fin", date_fin);
        query.bindValue(":telephone", telephone);

    return query.exec();
}

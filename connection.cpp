#include "connection.h"
#include <QtSql>
#include <QSqlError>
#include <QSqlQuery>

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet");
db.setUserName("ibtissem");//inserer nom de l'utilisateur
db.setPassword("ibtissem");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;


    return  test;
}
bool Connection::insertData(const QString &nom, const QString &prenom, const QString &profession, const QString &email, int &nbAbonnes, int &nbParticipation)
{
    QSqlQuery query;
    query.prepare("INSERT INTO INVITE (NOM, PRENOM, PROFESSION, EMAIL, NBABONNES, NBPARTICIPATION) "
                  "VALUES (:nom, :prenom, :profession, :email, :nbAbonnes, :nbParticipation)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":profession", profession);
    query.bindValue(":email", email);
    query.bindValue(":nbAbonnes", nbAbonnes);
    query.bindValue(":nbParticipation", nbParticipation);

    if (query.exec()) {
        // Insertion successful
        return true;
    } else {
        // Insertion failed, print error details
        qDebug() << "Error: " << query.lastError().text();
        return false;
    }
}

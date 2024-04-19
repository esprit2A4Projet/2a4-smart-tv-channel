#include "connexion.h"
#include <QtSql>
Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A4");
db.setUserName("louayzorai");//inserer nom de l'utilisateur
db.setPassword("louayzorai");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;


    return  test;
}
bool Connection::insertData(const QString &modeDePaiement, const QString &type, const QString &categorie, const QString &dateTransaction, const QString &montant)
{
        QSqlQuery query;
        query.prepare("INSERT INTO TRANSACTIONS (MODEDEPAIEMENT, TYPE, CATEGORIE, DATETRANSACTION, MONTANT) VALUES (:modeDePaiement, :type, :categorie, :dateTransaction, :montant)");
        query.bindValue(":modeDePaiement", modeDePaiement);
        query.bindValue(":type", type);
        query.bindValue(":categorie", categorie);
        query.bindValue(":dateTransaction", dateTransaction);
        query.bindValue(":montant", montant);
        return query.exec();
}

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
db.setDatabaseName("Source_Projet2A4");
db.setUserName("amine");//inserer nom de l'utilisateur
db.setPassword("amine");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

    return  test;
}

void Connection :: closeConnection(){db.close();}
// Ibtissem :
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

// Mokh


bool Connection::insertDataS(const QString &nom, const QString &budget, const QString &pack, const QString &date_deb, const QString &date_fin, const QString &telephone)
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
// Zoh :

bool Connection::insertDataM(const QString &nom, const QString &type, const QString &etat, const QString &quantite, const QString &date, int idTransaction) {
    QSqlQuery query;
    query.prepare("INSERT INTO MATERIAUX (NOM, TYPE, ETAT, QUANTITE, DATE_MISE, ID_TRANSACTION) VALUES (:nom, :type, :etat, :quantite, :date, :idTransaction)");
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    query.bindValue(":quantite", quantite);
    query.bindValue(":date", date);
    query.bindValue(":idTransaction", idTransaction);

    return query.exec();
}
// Louay :

bool Connection::insertDataT(const QString &modeDePaiement, const QString &type, const QString &categorie, const QString &dateTransaction, const QString &montant)
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

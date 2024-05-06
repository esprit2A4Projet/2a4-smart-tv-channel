#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection
{



public:
    Connection();
     bool createconnect();
     void closeConnection();

     QSqlDatabase db;
     bool insertData(const QString &nom, const QString &prenom, const QString &profession, const QString &email, int &nbAbonnes, int &nbParticipation );
     bool insertDataS(const QString &nom, const QString &budget, const QString &pack, const QString &date_deb,const QString &date_fin,const QString &telephone );
     bool insertDataM(const QString &nom, const QString &type, const QString &etat, const QString &quantite, const QString &date, int idTransaction);
     bool insertDataT(const QString &modeDePaiement, const QString &type, const QString &categorie, const QString &dateTransaction,const QString &montant);

};

#endif // CONNECTION_H


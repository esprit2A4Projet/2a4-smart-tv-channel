#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection
{
    QSqlDatabase db;
public:
    Connection();
    bool createconnect();
    bool insertData(const QString &modeDePaiement, const QString &type, const QString &categorie, const QString &dateTransaction,const QString &montant);
};

#endif // CONNECTION_H

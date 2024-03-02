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
    bool insertData(const QString &nom, const QString &prenom, const QString &profession, const QString &email,const QString &nbAbonnes,const QString &nbParticipation );
    QSqlDatabase db;
};

#endif // CONNECTION_H

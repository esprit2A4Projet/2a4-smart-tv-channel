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
    bool insertData(const QString &nom, const QString &prenom, const QString &profession, const QString &email,const QString &nbAbonnes,const QString &nbParticipation );
};

#endif // CONNECTION_H

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
     bool insertData(const QString &nom, const QString &budget, const QString &pack, const QString &date_deb,const QString &date_fin,const QString &telephone );
     //void closeConnection();
};

#endif // CONNECTION_H


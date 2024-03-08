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
    bool insertData(int id, const QString &nom, const QString &type, const QString &etat, const QString &quantite, const QString &date);
};
#endif // CONNECTION_H

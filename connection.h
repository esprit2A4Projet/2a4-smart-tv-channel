#ifndef CONNEXION_H
#define CONNEXION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class connection
{
     QSqlDatabase db;
public:
    connection();
     bool createconnect();
     void closeConnection();
};

#endif // CONNECTION_H

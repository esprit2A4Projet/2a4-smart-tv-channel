#include "transaction.h"
#include <QSqlError>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlQueryModel>

Transaction::Transaction()
{
    id="";
    dateTransaction="";
    montant="";
    modeDePaiement="";
    categorie="";
    type="";
}
Transaction::Transaction(QString a,QString b,QString c,QString d,QString e)
{


    modeDePaiement=a;
    type=b;
    categorie=c;
    montant=d;
    dateTransaction=e;

}

void Transaction::set_id(QString n){id=n;}
void Transaction::set_dateTransaction(QString n){dateTransaction=n;}
void Transaction::set_montant(QString n){montant=n;}
void Transaction::set_categorie(QString n){categorie=n;}
void Transaction::set_type(QString n){type=n;}
void Transaction::set_modeDePaiement(QString n){modeDePaiement=n;}


QString Transaction::get_id(){return id;}
QString Transaction::get_dateTransaction(){return dateTransaction;}
QString Transaction::get_montant(){return montant;}
QString Transaction::get_categorie(){return categorie;}
QString Transaction::get_type(){return type;}
QString Transaction::get_modeDePaiement(){return modeDePaiement;}

bool Transaction::ajouter()
{
             QSqlQuery query;
             query.prepare("INSERT INTO TRANSACTION (modeDePaiement, type, categorie,montant,dateTransaction) " "VALUES (:modeDePaiement, :type, :categorie,:montant,:dateTransaction)");
             query.bindValue(0,modeDePaiement);
             query.bindValue(1,type);
             query.bindValue(2,categorie);
             query.bindValue(3,montant);
             query.bindValue(4,dateTransaction);
             return query.exec();
}
QSqlQueryModel* Transaction::afficher()
{
         QSqlQueryModel* model= new QSqlQueryModel();
         model->setQuery("SELECT* FROM TRANSACTION");
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
         model->setHeaderData(1, Qt::Horizontal, QObject:: tr("modeDePaiement"));
         model->setHeaderData(2, Qt::Horizontal, QObject:: tr("type"));
         model->setHeaderData(3, Qt::Horizontal, QObject:: tr("categorie"));
         model->setHeaderData(4, Qt::Horizontal, QObject:: tr("montant"));
         model->setHeaderData(5, Qt::Horizontal, QObject:: tr("date de Transaction"));
 return model;
}


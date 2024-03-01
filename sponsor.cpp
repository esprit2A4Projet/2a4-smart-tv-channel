#include "sponsor.h"
#include "connection.h"
#include <QString>
#include <QDebug>
#include <QObject>
#include <QSqlQueryModel>

Sponsor::Sponsor()
{
    id_sponsor=0;
    nom="";
    budget="";
    pack="rien";
    date_deb="";
    date_fin="";
    telephone="";
}

int Sponsor::Getid_sponsor()
{ return id_sponsor; }
void Sponsor::Setid_sponsor(int val)
{ id_sponsor = val; }
QString Sponsor::Getnom()
{ return nom; }
void Sponsor::Setnom(QString val)
{ nom = val; }
QString Sponsor::Getbudget()
{ return budget; }
void Sponsor::Setbudget(QString val)
{ budget = val; }
QString Sponsor::Getpack()
{ return pack; }
void Sponsor::Setpack(QString val)
{ pack = val; }
QString Sponsor::Getdate_deb()
{ return date_deb; }
void Sponsor::Setdate_deb(QString val)
{ date_deb = val; }
QString Sponsor::Getdate_fin()
{ return date_fin; }
void Sponsor::Setdate_fin(QString val)
{ date_fin = val; }
QString Sponsor::Gettelephone()
{ return telephone; }
void Sponsor::Settelephone(QString val)
{ telephone = val; }

Sponsor::Sponsor(QString n, QString bud, QString date1, QString date2, QString tel)
{

    this->nom=n;
    this->budget=bud;
    this->date_deb=date1;
    this->date_fin=date2;
    this->telephone=tel;
}

bool Sponsor::ajouter()
{
    QSqlQuery query;

    query.prepare("insert into SPONSOR(nom, budget, date_deb, date_fin, telephone)" "values (:nom, :budget, :date1 ,:date2 , :telephone)");

    query.bindValue(":nom", nom);
    query.bindValue(":budget",budget);
    query.bindValue(":date1",date_deb);
    query.bindValue(":date2",date_fin);
    query.bindValue(":telephone",telephone);

    return query.exec();
}


QSqlQueryModel * Sponsor::afficher()
{
   QSqlQueryModel* model=new QSqlQueryModel();

         model->setQuery("SELECT* FROM SPONSOR");
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
         model->setHeaderData(1, Qt::Horizontal, QObject:: tr("Nom"));
         model->setHeaderData(2, Qt::Horizontal, QObject:: tr("Budge"));
         model->setHeaderData(3, Qt::Horizontal, QObject:: tr("Pack"));
         model->setHeaderData(4, Qt::Horizontal, QObject:: tr("Date debut"));
         model->setHeaderData(5, Qt::Horizontal, QObject:: tr("Date fin"));
         model->setHeaderData(5, Qt::Horizontal, QObject:: tr("Telephone"));

 return model;
}

bool Sponsor::supprimer(int id)
{
    QSqlQuery query;
           query.prepare("Delete from SPONSOR where id_sposnor=:id");
           query.bindValue(0, id);

            return query.exec();
}

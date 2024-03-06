#include "sponsor.h"
#include "connection.h"
#include <QString>
#include <QDebug>
#include <QObject>
#include <QSqlQueryModel>
#include <QTableWidget>

Sponsor::Sponsor()
{
    nom="";
    budget="";
    pack="";
    date_deb="";
    date_fin="";
    telephone="";
}

Sponsor::Sponsor(int id_sponsor, QString nom, QString budget, QString pack, QString date_deb, QString date_fin, QString telephone)
{
    this->id_sponsor=id_sponsor;
    this->nom=nom;
    this->budget=budget;
    this->pack=pack;
    this->date_deb=date_deb;
    this->date_fin=date_fin;
    this->telephone=telephone;
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


bool Sponsor::ajouter()
{
    Connection c;
    QSqlQuery query;
    if (c.createconnect())
    {
        return c.insertData(nom,budget,pack,date_deb,date_fin,telephone);
    }
    return false;
}

void Sponsor::afficher(QTableWidget *tableWidget)

{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A4");
    db.setUserName("amine");
    db.setPassword("amine");
    db.open();

    QSqlQuery query(db);
    QString str=("SELECT * FROM SPONSOR");
    if (query.exec(str))
    {
        tableWidget->setColumnCount(6);
        QStringList labels;
        labels<<"Nom"<<"Budget"<<"Pack"<<"Date debut"<<"Date fin"<<"Telephone";
        tableWidget->setHorizontalHeaderLabels(labels);
        int RowNumber=0;
        while(query.next())
        {
            tableWidget->insertRow(RowNumber);
            QTableWidgetItem *nom= new QTableWidgetItem;
            QTableWidgetItem *budget= new QTableWidgetItem;
            QTableWidgetItem *pack= new QTableWidgetItem;
            QTableWidgetItem *date_deb= new QTableWidgetItem;
            QTableWidgetItem *date_fin= new QTableWidgetItem;
            QTableWidgetItem *telephone= new QTableWidgetItem;

            nom->setText(query.value(0).toString());
            budget->setText(query.value(1).toString());
            pack->setText(query.value(2).toString());
            date_deb->setText(query.value(3).toString());
            date_fin->setText(query.value(4).toString());
            telephone->setText(query.value(5).toString());

            tableWidget->setItem(RowNumber,0,nom);
            tableWidget->setItem(RowNumber,1,budget);
            tableWidget->setItem(RowNumber,2,pack);
            tableWidget->setItem(RowNumber,3,date_deb);
            tableWidget->setItem(RowNumber,4,date_fin);
            tableWidget->setItem(RowNumber,5,telephone);
            RowNumber++;
        }
    }
    db.close();
}

bool Sponsor::supprimer(int id)
{
    QSqlQuery query;
    QString res = QString::number(id);
       query.prepare("DELETE FROM SPONSOR WHERE ID_SPONSOR = :id");
       query.bindValue(":id", res);

       if (query.exec()) {
           return true;
       } else {
           qDebug() << "Error deleting data:" << query.lastError().text();
           return false;
       }
}

bool Sponsor::modifier(int id_sponsor, const QString &nom, const QString &budget, const QString &pack, const QString &date_deb, const QString &date_fin, const QString &telephone)
{
    QSqlQuery query;
    query.prepare("UPDATE SPONSOR SET NOM = :nom, BUDGET = :budget, PACK = :pack, DATE_DEB = :date_deb, DATE_FIN = :date_fin, TELEPHONE = :telephone WHERE ID_SPONSOR = :id_sponsor");
    query.bindValue(":nom", nom);
    query.bindValue(":budget", budget);
    query.bindValue(":pack", pack);
    query.bindValue(":date_deb", date_deb);
    query.bindValue(":date_fin", date_fin);
    query.bindValue(":telephone", telephone);

    return query.exec();
}


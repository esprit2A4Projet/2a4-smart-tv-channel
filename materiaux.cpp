#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "connection.h"
#include "Materiaux.h"

//---------------------------------------------------les constructeurs---------------------------------------------------//
Materiaux::Materiaux()
{
    id=0;
    nom=""; //datetransaction
    type=""; //montant
    etat=""; //modeDePaiement
    quantite="";//categorie
    date="";//type
}

Materiaux::Materiaux(int id, QString nom, QString type, QString etat, QString quantite, QString date)
{
    this->id=id;
    this->nom=nom;
    this->type=type;
    this->etat=etat;
    this->quantite=quantite;
    this->date=date;
}
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//






//---------------------------------------------------fonction d'ajout--------------------------------------------------------//
bool Materiaux::ajouterMateriaux()
{
    Connection c;
    if (c.createconnect())
    {
        return c.insertDataM(nom, type, etat, quantite, date);

    }
    return false;
}

//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//







//---------------------------------------------------fonction d'affichage---------------------------------------------------------//
void Materiaux::afficherMateriaux(QTableWidget *tableWidget)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("Source_Projet2A4");
        db.setUserName("amine");
        db.setPassword("amine");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM MATERIAUX");
        if (query.exec(str))
        {
            tableWidget->setColumnCount(6);
            QStringList labelsM;
            labelsM<<"ID"<<"Nom"<<"Type"<<"Etat"<<"Quantite"<<"Date";
            tableWidget->setHorizontalHeaderLabels(labelsM);
            int RowNumber=0;
            while(query.next())
            {
                tableWidget->insertRow(RowNumber);
                QTableWidgetItem *id= new QTableWidgetItem;
                QTableWidgetItem *nom= new QTableWidgetItem;
                QTableWidgetItem *type= new QTableWidgetItem;
                QTableWidgetItem *etat= new QTableWidgetItem;
                QTableWidgetItem *quantite= new QTableWidgetItem;
                QTableWidgetItem *date= new QTableWidgetItem;

                id->setText(query.value(0).toString());
                nom->setText(query.value(1).toString());
                type->setText(query.value(2).toString());
                etat->setText(query.value(3).toString());
                quantite->setText(query.value(4).toString());
                date->setText(query.value(5).toString());

                tableWidget->setItem(RowNumber,0,id);
                tableWidget->setItem(RowNumber,1,nom);
                tableWidget->setItem(RowNumber,2,type);
                tableWidget->setItem(RowNumber,3,etat);
                tableWidget->setItem(RowNumber,4,quantite);
                tableWidget->setItem(RowNumber,5,date);
                RowNumber++;
            }
        }
        db.close();
}
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//







//---------------------------------------------------fonction de suppression----------------------------------------------------//
bool Materiaux::supprimerMateriaux(int id)
{
        QSqlQuery query;
        query.prepare("DELETE FROM MATERIAUX WHERE ID_MATERIEL = :id");
        query.bindValue(":id", id);

        if (query.exec()) return true;
        else
        {
            qDebug() << "Error deleting data:" << query.lastError().text();
            return false;
        }
}
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//









//---------------------------------------------------fonction de modification----------------------------------------------------//
bool Materiaux::modifierMateriaux(int id, const QString& nom, const QString& type, const QString& etat, const QString& quantite, const QString& date)
{
    QSqlQuery query;

        query.prepare("UPDATE MATERIAUX SET NOM = :nom, TYPE = :type, ETAT = :etat, QUANTITE = :quantite, DATE_MISE = :date WHERE ID_MATERIEL = :id");
        query.bindValue(":id", id);
        query.bindValue(":nom", nom);
        query.bindValue(":type", type);
        query.bindValue(":etat", etat);
        query.bindValue(":quantite", quantite);
        query.bindValue(":date", date);

        if (!query.exec())
        {
            qDebug() << "Erreur de mise à jour du materiel : " << query.lastError().text();
            return false;
        }

        return true;
}
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//



//setters
void Materiaux::set_nom(QString n){nom=n;}
void Materiaux::set_type(QString n){type=n;}
void Materiaux::set_etat(QString n){etat=n;}
void Materiaux::set_quantite(QString n){quantite=n;}
void Materiaux::set_date(QString n){date=n;}


//getters
QString Materiaux::get_nom(){return nom;}
QString Materiaux::get_type(){return type;}
QString Materiaux::get_etat(){return etat;}
QString Materiaux::get_quantite(){return quantite;}
QString Materiaux::get_date(){return date;}

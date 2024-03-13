#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "connection.h"
#include "transaction.h"
#include <QDateTime>


//---------------------------------------------------les constructeurs---------------------------------------------------//
Transaction::Transaction()
{
    id=0;
    dateTransaction="";
    montant="";
    modeDePaiement="";
    categorie="";
    type="";
    categorie="";
    dateTransaction="";
    montant="";
}

Transaction::Transaction(int id, QString modeDePaiement, QString type, QString categorie, QString dateTransaction, QString montant)
{
    this->id=id;
    this->modeDePaiement=modeDePaiement;
    this->type=type;
    this->categorie=categorie;
    this->dateTransaction=dateTransaction;
    this->montant=montant;
}
//------------------------------------------------------------------------------------------------------------------------//





//---------------------------------------------------fonction d'ajout--------------------------------------------------------//
bool Transaction::ajouterTransaction()
{


    Connection c;

        return c.insertDataT(modeDePaiement,type, categorie, dateTransaction, montant);


}

//---------------------------------------------------------------------------------------------------------------------------//






//---------------------------------------------------fonction d'affichage---------------------------------------------------------//
void Transaction::afficherTransaction(QTableWidget *table_de_transactions)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("Source_Projet2A4");
        db.setUserName("feriel");
        db.setPassword("feriel");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM TRANSACTIONS");
        if (query.exec(str))
        {
            table_de_transactions->setColumnCount(6);
            QStringList labelsT;
            labelsT<<"Mode de paiement"<<"Type"<<"Categorie"<<"Date de transaction"<<"Montant"<<"ID";
            table_de_transactions->setHorizontalHeaderLabels(labelsT);
            int RowNumber=0;
            while(query.next())
            {
                table_de_transactions->insertRow(RowNumber);
                QTableWidgetItem *modeDepaiement= new QTableWidgetItem;
                QTableWidgetItem *type= new QTableWidgetItem;
                QTableWidgetItem *categorie= new QTableWidgetItem;
                QTableWidgetItem *dateTransaction= new QTableWidgetItem;
                QTableWidgetItem *montant= new QTableWidgetItem;
                QTableWidgetItem *id= new QTableWidgetItem;

                modeDepaiement->setText(query.value(0).toString());
                type->setText(query.value(1).toString());
                categorie->setText(query.value(2).toString());
                dateTransaction->setText(query.value(3).toString());
                montant->setText(query.value(4).toString());
                id->setText(query.value(5).toString());

                table_de_transactions->setItem(RowNumber,0,modeDepaiement);
                table_de_transactions->setItem(RowNumber,1,type);
                table_de_transactions->setItem(RowNumber,2,categorie);
                table_de_transactions->setItem(RowNumber,3,dateTransaction);
                table_de_transactions->setItem(RowNumber,4,montant);
                table_de_transactions->setItem(RowNumber,5,id);
                RowNumber++;
            }
        }
        db.close();
}
//-------------------------------------------------------------------------------------------------------------------------------//






//---------------------------------------------------fonction de suppression----------------------------------------------------//
bool Transaction::supprimerTransaction(int id)
{
        QSqlQuery query;
        query.prepare("DELETE FROM TRANSACTIONS WHERE ID_TRANSACTION = :id");
        query.bindValue(":id", id);

        if (query.exec()) return true;
        else
        {
            qDebug() << "Error deleting data:" << query.lastError().text();
            return false;
        }
}
//-----------------------------------------------------------------------------------------------------------------------------//








//---------------------------------------------------fonction de modification----------------------------------------------------//
bool Transaction::modifierTransaction(int id, const QString& modeDePaiement, const QString& type, const QString& categorie, const QString& dateTransaction, const QString& montant)
{
    QSqlQuery query;

        query.prepare("UPDATE TRANSACTIONS SET MODEDEPAIEMENT = :modeDePaiement, TYPE = :type, CATEGORIE = :categorie, DATETRANSACTION = :dateTransaction, MONTANT = :montant WHERE ID_TRANSACTION = :id");
        query.bindValue(":modeDePaiement", modeDePaiement);
        query.bindValue(":type", type);
        query.bindValue(":categorie", categorie);
        query.bindValue(":dateTransaction", dateTransaction);
        query.bindValue(":montant", montant);
        query.bindValue(":id", id);

        if (!query.exec())
        {
            qDebug() << "Erreur de mise à jour de la transaction : " << query.lastError().text();
            return false;
        }

        return true;
}
//------------------------------------------------------------------------------------------------------------------------------//


//setters
void Transaction::set_modeDePaiement(QString n){modeDePaiement=n;}
void Transaction::set_type(QString n){type=n;}
void Transaction::set_categorie(QString n){categorie=n;}
void Transaction::set_dateTransaction(QString n){dateTransaction=n;}
void Transaction::set_montant(QString n){montant=n;}


//getters
QString Transaction::get_modeDePaiement(){return modeDePaiement;}
QString Transaction::get_type(){return type;}
QString Transaction::get_categorie(){return categorie;}
QString Transaction::get_dateTransaction(){return dateTransaction;}
QString Transaction::get_montant(){return montant;}




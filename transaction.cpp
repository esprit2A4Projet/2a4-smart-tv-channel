#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "connexion.h"
#include "transaction.h"
#include <QDateTime>
#include <QtAV/QtAV>
#include <QtAVWidgets/QtAVWidgets>

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
    if (c.createconnect())
    {
        //-----------------------------------------------SAISIR DANS LA TABLE NOTIFICATIONS APRES L'AJOUT----------------------------------------------------//
        QString messageDeNotification;
        QString tempsDeNotification = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"); // Obtenez l'heure système
        if (type == "Depense")
                {
                    messageDeNotification = "Vous avez depense " + montant + " dinars apres la Transaction faite le : " + dateTransaction + " et ajoutee le : " + tempsDeNotification;
                }
                else if (type == "Revenue")
                {
                    messageDeNotification = "Vous avez recu " + montant + " dinars apres la Transaction faite le : " + dateTransaction + " et ajoutee le : " + tempsDeNotification;
                }
                QString insertQuery = "INSERT INTO NOTIFICATIONS_TRANSACTION (MESSAGE_NOTIFICATION, TEMPS_NOTIFICATION) VALUES (:message, :temps)";
                QSqlQuery query;
                query.prepare(insertQuery);
                query.bindValue(":message", messageDeNotification);
                query.bindValue(":temps", tempsDeNotification);
                query.exec();
        QString messageNotification;
        QString tempsNotification = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"); // Obtenez l'heure système

        // Calculer la somme des dépenses
        QString sommeDepensesQuery = "SELECT SUM(montant) FROM TRANSACTIONS WHERE type = 'Depense'";
        QSqlQuery depensesQuery;
        if (!depensesQuery.exec(sommeDepensesQuery)) {
            qDebug() << "Erreur lors du calcul de la somme des dépenses:" << depensesQuery.lastError().text();
            return false ; // Quittez la fonction en cas d'erreur
        }

        depensesQuery.next();
        double sommeDepenses = depensesQuery.value(0).toDouble();

        // Calculer la somme des revenus
        QString sommeRevenusQuery = "SELECT SUM(montant) FROM TRANSACTIONS WHERE type = 'Revenue'";
        QSqlQuery revenusQuery;
        if (!revenusQuery.exec(sommeRevenusQuery)) {
            qDebug() << "Erreur lors du calcul de la somme des revenus:" << revenusQuery.lastError().text();
            return false; // Quittez la fonction en cas d'erreur
        }

        revenusQuery.next();
        double sommeRevenus = revenusQuery.value(0).toDouble();

        if (sommeDepenses > sommeRevenus) {
            double difference = sommeDepenses - sommeRevenus;
            messageNotification = "Les depense ont depasse les revenus de " + QString::number(difference) + " le " + tempsNotification;

            // Insérer la notification dans la base de données
            QString insertQuery = "INSERT INTO NOTIFICATIONS_TRANSACTION (MESSAGE_NOTIFICATION, TEMPS_NOTIFICATION) VALUES (:message, :temps)";
            QSqlQuery query;
            query.prepare(insertQuery);
            query.bindValue(":message", messageNotification);
            query.bindValue(":temps", tempsNotification);
            if (!query.exec()) {
                qDebug() << "Erreur lors de l'insertion de la notification:" << query.lastError().text();
                return false ; // Quittez la fonction en cas d'erreur
            } else {
                qDebug() << "Notification insérée avec succès";
            }
        } else {
            // Autre logique si les revenus dépassent les dépenses
        }

        //---------------------------------------------------------------------------------------------------------------------------------------------------//
        return c.insertData(modeDePaiement, type, categorie, dateTransaction, montant);
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------//






//---------------------------------------------------fonction d'affichage---------------------------------------------------------//
void Transaction::afficherTransaction(QTableWidget *tableWidget)
{
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("Source_Projet2A4");
        db.setUserName("louayzorai");
        db.setPassword("louayzorai");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM TRANSACTIONS");
        if (query.exec(str))
        {
            tableWidget->setColumnCount(6);
            QStringList labels;
            labels<<"Mode de paiement"<<"Type"<<"Categorie"<<"Date de transaction"<<"Montant"<<"ID";
            tableWidget->setHorizontalHeaderLabels(labels);
            int RowNumber=0;
            while(query.next())
            {
                tableWidget->insertRow(RowNumber);
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

                tableWidget->setItem(RowNumber,0,modeDepaiement);
                tableWidget->setItem(RowNumber,1,type);
                tableWidget->setItem(RowNumber,2,categorie);
                tableWidget->setItem(RowNumber,3,dateTransaction);
                tableWidget->setItem(RowNumber,4,montant);
                tableWidget->setItem(RowNumber,5,id);
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




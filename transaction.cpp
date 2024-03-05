#include "transaction.h"
#include <QTableWidget>
#include <QDebug>
#include <QSqlQuery>
#include "connexion.h"

//---------------------------------------------------les constructeurs---------------------------------------------------//
Transaction::Transaction()
{
    modeDePaiement="";
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
        db.setUserName("system");
        db.setPassword("Islemsallouma1");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM TRANSACTION");
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
        query.prepare("DELETE FROM TRANSACTION WHERE ID = :id");
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

        query.prepare("UPDATE TRANSACTION SET MODEDEPAIEMENT = :modeDePaiement, TYPE = :type, CATEGORIE = :categorie, DATETRANSACTION = :dateTransaction, MONTANT = :montant WHERE ID = :id");
        query.bindValue(":modeDePaiement", modeDePaiement);
        query.bindValue(":type", type);
        query.bindValue(":categorie", categorie);
        query.bindValue(":dateTransaction", dateTransaction);
        query.bindValue(":montant", montant);
        query.bindValue(":id", id);

        if (!query.exec()) {
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



//--------------------les fonctions pour les controle de saisie-------------------------//
bool Transaction::montantIsNumeric()
{
    // Vérifie si le montant est composé uniquement de chiffres
        for (const QChar &ch : montant) {
            if (!ch.isDigit()) {
                return false; // S'il y a un caractère qui n'est pas un chiffre, renvoie faux
            }
        }
        return true; // Si tous les caractères sont des chiffres, renvoie vrai
}
bool Transaction::isValidModeDePaiement()
{
    // Vérifie si le mode de paiement est valide
    QStringList validModesDePaiement{"TPE", "Cheques", "Especes"};
    return validModesDePaiement.contains(modeDePaiement);
}
bool Transaction::isValidDateTransaction()
{
    // Vérifie si la date de transaction est du format mm/jj/aaaa
    // Exemple de vérification simplifié pour la démonstration
    return dateTransaction.length() == 10 && dateTransaction.count('/') == 2;
}
bool Transaction::isValidCategorie()
{
    // Vérifie si la catégorie est valide
    QStringList validCategories{"Paiement employe", "Paiement facture", "Achat materiel", "Vente materiel", "Location local"};
    return validCategories.contains(categorie);
}
bool Transaction::isValidType()
{
    // Vérifie si le type est valide
    QStringList validTypes{"Revenue", "Depense"};
    return validTypes.contains(type);
}

#include "employee.h"
#include "connection.h"
#include <QDebug>
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextDocument>
Employee::Employee()
{
id=0;
nom="";
prenom="";
date_embauche="";
salaire=0;
poste="";

}

Employee::Employee(QString a,QString b,QString c,int d,QString e)
{
    nom=a;
    prenom=b;
    date_embauche=c;
    salaire=d;
    poste=e;

}

void Employee::setnom(QString n){nom=n;}
void Employee::setprenom(QString n){prenom=n;}
void Employee::setdate_embauche(QString n){date_embauche=n;}
void Employee::setsalaire(int n){salaire=n;}
void Employee::setposte(QString n){poste=n;}


int Employee::get_id(){return id;}
QString Employee::get_nom(){return nom;}
QString Employee::get_prenom(){return prenom;}
QString Employee::get_date_embauche(){return date_embauche;}
int Employee::get_salaire(){return salaire;}
QString Employee::get_poste(){return poste;}


bool Employee::ajouter()
{
       QSqlQuery query;

             query.prepare("INSERT INTO EMPLOYES (nom, prenom, date_embauche,salaire,poste) "
                        "VALUES (:nom, :prenom, :date_embauche,:salaire,:poste)");
             query.bindValue(0,nom);
             query.bindValue(1,prenom);
             query.bindValue(2,date_embauche);
             query.bindValue(3,salaire);
             query.bindValue(4,poste);

        return query.exec();
}


QSqlQueryModel* Employee::afficher()
{
   QSqlQueryModel* model=new QSqlQueryModel();

         model->setQuery("SELECT* FROM EMPLOYES");
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
         model->setHeaderData(1, Qt::Horizontal, QObject:: tr("Nom"));
         model->setHeaderData(2, Qt::Horizontal, QObject:: tr("Prénom"));
         model->setHeaderData(3, Qt::Horizontal, QObject:: tr("Date d'embauche"));
         model->setHeaderData(4, Qt::Horizontal, QObject:: tr("Salaire"));
         model->setHeaderData(5, Qt::Horizontal, QObject:: tr("Poste"));

 return model;
}


bool Employee::supprimer(int id)
{

    QSqlQuery query;
           query.prepare("Delete from EMPLOYES where id=:id");
           query.bindValue(0, id);
            return query.exec();
}

bool Employee::update(int id)
{
    QSqlQuery query;
    // Exécuter la mise à jour avec les nouvelles valeurs des attributs de l'employé
    query.prepare("UPDATE EMPLOYES SET NOM=:nom, PRENOM=:prenom, date_embauche=:date_embauche, Salaire=:salaire, poste=:poste WHERE id=:id");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":salaire", salaire);
    query.bindValue(":poste", poste);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "Update operation failed.";
        return false; // Return false if the update operation fails
    }
else
    {
    return true; // Return true if the update operation is successful
    }
}









QSqlQueryModel* Employee::Rechercher(int id)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;

    // Préparer la requête SQL avec un paramètre lié à l'ID
    query.prepare("SELECT * FROM EMPLOYES WHERE id = :id");
    query.bindValue(":id", id); // Lier la valeur de l'ID à la requête

    // Exécuter la requête et vérifier si elle a réussi
    if (query.exec()) {
        // Si la requête réussit, associer le modèle à la requête
        model->setQuery(query);
    } else {
        // Si la requête échoue, afficher un message d'erreur et détruire le modèle
        qDebug() << "Erreur lors de l'exécution de la requête SQL:" << query.lastError().text();
        delete model;
        model = nullptr;
    }

    return model;
}



QSqlQueryModel* Employee::tri()
{
   QSqlQueryModel * model=new QSqlQueryModel();
   model->setQuery("SELECT * FROM EMPLOYES ORDER BY prenom ASC ");

   model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
   model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
   model->setHeaderData(2,Qt::Horizontal,QObject::tr("prenom"));
   model->setHeaderData(3,Qt::Horizontal,QObject::tr("date d'embauche"));
   model->setHeaderData(4,Qt::Horizontal,QObject::tr("salaire"));
   model->setHeaderData(5,Qt::Horizontal,QObject::tr("poste"));


   return  model;

}



void Employee::genererPDFact()
{



}

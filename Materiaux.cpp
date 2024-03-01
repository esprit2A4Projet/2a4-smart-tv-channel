#include "Materiaux.h"

Materiaux::Materiaux()
{
    nom="";
    type="";
    etat="";
    quantite="";
    date="";

}
Materiaux::Materiaux(QString nom,QString type,QString etat,QString quantite,QString date)
{
    //this->id=id;
    this->nom=nom;
    this->type=type;
    this->etat=etat;
    this->quantite=quantite;
    this->date=date;
}
void Materiaux::setnom(QString n){nom=n;}
void Materiaux::settype(QString n){type=n;}
void Materiaux::setetat(QString n){etat=n;}
void Materiaux::setquantite(QString n){quantite=n;}
void Materiaux::setdate(QString n){date=n;}


QString Materiaux::get_nom(){return nom;}
QString Materiaux::get_type(){return type;}
QString Materiaux::get_etat(){return etat;}
QString Materiaux::get_quantite(){return quantite;}
QString Materiaux::get_date(){return date;}

bool Materiaux::ajouter()
{
    QSqlQuery query;
    //QString res = QString::number(id);
    query.prepare("insert into etudiant (id,nom,type,etat,quantite,date)" "values (:nom, :type, :etat, :quantite, :date)");
   // query.bindValue(":id",id);
    query.bindValue(":nom",nom);
    query.bindValue("type",type);
    query.bindValue("etat",etat);
    query.bindValue("quantite",quantite);
    query.bindValue("date",date);

    return query.exec();
}
bool Materiaux::supprimer(const QString nom)
{
    QSqlQuery query;
    QString res=nom;
    query.prepare("Delete from materiaux where ID= :nom");
    query.bindValue(":nom",res);
    return query.exec();
}

QSqlQueryModel* Materiaux::afficher()
{
   QSqlQueryModel* model=new QSqlQueryModel();

         model->setQuery("SELECT* FROM MATERIAUX");
         model->setHeaderData(0, Qt::Horizontal, QObject:: tr("nom"));
         model->setHeaderData(1, Qt::Horizontal, QObject:: tr("type"));
         model->setHeaderData(2, Qt::Horizontal, QObject:: tr("etat"));
         model->setHeaderData(3, Qt::Horizontal, QObject:: tr("quantite"));
         model->setHeaderData(4, Qt::Horizontal, QObject:: tr("date"));
 return model;
}

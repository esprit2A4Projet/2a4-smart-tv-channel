#include "Materiaux.h"
#include <QDebug>
#include <QSqlError>



Materiaux::Materiaux()
{
    id=0;
    nom="";
    type="";
    etat="";
    quantite="";
    date="";

}
Materiaux::Materiaux(int id,QString nom,QString type,QString etat,QString quantite,QString date)
{
    this->id=id;
    this->nom=nom;
    this->type=type;
    this->etat=etat;
    this->quantite=quantite;
    this->date=date;
}
void Materiaux::setid(int n) { id = n; }
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
int Materiaux::get_id() { return id; }

bool Materiaux::ajouter()
{
    QSqlQuery query;
    //QString res = QString::number(id);
    query.prepare("insert into Materiaux (id,nom,type,etat,quantite,date)" "values (:nom, :type, :etat, :quantite, :date)");
    query.bindValue(":id",id);
    query.bindValue(":nom",nom);
    query.bindValue("type",type);
    query.bindValue("etat",etat);
    query.bindValue("quantite",quantite);
    query.bindValue("date",date);

    return query.exec();
}
/*bool Materiaux::supprimer(int id)
{
    QSqlQuery query;
    //QString res=id;
    query.prepare("Delete from materiaux where ID_MATERIEL= :id");
    query.bindValue(":id",id);
    return query.exec();
}*/

QSqlQueryModel* Materiaux::afficher()
{
   QSqlQueryModel* model=new QSqlQueryModel();

         model->setQuery("SELECT* FROM MATERIAUX");
         model->setHeaderData(0, Qt::Horizontal, QObject:: tr("id"));
         model->setHeaderData(1, Qt::Horizontal, QObject:: tr("nom"));
         model->setHeaderData(2, Qt::Horizontal, QObject:: tr("type"));
         model->setHeaderData(3, Qt::Horizontal, QObject:: tr("etat"));
         model->setHeaderData(4, Qt::Horizontal, QObject:: tr("quantite"));
         model->setHeaderData(5, Qt::Horizontal, QObject:: tr("date"));
 return model;
}

bool Materiaux::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM MATERIAUX WHERE ID_MATERIEL = :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        qDebug() << "Suppression réussie.";
        return true;
    } else {
        qDebug() << "Échec de la suppression :" << query.lastError().text();
        return false;
    }
}

/*bool Materiaux::update(int id) {
    QSqlQuery query;
    query.prepare("UPDATE MATERIAUX SET NOM = :nom, TYPE = :type, ETAT = :etat, QUANTITE = :quantite, DATE_MISE = :date WHERE ID_MATERIEL = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    query.bindValue(":quantite", quantite);
    query.bindValue(":date", date);

    if (query.exec()) {
        qDebug() << "Mise à jour réussie.";
        return true;
    } else {
        qDebug() << "Échec de la mise à jour :" << query.lastError().text();
        return false;
    }
}*/

bool Materiaux::update(int id, const QString& nom, const QString& type, const QString& etat, const QString& quantite, const QString& date)
{
    QSqlQuery query;
    query.prepare("UPDATE MATERIAUX SET NOM = :nom, TYPE = :type, ETAT = :etat, QUANTITE = :quantite, DATE_MISE = :date WHERE ID_MATERIEL = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    query.bindValue(":quantite", quantite);
    query.bindValue(":date", date);

    if (query.exec()) {
        return true; // La modification a réussi
    } else {
        qDebug() << "Erreur de mise à jour du materiel:" << query.lastError().text();
        return false; // La modification a échoué
    }
}


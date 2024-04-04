#include "podcast.h"
#include "connection.h"
#include <QDebug>
#include <QObject>
#include <QSqlQueryModel>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextEdit>

podcast::podcast()
{
    id=0;
    nom="";
    duree="";
    lieu="";
    categorie="";
    date_pod="";

}

podcast::podcast(QString a,QString b,QString c,QString d,QString e)
{
    nom=a;
    duree=b;
    lieu=c;
    categorie=d;
    date_pod=e;

}

void podcast::setnom(QString n){nom=n;}
void podcast::setduree(QString n){duree=n;}
void podcast::setlieu(QString n){lieu=n;}
void podcast::setcategorie(QString n){categorie=n;}
void podcast::setdate_pod(QString n){date_pod=n;}


int podcast::get_id(){return id;}
QString podcast::get_nom(){return nom;}
QString podcast::get_duree(){return duree;}
QString podcast::get_lieu(){return lieu;}
QString podcast::get_categorie(){return categorie;}
QString podcast::get_date_pod(){return date_pod;}


bool podcast::ajouterp(){
       QSqlQuery query;

             query.prepare("INSERT INTO PODCASTS (nom, duree, lieu,categorie,date_pod) "
                        "VALUES (:nom, :duree, :lieu,:categorie,:date_pod)");
             query.bindValue(0,nom);
             query.bindValue(1,duree);
             query.bindValue(2,lieu);
             query.bindValue(3,categorie);
             query.bindValue(4,date_pod);

        return query.exec();
    }


QSqlQueryModel* podcast::afficherp(){
   QSqlQueryModel* model=new QSqlQueryModel();

         model->setQuery("SELECT* FROM PODCASTS");
         model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
         model->setHeaderData(1,Qt::Horizontal,QObject::tr("Nom"));
         model->setHeaderData(2,Qt::Horizontal,QObject::tr("Durée min"));
         model->setHeaderData(3,Qt::Horizontal,QObject::tr("Lieu"));
         model->setHeaderData(4,Qt::Horizontal,QObject::tr("Categorie"));
         model->setHeaderData(5,Qt::Horizontal,QObject::tr("Date du Podcast"));


 return model;
}
bool podcast::nomExists(const QString& nom) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM podcasts WHERE nom = :nom");
    query.bindValue(":nom", nom);
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }
    return false;
}


bool podcast::supprimerp(QString nom)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PODCASTS WHERE nom = ?");
    query.addBindValue(nom);
    return query.exec();
}


/*
bool podcast::supprimerp(int id)
{

    QSqlQuery query;
           query.prepare("Delete from PODCASTS where nom=:nom");
           query.bindValue(0, id);
            return query.exec();
}
*/




bool podcast::update(int, const QString& newNom, const QString& duree, const QString& lieu, const QString& categorie, const QString& date_pod)
{
    QSqlQuery query;
    query.prepare("UPDATE PODCASTS SET nom = :newNom, duree = :duree, lieu = :lieu, categorie = :categorie, date_pod = :date_pod WHERE nom = :nom");
    query.bindValue(":newNom", newNom);
    query.bindValue(":duree", duree);
    query.bindValue(":lieu", lieu);
    query.bindValue(":categorie", categorie);
    query.bindValue(":date_pod", date_pod);
    query.bindValue(":nom", nom);

    if (query.exec()) {
        return true; // La modification a réussi
    } else {
        qDebug() << "Erreur de mise à jour du podcast:" << query.lastError().text();
        return false; // La modification a échoué
    }
}







QSqlQueryModel* podcast::Rechercherp(const QString& nom)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;

    // Préparer la requête SQL avec un paramètre lié au nom
    query.prepare("SELECT * FROM PODCASTS WHERE nom_podcast = :nom");
    query.bindValue(":nom", nom); // Lier la valeur du nom à la requête

    // Exécuter la requête et vérifier si elle a réussi
    if (query.exec()) {
        // Si la requête réussit, associer le modèle à la requête
        model->setQuery(query);
        if (model->rowCount() == 0) {
            qDebug() << "Aucun résultat trouvé pour le podcast avec le nom" << nom;
        }
    } else {
        // Si la requête échoue, afficher un message d'erreur et détruire le modèle
        qDebug() << "Erreur lors de l'exécution de la requête SQL:" << query.lastError().text();
        delete model;
        model = nullptr;
    }

    return model;
}


QSqlQueryModel* podcast::trip()
{
   QSqlQueryModel * model=new QSqlQueryModel();
   model->setQuery("SELECT * FROM PODCASTS ORDER BY date_pod ASC ");

   model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
   model->setHeaderData(1,Qt::Horizontal,QObject::tr("Nom"));
   model->setHeaderData(2,Qt::Horizontal,QObject::tr("Durée min"));
   model->setHeaderData(3,Qt::Horizontal,QObject::tr("Lieu"));
   model->setHeaderData(4,Qt::Horizontal,QObject::tr("Categorie"));
   model->setHeaderData(5,Qt::Horizontal,QObject::tr("Date du Podcast"));


   return  model;

}

QSqlQueryModel* podcast::tripDesc()
{
   QSqlQueryModel * model=new QSqlQueryModel();
   model->setQuery("SELECT * FROM PODCASTS ORDER BY date_pod DESC ");

   model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
   model->setHeaderData(1,Qt::Horizontal,QObject::tr("Nom"));
   model->setHeaderData(2,Qt::Horizontal,QObject::tr("Durée min"));
   model->setHeaderData(3,Qt::Horizontal,QObject::tr("Lieu"));
   model->setHeaderData(4,Qt::Horizontal,QObject::tr("Categorie"));
   model->setHeaderData(5,Qt::Horizontal,QObject::tr("Date du Podcast"));



   return  model;

}


void podcast::afficherHistorique() {
    QFile historyFile("C:/Users/ISMAIL/Desktop/crud/Historique.txt");
    if (historyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&historyFile);
        QString historiqueContenu = in.readAll();
        historyFile.close();

        // Displaying the history in a dialog
        QDialog* historiqueDialog = new QDialog(nullptr);
        historiqueDialog->setWindowTitle("Historique");
        historiqueDialog->setMinimumSize(400, 300);

        QTextEdit* textEdit = new QTextEdit(historiqueDialog);
        textEdit->setPlainText(historiqueContenu);
        textEdit->setReadOnly(true);

        QVBoxLayout* layout = new QVBoxLayout(historiqueDialog);
        layout->addWidget(textEdit);

        historiqueDialog->exec();
    } else {
        QMessageBox::warning(nullptr, "Erreur", "Impossible d'ouvrir le fichier d'historique pour la lecture.");
    }
}

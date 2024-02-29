#include "invite.h"
#include <QTableWidget>
#include <QDebug>
#include "connection.h"


Invite::Invite()
{
    nom="";
    prenom="";
    profession="";
    email="";
    nbAbonnes="";
    nbParticipation="";

}

Invite::Invite(int id, QString nom, QString prenom, QString profession, QString email, QString nbAbonnes, QString nbParticipation)
{
    this->id=id;
    this->nom=nom;
    this->prenom=prenom;
    this->profession=profession;
    this->email=email;
    this->nbAbonnes=nbAbonnes;
    this->nbParticipation=nbParticipation;
}

bool Invite::ajouterInvite()
{
    Connection c;
    if (c.createconnect())
    {
        return c.insertData(nom, prenom, profession, email, nbAbonnes, nbParticipation);
    }
    return false;
}

void Invite::afficherInvite(QTableWidget *tableWidget)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet");
    db.setUserName("system");
    db.setPassword("09649831");
    db.open();

    QSqlQuery query(db);
    QString str=("SELECT * FROM INVITE");
    if (query.exec(str))
    {
        tableWidget->setColumnCount(7);
        QStringList labels;
        labels<<"ID"<<"Nom"<<"Prenom"<<"Profession"<<"Email"<<"Nombre d'abonnes"<<"Nombre de Participation";
        tableWidget->setHorizontalHeaderLabels(labels);
        int RowNumber=0;
        while(query.next())
        {
            tableWidget->insertRow(RowNumber);
            QTableWidgetItem *id= new QTableWidgetItem;
            QTableWidgetItem *nom= new QTableWidgetItem;
            QTableWidgetItem *prenom= new QTableWidgetItem;
            QTableWidgetItem *profession= new QTableWidgetItem;
            QTableWidgetItem *email= new QTableWidgetItem;
            QTableWidgetItem *nbAbonnes= new QTableWidgetItem;
            QTableWidgetItem *nbParticipation= new QTableWidgetItem;

            id->setText(query.value(0).toString());
            nom->setText(query.value(1).toString());
            prenom->setText(query.value(2).toString());
            profession->setText(query.value(3).toString());
            email->setText(query.value(4).toString());
            nbAbonnes->setText(query.value(5).toString());
            nbParticipation->setText(query.value(6).toString());

            tableWidget->setItem(RowNumber,0,id);
            tableWidget->setItem(RowNumber,1,nom);
            tableWidget->setItem(RowNumber,2,prenom);
            tableWidget->setItem(RowNumber,3,profession);
            tableWidget->setItem(RowNumber,4,email);
            tableWidget->setItem(RowNumber,5,nbAbonnes);
            tableWidget->setItem(RowNumber,6,nbParticipation);
            RowNumber++;
        }
    }
    db.close();
}


bool Invite::supprimerInvite(int id)
{
    QSqlQuery query;
    QString res = QString::number(id);
    query.prepare("DELETE FROM INVITE WHERE ID = :id");
    query.bindValue(":id", res);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error deleting data:" << query.lastError().text();
        return false;
    }
}
bool Invite::modifierInvite(int id, const QString &nom, const QString &prenom, const QString &profession, const QString &email, const QString &nbAbonnes, const QString &nbParticipation)
{
    QSqlQuery query;
    query.prepare("UPDATE INVITE SET NOM = :nom, PRENOM = :prenom, PROFESSION = :profession, EMAIL = :email, NBABONNES = :nbAbonnes, NBPARTICIPATION = :nbParticipation WHERE ID = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":profession", profession);
    query.bindValue(":email", email);
    query.bindValue(":nbAbonnes", nbAbonnes);
    query.bindValue(":nbParticipation", nbParticipation);
    query.bindValue(":id", id);

    return query.exec();
}


void Invite::setnom(QString n){nom=n;}
void Invite::setprenom(QString n){prenom=n;}
void Invite::setprofession(QString n){profession=n;}
void Invite::setemail(QString n){email=n;}
void Invite::setnbAbonnes(QString n){nbAbonnes=n;}
void Invite::setnbParticipation(QString n){nbParticipation=n;}


QString Invite::get_nom(){return nom;}
QString Invite::get_prenom(){return prenom;}
QString Invite::get_profession(){return profession;}
QString Invite::get_email(){return email;}
QString Invite::get_nbAbonnes(){return nbAbonnes;}
QString Invite::get_nbParticipation(){return nbParticipation;}






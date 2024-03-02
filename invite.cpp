#include "invite.h"
#include <QTableWidget>
#include <QDebug>
#include "connection.h"
#include <QMessageBox>


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
    return c.createconnect() && c.insertData(nom, prenom, profession, email, nbAbonnes, nbParticipation);
}


void Invite::afficherInvite(QTableWidget *tableWidget)
{
    Connection c;
    if (!c.createconnect()) return;

    QSqlQuery query(c.db);
    if (query.exec("SELECT * FROM INVITE"))
    {
        tableWidget->setColumnCount(7);
        tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Profession", "Email", "Nombre d'abonnes", "Nombre de Participation"});

        int RowNumber = 0;
        while (query.next())
        {
            tableWidget->insertRow(RowNumber);
            for (int col = 0; col < 7; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                tableWidget->setItem(RowNumber, col, item);
            }
            RowNumber++;
        }
    }
    c.db.close();
}

bool Invite::supprimerInvite(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM INVITE WHERE ID = :id");
    query.bindValue(":id", id);

    return query.exec();
}

bool Invite::modifierInvite(int id, const QString &nom, const QString &prenom, const QString &profession, const QString &email, const QString &nbAbonnes, const QString &nbParticipation)
{
    /*if (!email.contains('@')) {
        QMessageBox::critical(nullptr, "Erreur", "Format d'email invalide.");
        return false; // Vous pouvez également afficher un message d'erreur si nécessaire
    }*/
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

void Invite::rechercherInvite(const QString &searchTerm, QTableWidget *tableWidget)
{
    Connection c;
    if (!c.createconnect()) return;

    QSqlQuery query(c.db);
    query.prepare("SELECT * FROM INVITE WHERE NOM LIKE :searchTerm OR PRENOM LIKE :searchTerm");
    query.bindValue(":searchTerm", "%" + searchTerm + "%");

    if (query.exec())
    {
        tableWidget->setColumnCount(7);
        tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Profession", "Email", "Nombre d'abonnes", "Nombre de Participation"});

        int RowNumber = 0;
        while (query.next())
        {
            tableWidget->insertRow(RowNumber);
            for (int col = 0; col < 7; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                tableWidget->setItem(RowNumber, col, item);
            }
            RowNumber++;
        }
    }
    c.db.close();
}


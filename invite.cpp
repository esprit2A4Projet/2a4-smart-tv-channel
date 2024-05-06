#include "invite.h"
#include <QTableWidget>
#include <QDebug>
#include "connection.h"
#include <QMessageBox>
#include "mainwindow.h"
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>
#include <QTextCharFormat>
#include <QSqlQuery>
#include <QCalendarWidget>

/*-------------------------------------------Constructeur Par Defaut------------------------------------------------------*/
Invite::Invite()
{
    nom="";
    prenom="";
    profession="";
    email="";

}
/*-------------------------------------------Constructeur Parametré------------------------------------------------------*/

Invite::Invite(int id, QString nom, QString prenom, QString profession, QString email, int nbAbonnes, int nbParticipation)
{
    this->id=id;
    this->nom=nom;
    this->prenom=prenom;
    this->profession=profession;
    this->email=email;
    this->nbAbonnes=nbAbonnes;
    this->nbParticipation=nbParticipation;
}
/*-------------------------------------------AJOUT------------------------------------------------------*/
bool Invite::ajouterInvite()
{
    Connection c;
    return c.createconnect() && c.insertData(nom, prenom, profession, email, nbAbonnes, nbParticipation);
}

/*-------------------------------------------AFFICHAGE------------------------------------------------------*/

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
/*-------------------------------------------SUPPRESSION------------------------------------------------------*/

bool Invite::supprimerInvite(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM INVITE WHERE ID = :id");
    query.bindValue(":id", id);

    return query.exec();
}
/*-------------------------------------------MODIFICATION------------------------------------------------------*/

bool Invite::modifierInvite(int id, const QString &nom, const QString &prenom, const QString &profession, const QString &email, int &nbAbonnes, int &nbParticipation)
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
/*-------------------------------------------RECHERCHE------------------------------------------------------*/

void Invite::rechercherInvite(const QString &searchTerm, QTableWidget *tableWidget)
{
    Connection c;
    if (!c.createconnect()) return;

    QSqlQuery query(c.db);
    query.prepare("SELECT * FROM INVITE WHERE NOM LIKE :searchTerm");
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
/*-------------------------------------------TRI PAR NBABONNES------------------------------------------------------*/
bool Invite::trierParNbAbonnes(QTableWidget *tableWidget)
{
    Connection c;
    if (!c.createconnect()) return false;

    QSqlQuery query(c.db);
    query.prepare("SELECT * FROM INVITE ORDER BY NBABONNES ASC");


    if (query.exec())
    {
        tableWidget->clearContents();  // Clear existing contents
        tableWidget->setRowCount(0);  // Clear existing rows

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

        c.db.close();
        return true;
    }
    else
    {
        c.db.close();
        return false;
    }
}
/*-------------------------------------------TRI PAR NBPARTICIPATION-------------------------------------------------*/
bool Invite::trierParNbParticipations(QTableWidget *tableWidget)
{
    Connection c;
    if (!c.createconnect()) return false;

    QSqlQuery query(c.db);
    query.prepare("SELECT * FROM INVITE ORDER BY NBPARTICIPATION ASC"); // Tri par le nombre de participations

    if (query.exec())
    {
        tableWidget->clearContents();  // Clear existing contents
        tableWidget->setRowCount(0);  // Clear existing rows

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

        c.db.close();
        return true;
    }
    else
    {
        c.db.close();
        return false;
    }
}




/*-------------------------------------------EXPORTATION------------------------------------------------------*/
bool Invite::exportToPDF(const QString &filePath, QTableWidget *tableWidget)
{
    // Create a printer
    QPrinter printer;

    // Set printer properties
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Create a painter
    QPainter painter(&printer);

    // Set painter properties
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Begin painting
    painter.begin(&printer);

    // Draw the table contents
    tableWidget->render(&painter);

    // End painting
    painter.end();

    return true;
}
/*------------------------------------------CALENDRIER------------------------------------------------*/

void Invite::afficherDatePodcastsCalendrier(QCalendarWidget *calendarWidget) {
    // Créer un format de texte pour les dates de podcast
       QTextCharFormat format;
       format.setBackground(QColor(4, 16, 59)); // Changer la couleur en violet (Qt::magenta)
       format.setForeground(Qt::white);


       // Récupérer les dates de podcast à partir de la table "participer"
       Connection c;
       if (!c.createconnect()) {
           // Gestion de l'erreur de connexion à la base de données
           return;
       }

       QSqlQuery query(c.db);
       if (query.exec("SELECT DISTINCT p.DATE_POD "
                          "FROM PARTICIPER pa "
                          "JOIN PODCASTS p ON pa.ID_PODCAST = p.ID_PODCAST")) {
               while (query.next()) {
                   QString dateString = query.value(0).toString();
                   QDate date = QDate::fromString(dateString, "dd/MM/yyyy"); // Convertir la chaîne en QDate
                   if (date.isValid()) {
                       calendarWidget->setDateTextFormat(date, format);
                   }
               }
           } else {
               // Gestion de l'erreur d'exécution de la requête
               QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la récupération des dates de podcast !");
           }

           c.db.close();

}
void Invite::effacerDatesPodcastsCalendrier(QCalendarWidget *calendarWidget) {
    // Créer un format de texte par défaut pour les dates du calendrier
    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::NoBrush); // Pas de couleur de fond
    defaultFormat.setForeground(Qt::NoBrush); // Pas de couleur de texte

    // Appliquer le format par défaut à toutes les dates du calendrier
    calendarWidget->setDateTextFormat(QDate(), QTextCharFormat());
}

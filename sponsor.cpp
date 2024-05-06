#include "sponsor.h"
#include "connection.h"
#include <QString>
#include <QDebug>
#include <QObject>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QDate>
#include <QMessageBox>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCoreApplication>

Sponsor::Sponsor()
{
    nom="";
    budget="";
    pack="";
    date_deb="";
    date_fin="";
    telephone="";
}

Sponsor::Sponsor(int id_sponsor, QString nom, QString budget, QString pack, QString date_deb, QString date_fin, QString telephone)
{
    this->id_sponsor=id_sponsor;
    this->nom=nom;
    this->budget=budget;
    this->pack=pack;
    this->date_deb=date_deb;
    this->date_fin=date_fin;
    this->telephone=telephone;
}


QString Sponsor::Getnom()
{ return nom; }

void Sponsor::Setnom(QString val)
{ nom = val; }

QString Sponsor::Getbudget()
{ return budget; }

void Sponsor::Setbudget(QString val)
{ budget = val; }

QString Sponsor::Getpack()
{ return pack; }

void Sponsor::Setpack(QString val)
{ pack = val; }

QString Sponsor::Getdate_deb()
{ return date_deb; }

void Sponsor::Setdate_deb(QString val)
{ date_deb = val; }

QString Sponsor::Getdate_fin()
{ return date_fin; }

void Sponsor::Setdate_fin(QString val)
{ date_fin = val; }

QString Sponsor::Gettelephone()
{ return telephone; }

void Sponsor::Settelephone(QString val)
{ telephone = val; }


bool Sponsor::ajouter()
{
    Connection c;
    QSqlQuery query;
    if (c.createconnect())
    {
        return c.insertDataS(nom, budget, pack, date_deb, date_fin, telephone);
    }

    return false;
}

void Sponsor::afficher(QTableWidget *tableWidget)
{
    Connection c;

    QSqlQuery query(c.db);
    if (query.exec("SELECT * FROM SPONSOR"))
    {
        tableWidget->clearContents(); // Clear existing content
        tableWidget->setRowCount(0); // Reset row count

        tableWidget->setColumnCount(7);
        QStringList labels;
        labels <<"Id" << "Nom" << "Budget" << "Pack" << "Date debut" << "Date fin" << "Telephone";
        tableWidget->setHorizontalHeaderLabels(labels);
        int RowNumber = 0;
        while (query.next())
        {
            tableWidget->insertRow(RowNumber);
            QTableWidgetItem *nom = new QTableWidgetItem;
            QTableWidgetItem *budget = new QTableWidgetItem;
            QTableWidgetItem *pack = new QTableWidgetItem;
            QTableWidgetItem *date_deb = new QTableWidgetItem;
            QTableWidgetItem *date_fin = new QTableWidgetItem;
            QTableWidgetItem *telephone = new QTableWidgetItem;
            QTableWidgetItem *id = new QTableWidgetItem;

            nom->setText(query.value("NOM").toString());
            budget->setText(query.value("BUDGET").toString());
            pack->setText(query.value("PACK").toString());
            date_deb->setText(query.value("DATE_DEB").toString());
            date_fin->setText(query.value("DATE_FIN").toString());
            telephone->setText(query.value("TELEPHONE").toString());
            id->setText(query.value("ID_SPONSOR").toString());

            tableWidget->setItem(RowNumber, 0, id);
            tableWidget->hideColumn(0);
            tableWidget->setItem(RowNumber, 1, nom);
            tableWidget->setItem(RowNumber, 2, budget);
            tableWidget->setItem(RowNumber, 3, pack);
            tableWidget->setItem(RowNumber, 4, date_deb);
            tableWidget->setItem(RowNumber, 5, date_fin);
            tableWidget->setItem(RowNumber, 6, telephone);
            RowNumber++;
        }
    }
    c.db.close();
}


bool Sponsor::supprimer(int id)
{
    QSqlQuery query;
    QString res = QString::number(id);
       query.prepare("DELETE FROM SPONSOR WHERE ID_SPONSOR = :id");
       query.bindValue(":id", res);

       if (query.exec()) {
           return true;
       } else {
           qDebug() << "Error deleting data:" << query.lastError().text();
           return false;
       }
}

bool Sponsor::modifier(int id_sponsor, const QString &nom, const QString &budget, const QString &pack, const QString &date_deb, const QString &date_fin, const QString &telephone)
{
    QSqlQuery query;
    query.prepare("UPDATE SPONSOR SET NOM = :nom, BUDGET = :budget, PACK = :pack, DATE_DEB = :date_deb, DATE_FIN = :date_fin, TELEPHONE = :telephone WHERE ID_SPONSOR = :id_sponsor");
    query.bindValue(":nom", nom);
    query.bindValue(":budget", budget);
    query.bindValue(":pack", pack);
    query.bindValue(":date_deb", date_deb);
    query.bindValue(":date_fin", date_fin);
    query.bindValue(":telephone", telephone);
    query.bindValue(":id_sponsor",id_sponsor);
    if (!query.exec())
    {
        qDebug() << "Erreur de mise a jour : " << query.lastError().text();
        return false;
    }
    return true;
}

void Sponsor::rechercherS(const QString &nomSponsor, QTableWidget *tableWidget)
{
    Connection c;

    QSqlQuery query(c.db);
    query.prepare("SELECT * FROM SPONSOR WHERE NOM = :nomSponsor");
    query.bindValue(":nomSponsor", nomSponsor);

    if (query.exec())
    {
        tableWidget->setColumnCount(7);
        tableWidget->hideColumn(0);
        tableWidget->setHorizontalHeaderLabels({"ID","Nom", "Budget", "Pack", "Date debut", "Date fin", "Telephone"});

        int RowNumber = 0;
        bool sponsorFound = false; // Flag to track if the sponsor name is found in the database
        while (query.next())
        {
            sponsorFound = true;
            if (RowNumber >= tableWidget->rowCount()) {
                tableWidget->insertRow(RowNumber);
            }
            for (int col = 0; col < 7; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                tableWidget->setItem(RowNumber, col, item);
            }
            RowNumber++;
        }

        // Display a message if the sponsor name doesn't exist
        if (!sponsorFound) {
            QMessageBox::information(nullptr, "Information", "Aucun sponsor avec ce nom n'a été trouvé dans la base de données.");
        } else {
            // If sponsor found, resize the table to fit the contents
            tableWidget->setRowCount(RowNumber);
        }
    }
    else
    {
        qDebug() << "Error executing query: " << query.lastError().text();
    }

    c.db.close();
}




bool Sponsor::trierParPack(QTableWidget *tableWidget)
{
    Connection c;

    QSqlQuery query(c.db);
    query.prepare("SELECT * FROM SPONSOR ORDER BY PACK ASC");


    if (query.exec())
    {
        tableWidget->setSortingEnabled(true);
        tableWidget->sortByColumn(3, Qt::AscendingOrder);

            // Optional: You can reset the sorting mode if needed
            tableWidget->setSortingEnabled(false);

        c.db.close();
        return true;
    }
    else
    {
        c.db.close();
        return false;
    }
}

bool Sponsor::trierParDateDeb(QTableWidget *tableWidget)
{
    Connection c;

    QSqlQuery query(c.db);
    query.prepare("SELECT * FROM SPONSOR ORDER BY DATE_DEB ASC");

    if (query.exec())
    {
        tableWidget->setSortingEnabled(true);
        tableWidget->sortByColumn(4, Qt::AscendingOrder); // Assuming DATE_DEB is in the 5th column (index 4)

        // Optional: You can reset the sorting mode if needed
         tableWidget->setSortingEnabled(false);

        c.db.close();
        return true;
    }
    else
    {
        c.db.close();
        return false;
    }
}


QString Sponsor::constructSMS(const QString& nomSponsor)
{
    Connection c;

    QSqlQuery query(c.db);
    query.prepare("SELECT * FROM SPONSOR WHERE NOM = :nomSponsor");
    query.bindValue(":nomSponsor", nomSponsor);
    if (!query.exec())
    {
        qDebug() << "Error construction: Failed to execute query";
        c.db.close();
        return ""; // Return empty message if query execution fails
    }
    else {
    if (query.next())
    {
        QString message = "Cher " + query.value("NOM").toString() + ", j'espère que ce message vous convient.\n"
                          "Nous sommes ravis de confirmer notre accord de parrainage pour INSIGHTIFY.\n"
                          "Voici vos informations relatives :\n"
                          "Votre budget est : " + query.value("BUDGET").toString() + "\n"
                          "Votre pack est : " + query.value("PACK").toString() + "\n"
                          "Date debut contrat : " + query.value("DATE_DEB").toString() + "\n"
                          "Date fin contrat : " + query.value("DATE_FIN").toString() + "\n";
        c.db.close();
        return message;
    } else
    {
        qDebug() << "Error: No sponsor found with the given name";
        c.db.close();
        return ""; // Return empty message if no sponsor found
    }
    }
}


bool Sponsor::sponsorExists(const QString& nomSponsor)
{
    Connection c;
    QSqlQuery query(c.db);
    query.prepare("SELECT COUNT(*) FROM SPONSOR WHERE NOM = :nomSponsor");
    query.bindValue(":nomSponsor", nomSponsor);
    if (!query.exec()) {
        qDebug() << "Error existance: Failed to execute query";
        return false; // Return false if query execution fails
    }

    // Check if any records were found
    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0; // Return true if sponsor exists, false otherwise
    } else {
        qDebug() << "Error: Unable to retrieve result from query";
        return false; // Return false if query result retrieval fails
    }
}

void Sponsor::sendSMS(const QString& phoneNumber, const QString& message ) {
    // Twilio Account SID, Auth Token, and Twilio phone number
    QString accountSid = "ACa18405085019ebc311b26b8ade0edd23";
    QString authToken = "f4842fc74fb08e3c8918b82b67bbaffc";
    QString twilioPhoneNumber = "+12185146708";

    // Twilio API endpoint
    QUrl apiUrl("https://api.twilio.com/2010-04-01/Accounts/ACa18405085019ebc311b26b8ade0edd23/Messages.json");

    // Create a request
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", "Basic " + QByteArray((accountSid + ":" + authToken).toUtf8()).toBase64());

    // Prepare POST data
    QUrlQuery postData;
    postData.addQueryItem("To", phoneNumber);
    postData.addQueryItem("From", twilioPhoneNumber);
    postData.addQueryItem("Body", message);

    // Create a network manager and send the request
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkReply* reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    // Handle the reply
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(nullptr, "SMS envoye", "SMS envoyé avec succès.");
        } else {
            QMessageBox::critical(nullptr, "SMS erreur", "Échec de l'envoi du SMS. Erreur: " + reply->errorString());
        }

        // Clean up
        reply->deleteLater();
        manager->deleteLater();
    });
}

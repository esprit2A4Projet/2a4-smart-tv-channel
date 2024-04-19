#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "invite.h"
#include <QMainWindow>
#include "connection.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTcpSocket>
#include <QtCore>
#include "smtp.h"
#include <QtNetwork>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QSqlRecord>
#include <QPrinter>
#include <QPrintDialog>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget())); // connecter le signal envers le slot

   //initialiser la table
   ui->tableWidget->setColumnCount(7);
   QStringList labels{"ID", "Nom", "Prenom", "Profession", "Email", "Nombre d'abonnes", "Nombre de Participation"};
   ui->tableWidget->setHorizontalHeaderLabels(labels);

   Connection c;
   if (c.createconnect()) {
       QString searchTerm =ui->lineEdit_rechercher->text();
       updateTableWidget(searchTerm);
   }

   connect(ui->pushButton_annulerInvite, &QPushButton::clicked, this, &MainWindow::on_pushButton_annulerInvite_clicked);
   //controle de saisie

   ui->label_emailError->setVisible(false);
   ui->label_nbAbonnesError->setVisible(false);
   ui->label_nbParticipationError->setVisible(false);
   ui->label_nomError->setVisible(false);
   ui->label_prenomError->setVisible(false);
   ui->label_professionError->setVisible(false);
   ui->label_emailInviteError->setVisible(false);

   connect(ui->lineEdit_email, &QLineEdit::textChanged, this, &MainWindow::validateEmail);
   connect(ui->lineEdit_nbAbonnes, &QLineEdit::textChanged, this, &MainWindow::validateNbAbonnes);
   connect(ui->lineEdit_nbParticipation, &QLineEdit::textChanged, this, &MainWindow::validateNbParticipation);
   connect(ui->lineEdit_nom, &QLineEdit::textChanged, this, &MainWindow::validateNom);
   connect(ui->lineEdit_prenom, &QLineEdit::textChanged, this, &MainWindow::validatePrenom);
   connect(ui->lineEdit_profession, &QLineEdit::textChanged, this, &MainWindow::validateProfession);
   connect(ui->lineEdit_emailInvite, &QLineEdit::textChanged, this, &MainWindow::validateEmailInvite);
   /****************FONCTIONNALITES******************/
   Invite i;
   i.afficherDatePodcastsCalendrier(ui->calendarWidget);

   ui->tableWidget->hideColumn(0); // Cette ligne masque la première colonne
   connect(ui->calendarWidget, SIGNAL(clicked(const QDate&)), this, SLOT(on_calendarWidget_clicked(const QDate&)));

   statistiquesNbAbonnes();
   // Dans le constructeur ou l'initialisation de votre classe MainWindow
   disconnect(ui->calendarWidget, SIGNAL(clicked(const QDate&)), this, SLOT(on_calendarWidget_clicked(const QDate&)));
   connect(ui->calendarWidget, SIGNAL(clicked(const QDate&)), this, SLOT(on_calendarWidget_clicked(const QDate&)));



}


//destructeur
MainWindow::~MainWindow()
{
   delete ui;
}
/************************************************************************CRUD***********************************************************************************/

/*-----------------------------------------------------------------------Ajouter Un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_ajouterInvite_clicked() {
    // Valider tous les champs
    validateEmail(ui->lineEdit_email->text());
    validateNbAbonnes(ui->lineEdit_nbAbonnes->text());
    validateNbParticipation(ui->lineEdit_nbParticipation->text());
    validateNom(ui->lineEdit_nom->text());
    validatePrenom(ui->lineEdit_prenom->text());
    validateProfession(ui->lineEdit_profession->text());

    // Vérifier si toutes les validations sont réussies et si tous les champs sont remplis
    if (ui->label_emailError->isVisible() ||
        ui->label_nbAbonnesError->isVisible() ||
        ui->label_nbParticipationError->isVisible() ||
        ui->label_nomError->isVisible() ||
        ui->label_prenomError->isVisible() ||
        ui->label_professionError->isVisible() ||
        ui->lineEdit_email->text().isEmpty() ||
        ui->lineEdit_nbAbonnes->text().isEmpty() ||
        ui->lineEdit_nbParticipation->text().isEmpty() ||
        ui->lineEdit_nom->text().isEmpty() ||
        ui->lineEdit_prenom->text().isEmpty() ||
        ui->lineEdit_profession->text().isEmpty()) {
        showCustomMessageBox("Erreur", "Veuillez vérifier les données saisies.", QMessageBox::Critical);
        return;
    }

    // Si toutes les validations réussissent et tous les champs sont remplis, ajouter l'invite à la base de données
    Invite i;
    i.setnom(ui->lineEdit_nom->text());
    i.setprenom(ui->lineEdit_prenom->text());
    i.setprofession(ui->lineEdit_profession->text());
    i.setemail(ui->lineEdit_email->text());
    i.setnbAbonnes(ui->lineEdit_nbAbonnes->text().toInt());
    i.setnbParticipation(ui->lineEdit_nbParticipation->text().toInt());

    Connection c;
    if (c.createconnect()) {
        if (i.ajouterInvite()) {
            updateTableWidget(ui->lineEdit_rechercher->text());
            showCustomMessageBox("Succès", "Succès d'ajout dans la base de données", QMessageBox::Information);
        } else {
            showCustomMessageBox("Erreur", "Erreur de l'insertion dans la base de données", QMessageBox::Critical);
        }
    } else {
        showCustomMessageBox("Erreur", "Connexion impossible à la base de données.", QMessageBox::Critical);
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Mettre à jour les informations d'un Invite-------------------------------------------------------------------*/

void MainWindow::on_pushButton_updateInvite_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidget->item(row, 0)->text().toInt();

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidget->item(row, 1)->text();
        QString prenom = ui->tableWidget->item(row, 2)->text();
        QString profession = ui->tableWidget->item(row, 3)->text();
        QString email = ui->tableWidget->item(row, 4)->text();
        QString nbAbonnesStr = ui->tableWidget->item(row, 5)->text();
        QString nbParticipationStr = ui->tableWidget->item(row, 6)->text();

        // Contrôle de saisie pour le nom et le prénom (max 10 caractères)
        bool isNomValid = !nom.isEmpty() && nom.length() <= 10;
        bool isPrenomValid = !prenom.isEmpty() && prenom.length() <= 10;

        // Contrôle de saisie pour la profession (commencer par une majuscule)
        bool isProfessionValid = !profession.isEmpty() && profession.at(0).isUpper();

        // Contrôle de saisie pour l'email (format valide)
        QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
        bool isEmailValid = emailRegex.exactMatch(email);

        // Vérifiez si toutes les validations ont réussi
        QString errorMessage;
        if (!isNomValid) {
            errorMessage += "Le nom ne doit pas dépasser 10 lettres\n";
        }
        if (!isPrenomValid) {
            errorMessage += "Le prenom ne doit pas dépasser 10 lettres\n";
        }
        if (!isProfessionValid) {
            errorMessage += "La profession doit commencer par une lettre majuscule \n";
        }
        if (!isEmailValid) {
            errorMessage += "Verifier le format de l'email\n";
        }

        // Vérifiez si nbAbonnes et nbParticipation sont des entiers
        bool isNbAbonnesValid, isNbParticipationValid;
        int nbAbonnes = nbAbonnesStr.toInt(&isNbAbonnesValid);
        int nbParticipation = nbParticipationStr.toInt(&isNbParticipationValid);
        if (!isNbAbonnesValid) {
            errorMessage += "Nombre d'abonnés doit être un entier.\n";
        }
        if (!isNbParticipationValid) {
            errorMessage += "Nombre de participations doit être un entier.\n";
        }

        if (!errorMessage.isEmpty()) {
            showCustomMessageBox("Erreur", errorMessage, QMessageBox::Critical);
            return;
        }

        // Appelez la fonction modifierInvite pour mettre à jour la base de données
        Invite i;
        if (i.modifierInvite(idToModify, nom, prenom, profession, email, nbAbonnes, nbParticipation)) {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(prenom));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(profession));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(email));

            ui->tableWidget->item(row, 5)->setFlags(ui->tableWidget->item(row, 5)->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->item(row, 6)->setFlags(ui->tableWidget->item(row, 6)->flags() & ~Qt::ItemIsEditable);

            emit dataUpdated(); // Mettre à jour la vue après la modification
            showCustomMessageBox("Succès", "Succès de la modification dans la base de données", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Erreur", "Connexion impossible à la base de données.", QMessageBox::Critical);
        }
    }
    else
    {
        showCustomMessageBox("Attention", "Sélectionnez une ligne à modifier", QMessageBox::Warning);
    }
}


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Supprimer Un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_deleteInvite_clicked()
{
   QItemSelectionModel *select = ui->tableWidget->selectionModel();
   QModelIndexList selectedIndexes = select->selectedIndexes();

   if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
       QTableWidgetItem *item = ui->tableWidget->item(row, 0);
       int idToDelete = item->text().toInt();

       Invite i;
       if (i.supprimerInvite(idToDelete))
       {
           // Supprimer la ligne de l'affichage
           ui->tableWidget->removeRow(row);
           emit dataUpdated(); // Mettre à jour la vue après la suppression
           showCustomMessageBox("Succès", "Donnée supprimée de la base de donnée", QMessageBox::Information);
       }
       else
       {
           showCustomMessageBox("Erreur", "Echec de suppression", QMessageBox::Critical);
       }
   }
   else
   {
       showCustomMessageBox("Attention", "Selectionnez une colonne à supprimer", QMessageBox::Warning);
   }
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************FONCTIONNALITES**************************************************************************************/

/*-----------------------------------------------------------------------Rechercher Un Invite----------------------------------------------------------------------------------*/

void MainWindow::on_pushButton_rechercherInvite_clicked()
{
   QString searchTerm = ui->lineEdit_rechercher->text();
   updateTableWidget(searchTerm);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Trier Un Invite-----------------------------------------------------------------------------------*/
void MainWindow::on_pushButton_trierInvite_clicked()
{
    // Vérifier quelle option de tri est sélectionnée dans le comboBox
    QString triSelectionne = ui->comboBoxTri->currentText();

    Invite i;
    if (triSelectionne == "Nombre d'abonnés")
    {
        if (i.trierParNbAbonnes(ui->tableWidget))
        {
            emit dataUpdated();
            showCustomMessageBox("Succès", "Données triées par nombre d'abonnés", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Erreur", "Echec de tri de données", QMessageBox::Critical);
        }
    }
    else if (triSelectionne == "Nombre de participation")
    {
        if (i.trierParNbParticipations(ui->tableWidget))
        {
            emit dataUpdated();
            showCustomMessageBox("Succès", "Données triées par nombre de participations", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Erreur", "Echec de tri de données", QMessageBox::Critical);
        }
    }

}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Exporter les informations d'un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_exporterInvite_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Sauvegarder en PDF", "", "Fichiers PDF (*.pdf)");

    if (!filePath.isEmpty())
    {
        QString strStream;
        QTextStream out(&strStream);

        const int rowCount = ui->tableWidget->rowCount();
        const int columnCount = ui->tableWidget->columnCount();

        out << "<html>\n"
               "<head>\n"
               "<meta Content=\"Text/html; charset=Windows-1251\">\n"
               << QString("<title>%1</title>\n").arg("Liste des invites")
               << "<style>"
               "   body { font-family: 'Space Grotesk'; background-color: #ffffff; }"
               "   table { border-collapse: collapse; width: 100%; }"
               "   th, td { border: 1px solid #dddddd; text-align: left; padding: 8px; }"
               "   th { background-color: #E8E8FD; }"
               "</style>\n"
               "</head>\n"
               "<body>\n";

        // Add logo and application name
        out << "<div style=\"text-align: right;\"><img src=\"C:/Users/ibtis/OneDrive/Desktop/ESPRIT/initiation/logoo.png\" width=\"250\" height=\"100\"><br>"
               "<h1 style=\"color: #E8E8FD; text-align: center;\">Application Insightify</h1></div>\n";
        "<h2 style=\"color: #E8E8FD; text-align: center;\">Liste des invites</h1></div>\n";



        // Add table
        out << "<table>\n"
               "<thead><tr> <th>Numéro</th>";
        for (int column = 0; column < columnCount; column++)
            out << QString("<th>%1</th>").arg(ui->tableWidget->horizontalHeaderItem(column)->text());
        out << "</tr></thead>\n";

        // Add data rows
        for (int row = 0; row < rowCount; row++)
        {
            out << "<tr> <td>" << row + 1 << "</td>";
            for (int column = 0; column < columnCount; column++)
            {
                QTableWidgetItem *item = ui->tableWidget->item(row, column);
                QString data = (item) ? item->text().simplified() : "&nbsp;";
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : "&nbsp;");
            }
            out << "</tr>\n";
        }
        out << "</table>\n";

        // Add copyright and team information
        out << "<br><br><div style=\"text-align: center;\">"
               " 2024 Insightify. All rights reserved.<br>"
               "Developed by Insightify Team</div>\n";

        out << "</body>\n"
               "</html>\n";

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(filePath);

        QTextDocument doc;
        doc.setHtml(strStream);
        doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);

        showCustomMessageBox("Succès", "Tableau exporté en PDF.", QMessageBox::Information);
    }
    else
    {
        showCustomMessageBox("Erreur", "Échec de l'exportation du tableau en PDF.", QMessageBox::Critical);
    }
}



/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Effacer le formulaire-------------------------------------------------------------------*/
void MainWindow::on_pushButton_annulerInvite_clicked()
{
   ui->lineEdit_nom->clear();
   ui->lineEdit_prenom->clear();
   ui->lineEdit_profession->clear();
   ui->lineEdit_email->clear();
   ui->lineEdit_nbAbonnes->clear();
   ui->lineEdit_nbParticipation->clear();
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------Rafraichir la table apres chaque modification--------------------------------------------------------------------------------------*/

void MainWindow::updateTableWidget(const QString &filter)
{
   Connection c;
   QSqlQueryModel *model = new QSqlQueryModel();

   if (filter.isEmpty()) {
       // Si le filtre de recherche est vide, récupérer toutes les lignes
       model->setQuery("SELECT * FROM INVITE");
   } else {
       // Si un filtre de recherche est spécifié, utiliser la requête filtrée
       QString query = QString("SELECT * FROM INVITE WHERE Nom LIKE '%1%'").arg(filter);
       model->setQuery(query);
   }

   int RowNumber = 0;
   ui->tableWidget->setRowCount(0);

   while (model->canFetchMore()) {
       model->fetchMore();
   }

   for (int row = 0; row < model->rowCount(); ++row) {
           // Ajouter la ligne uniquement si le filtre est vide ou si c'est la première ligne de la recherche
           ui->tableWidget->insertRow(RowNumber);
           for (int col = 0; col < 7; ++col) {
               QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
               ui->tableWidget->setItem(RowNumber, col, item);
           }
           RowNumber++;
       }

   delete model;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Modifier le style du Message Box-------------------------------------------------------------------*/

void MainWindow::showCustomMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon)
{
   // Créer la boîte de message avec le texte et l'icône fournis
   QMessageBox messageBox(icon, title, message, QMessageBox::Ok, this);

   // Obtenir le bouton Ok
   QAbstractButton *okButton = messageBox.button(QMessageBox::Ok);

   // Styles pour le bouton Ok
   QString okButtonStyle = "QPushButton {"
                           "background-color: #D1D0FB;"
                           "color: white;"
                           "border: 1px solid #C4C4C4;"
                           "border-radius: 5px;"
                           "padding: 5px;"
                           "margin: 4px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: #D3D3D3;"
                           "}";

   okButton->setStyleSheet(okButtonStyle);

   // Appliquer les styles directement au QLabel (texte)
   QLabel *label = messageBox.findChild<QLabel*>("qt_msgbox_label");
   if (label) {
       label->setStyleSheet("min-width: 300px; text-align: center;");
   }

   // Styles pour l'ensemble de la QMessageBox
   QString messageBoxStyle = "QMessageBox {"
                             "background-color: #E8E8FD;"
                             "color: white;"
                             "font-size: 16px;"
                             "font-family: 'Space Grotesk';"
                             "}"
                             "QPushButton:pressed {"
                             "background-color: #A5A5A5;"
                             "}";

   messageBox.setStyleSheet(messageBoxStyle);

   // Connecter l'événement hover personnalisé pour le bouton Ok
   connect(okButton, &QAbstractButton::pressed, [=]() {
       okButton->setStyleSheet("background-color: #A5A5A5;");
   });
   connect(okButton, &QAbstractButton::released, [=]() {
       okButton->setStyleSheet("background-color: #D3D3D3;");
   });

   // Ajuster la taille de la boîte de message
   messageBox.setMinimumSize(300, 100);

   // Afficher la boîte de message
   messageBox.exec();
}
/*---------------------------------------------------------------------------Controle de Saisie-----------------------------------------------------------------------------------*/
// Fonction de validation de l'e-mail
void MainWindow::validateEmail(const QString &text) {
    QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
    bool isValid = emailRegex.exactMatch(text);
    ui->label_emailError->setVisible(!isValid);
    ui->label_emailError->setStyleSheet(isValid ? "" : "color: red;");
}
void MainWindow::validateEmailInvite(const QString &text) {
    QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
    bool isValid = emailRegex.exactMatch(text);
    ui->label_emailInviteError->setVisible(!isValid);
    ui->label_emailInviteError->setStyleSheet(isValid ? "" : "color: red;");
}

// Fonction de validation du nombre d'abonnés
void MainWindow::validateNbAbonnes(const QString &text)
{
    if (!text.isEmpty() && !text.toInt()) {
        // Si le texte n'est pas vide mais ne peut pas être converti en entier, le supprimer
        ui->lineEdit_nbAbonnes->setText("");
    }
}

// Fonction de validation du nombre de participations
void MainWindow::validateNbParticipation(const QString &text)
{
    if (!text.isEmpty() && !text.toInt()) {
        // Si le texte n'est pas vide mais ne peut pas être converti en entier, le supprimer
        ui->lineEdit_nbParticipation->setText("");
    }
}
void MainWindow::validateNom(const QString &text) {
    bool isValid = !text.isEmpty() && text.length() <= 10; // Le nom ne doit pas être vide et ne doit pas dépasser 10 caractères
    ui->label_nomError->setVisible(!isValid);
    ui->label_nomError->setStyleSheet(isValid ? "" : "color: red;");
}

void MainWindow::validatePrenom(const QString &text) {
    bool isValid = !text.isEmpty() && text.length() <= 10; // Le prénom ne doit pas être vide et ne doit pas dépasser 10 caractères
    ui->label_prenomError->setVisible(!isValid);
    ui->label_prenomError->setStyleSheet(isValid ? "" : "color: red;");
}

void MainWindow::validateProfession(const QString &text) {
    bool isValid = !text.isEmpty() && text.at(0).isUpper(); // La profession ne doit pas être vide et doit commencer par une lettre majuscule
    ui->label_professionError->setVisible(!isValid);
    ui->label_professionError->setStyleSheet(isValid ? "" : "color: red;");
}

/*---------------------------------------------------------------------------CALENDRIER-----------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************************************************************************************/

/*--------Rafraichir le calendrier-------------*/
void MainWindow::on_pushButton_rafraichir_clicked()
{
    Invite i;
    // Appeler la fonction pour afficher les dates des podcasts sur le calendrier
    i.afficherDatePodcastsCalendrier(ui->calendarWidget);
    // Effacer le contenu du lineEdit de l'email
    ui->lineEdit_emailInvite->clear();
    ui->calendarWidget->setMinimumDate(QDate(1900, 1, 1));
    ui->calendarWidget->setMaximumDate(QDate(3000, 1, 1));

       // Sélectionner une date qui ne sera jamais utilisée
       ui->calendarWidget->setSelectedDate(QDate(2024, 1, 1));

}
/*--------Verifier si le calendrier est selectionné-------------*/
void MainWindow::on_calendarWidget_clicked()
{
     dateSelected = true;

}

/*---------------------------------------------------------------------------EMAIL-----------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************************************************************************************/

/*--------Verifier si l'email existe dans la bdd-------------*/
bool emailExists(const QString &email)
{
    Connection c;
    if (!c.createconnect())
    {
        return false;
    }

    QSqlQuery query(c.db);
    query.prepare("SELECT COUNT(*) FROM INVITE WHERE Email = ?");
    query.addBindValue(email);

    if (!query.exec())
    {

        return false;
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        if (count > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
/*-------------Envoyer l'Email----------------*/

void MainWindow::on_pushButton_envoyer_clicked()
{
    QString emailAddress = ui->lineEdit_emailInvite->text();
    QDate selectedDate = ui->calendarWidget->selectedDate();

    // Validate the email address
    validateEmailInvite(emailAddress);

    // Check if a date is selected
    if (!emailAddress.isEmpty() && selectedDate.isValid())
    {
        // Check if the email exists in the database
        if (emailExists(emailAddress))
        {
            // Check if the selected date is a podcast date
            if (isPodcastDate(selectedDate))
            {
                // Fetch the name of the podcast based on the selected date
                QString podcastName = getPodcastNameByDate(selectedDate);

                // Create an instance of the Smtp class
                Smtp *smtp = new Smtp("ibtissembenamara14@gmail.com", "pgygpnhtzubfjruk", "smtp.gmail.com", 465);

                // Connect signals from the Smtp object to slots in the MainWindow class to capture status updates or errors
                connect(smtp, SIGNAL(status(QString)), this, SLOT(mailStatus(QString)));

                // Construct the email subject with the podcast date
                QString dateString = selectedDate.toString("dd/MM/yyyy");
                QString subject = "Invitation au podcast \"" + podcastName + "\" du " + dateString;

                // Construct the email message content
                QString messageContent = "Bonjour,\n\nVous êtes cordialement invité au podcast \"" + podcastName + "\" du " + dateString + ".\n\nCordialement,\nInsightify Team";

                // Attempt to send the email
                if (smtp->sendMail("ibtissembenamara14@gmail.com", emailAddress, subject, messageContent))
                {
                    QMessageBox::information(this, "Succès", "L'email a été envoyé avec succès!");
                }
                else
                {

                    QMessageBox::critical(this, "Erreur", "Échec de l'envoi de l'email.");
                }

            }
            else
            {

                QMessageBox::warning(this, "Date invalide", "La date sélectionnée n'est pas une date de podcast valide.");
            }
        }
        else
        {

            QMessageBox::critical(this, "Erreur", "L'e-mail n'existe pas dans la base de données. Impossible d'envoyer l'e-mail.");
        }
    }
    else
    {

        QMessageBox::warning(this, "Champs vides", "Veuillez saisir une adresse e-mail et sélectionner une date avant d'envoyer l'e-mail.");
    }
}

/*--------Verifier si la date selectionnée est une date de podcast-------------*/

bool MainWindow::isPodcastDate(const QDate &date)
{
    Connection c;
    if (!c.createconnect()) return false;

    QSqlQuery query(c.db);

    if (!query.exec("SELECT DATE_POD FROM PODCASTS")) {

        c.db.close();
        return false;
    }

    QList<QDate> podcastDates;

    while (query.next()) {
        QString dateString = query.value(0).toString();
        QDate podcastDate = QDate::fromString(dateString, "dd/MM/yyyy");
        if (podcastDate.isValid()) {
            podcastDates.append(podcastDate);
        } else {

        }
    }

    return podcastDates.contains(date);
}

/*--------obtenir le nom du podcast suivant la date-------------*/

QString MainWindow::getPodcastNameByDate(const QDate &date)
{
    Connection c;
    if (!c.createconnect())
    {

        return QString();
    }

    QSqlQuery query(c.db);

    query.prepare("SELECT NOM FROM PODCASTS WHERE TO_CHAR(TO_DATE(DATE_POD, 'DD/MM/YYYY'), 'DD/MM/YYYY') = :date");

    query.bindValue(":date", date.toString("dd/MM/yyyy"));

    if (!query.exec())
    {

        c.db.close();
        return QString();
    }

    if (query.next())
    {
        QString podcastName = query.value(0).toString();
        c.db.close();
        return podcastName;
    }
    else
    {

        c.db.close();
        return QString();
    }
}



/*--------le status de l'email-------------*/
void MainWindow::mailStatus(const QString &status)
{
    qDebug() << "Mail status:" << status;
}


/*-----------------------------------------------------------------STATISTIQUES------------------------------------------------------------------------*/
/*******************************************************************************************************************************************************************************************/

void MainWindow::statistiquesNbAbonnes()
{
    Connection C;
        C.createconnect();
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM INVITE");

        int totalCount = model->rowCount();

        int count0_1000 = 0;
        int count1000_10000 = 0;
        int count10000_100000 = 0;
        int count100000_plus = 0;

        for (int i = 0; i < model->rowCount(); ++i) {
            int nbAbonnes = model->record(i).value("nbAbonnes").toInt();
            if (nbAbonnes >= 0 && nbAbonnes <= 1000) {
                count0_1000++;
            } else if (nbAbonnes > 1000 && nbAbonnes <= 10000) {
                count1000_10000++;
            } else if (nbAbonnes > 10000 && nbAbonnes <= 100000) {
                count10000_100000++;
            } else if (nbAbonnes > 100000) {
                count100000_plus++;
            }
        }

        // Calculate percentages
        qreal percent0_1000 = static_cast<qreal>(count0_1000) / totalCount * 100;
           qreal percent1000_10000 = static_cast<qreal>(count1000_10000) / totalCount * 100;
           qreal percent10000_100000 = static_cast<qreal>(count10000_100000) / totalCount * 100;
           qreal percent100000_plus = static_cast<qreal>(count100000_plus) / totalCount * 100;

        QString a = QString("0-1k: %1%").arg(percent0_1000, 0, 'f', 2);
        QString b = QString("1k-10k: %1%").arg(percent1000_10000, 0, 'f', 2);
        QString c = QString("10k-100kk: %1%").arg(percent10000_100000, 0, 'f', 2);
        QString d = QString("100k+: %1%").arg(percent100000_plus, 0, 'f', 2);




    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.2);
    series->append(a, count0_1000);
    series->append(b, count1000_10000);
    series->append(c, count10000_100000);
    series->append(d, count100000_plus);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QFont titleFont("Space Grotesk", 11, QFont::Bold);
    chart->setTitleFont(titleFont);
    chart->setTitle("Statistiques des nombres d'invités selon nombre d'abonnés");
    chart->setBackgroundBrush(QBrush(QColor("#E8E8FD")));

    QList<QColor> sliceColors = {
        QColor("#354c7c"),
        QColor("#0b1338"),
        QColor("#a69fca"),
        QColor("#d9d9d9")
    };

    int colorIndex = 0;
    for (QAbstractSeries *series : chart->series()) {
        QPieSeries *pieSeries = qobject_cast<QPieSeries *>(series);
        if (pieSeries) {
            for (QPieSlice *slice : pieSeries->slices()) {
                slice->setColor(sliceColors.at(colorIndex % sliceColors.size()));
                slice->setLabelVisible();
                colorIndex++;
            }
        }
    }

    QFont font("Space Grotesk");
    font.setPointSize(9);
    chart->setTitleFont(font);

    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setMouseTracking(true);

    ui->statS->setVisible(true);
    ui->statS->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QLayout *layout = ui->statS->layout();
    if (layout)
        delete layout;

    layout = new QVBoxLayout(ui->statS);
    layout->addWidget(chartview);
    layout->setAlignment(Qt::AlignCenter);
    ui->statS->setLayout(layout);
}

/*-----------------Actualiser le piechart--------------------*/

void MainWindow::on_pushButton_actualiser_clicked()
{
    statistiquesNbAbonnes();
}

/*-----------------Imprimer le piechart--------------------*/

void MainWindow::on_pushButton_imprimer_clicked()
{
    // Créer un objet printer
    QPrinter printer(QPrinter::HighResolution);

    // Configurer le dialogue d'impression
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() != QDialog::Accepted)
        return; // Sortir si l'utilisateur annule l'impression

    // Créer un painter pour dessiner sur le printer
    QPainter painter(&printer);

    // Récupérer la taille du chartview
    QSize size = ui->statS->size();

    // Créer un facteur d'échelle pour adapter le graphique à la taille de la page A4
    double margin = 50; // in millimeters
    double width = printer.pageRect().width() - 2 * margin;
    double height = printer.pageRect().height() - 2 * margin;
    QSize chartSize(qRound(width), qRound(height));

    // Définir la zone de visualisation du painter pour correspondre à la taille du graphique
    painter.setViewport(0, 0, chartSize.width(), chartSize.height());

    // Centrer et mettre à l'échelle le graphique dans le painter
    double scaleFactor = qMin(chartSize.width() / size.width(), chartSize.height() / size.height());
    painter.translate((printer.pageRect().width() - size.width() * scaleFactor) / 2,
                      (printer.pageRect().height() - size.height() * scaleFactor) / 2);
    painter.scale(scaleFactor, scaleFactor);

    // Dessiner le chartview sur le painter du printer
    ui->statS->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
}

// Connectez le signal clicked de votre QCalendarWidget à ce slot
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    // Récupérez les informations du podcast pour la date sélectionnée
    QString podcastInfo = getPodcastInfoByDate(date);

    // Vérifiez si des informations ont été trouvées
    if (!podcastInfo.isEmpty()) {
        // Créez une fenêtre de dialogue pour afficher les informations du podcast
        QMessageBox::information(this, "Informations du podcast", podcastInfo);
    } else {
        // Si aucune information n'a été trouvée, affichez un message d'erreur
        QMessageBox::warning(this, "Aucun podcast trouvé", "Aucun podcast n'est prévu pour la date sélectionnée.");
    }
}

// Méthode pour récupérer les informations du podcast à partir de la base de données
QString MainWindow::getPodcastInfoByDate(const QDate &date)
{
    Connection c;
    if (!c.createconnect()) {
        return QString();
    }

    QSqlQuery query(c.db);
    query.prepare("SELECT NOM, DUREE, LIEU, CATEGORIE FROM PODCASTS WHERE DATE_POD = :date");
    query.bindValue(":date", date.toString("dd/MM/yyyy"));

    if (!query.exec()) {
        c.db.close();
        return QString();
    }

    if (query.next()) {
        QString podcastName = query.value(0).toString();
        QString podcastDuration = query.value(1).toString();
        QString podcastLocation = query.value(2).toString();
        QString podcastCategory = query.value(3).toString();

        // Construisez une chaîne de texte avec les informations récupérées
        QString podcastInfo = "Nom : " + podcastName + "\n"
                            + "Durée : " + podcastDuration + "\n"
                            + "Lieu : " + podcastLocation + "\n"
                            + "Catégorie : " + podcastCategory;

        c.db.close();
        return podcastInfo;
    } else {
        c.db.close();
        return QString();
    }
}

/*-------------------------------------------------------------------------------------------------------FIN----------------------------------------------------------------------------------------------*/

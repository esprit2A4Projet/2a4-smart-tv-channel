#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "invite.h"
#include "connection.h"
#include <QMessageBox>
#include <QFileDialog>

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
   } else {
      // showCustomMessageBox("Error", "Failed to connect to the database.");
   }

   connect(ui->pushButton_annuler, &QPushButton::clicked, this, &MainWindow::on_pushButton_annuler_clicked);
   //controle de saisie

   ui->label_emailError->setVisible(false);
   ui->label_nbAbonnesError->setVisible(false);
   ui->label_nbParticipationError->setVisible(false);
   ui->label_nomError->setVisible(false);
   ui->label_prenomError->setVisible(false);
   ui->label_professionError->setVisible(false);

   connect(ui->lineEdit_email, &QLineEdit::textChanged, this, &MainWindow::validateEmail);
   connect(ui->lineEdit_nbAbonnes, &QLineEdit::textChanged, this, &MainWindow::validateNbAbonnes);
   connect(ui->lineEdit_nbParticipation, &QLineEdit::textChanged, this, &MainWindow::validateNbParticipation);
   connect(ui->lineEdit_nom, &QLineEdit::textChanged, this, &MainWindow::validateNom);
   connect(ui->lineEdit_prenom, &QLineEdit::textChanged, this, &MainWindow::validatePrenom);
   connect(ui->lineEdit_profession, &QLineEdit::textChanged, this, &MainWindow::validateProfession);
}


//destructeur
MainWindow::~MainWindow()
{
   delete ui;
}
/************************************************************************CRUD***********************************************************************************/

/*-----------------------------------------------------------------------Ajouter Un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_ajouter_clicked() {
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

void MainWindow::on_pushButton_update_clicked()
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
       int nbAbonnes = ui->tableWidget->item(row, 5)->text().toInt();
       int nbParticipation = ui->tableWidget->item(row, 6)->text().toInt();

       // controle de saisie (email)
       if (!email.isEmpty() && ui->tableWidget->currentColumn() == 4) {
           QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
           if (!emailRegex.exactMatch(email)) {
               showCustomMessageBox("Error", "Format d'email invalide.", QMessageBox::Critical);
               return;
           }
       }

       // controle de saisie (nbAbonnes et nbParticipation)
       bool nbAbonnesOk, nbParticipationOk;
       nbAbonnesOk = true;
       nbParticipationOk = true;

       // verifier la saisie
       if (ui->tableWidget->currentColumn() == 5) {
           nbAbonnesOk = true;
       } else if (ui->tableWidget->currentColumn() == 6) {
           nbParticipation = ui->tableWidget->item(row, 6)->text().toInt(&nbParticipationOk);
       }


       if (!nbAbonnesOk || !nbParticipationOk) {
           showCustomMessageBox("Error", "Veuillez entrer des nombres valides pour Nombre d'abonnes et Nombre de Participation.", QMessageBox::Critical);
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
           ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(nbAbonnes)));
           ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(nbParticipation)));

           emit dataUpdated(); // Mettre à jour la vue après la modification
           showCustomMessageBox("Succés", "Succés d'ajout dans la base de données", QMessageBox::Information);
       }
       else
       {
           showCustomMessageBox("Erreur", "Connexion impossible à la base de données.", QMessageBox::Critical);
       }
   }
   else
   {
       showCustomMessageBox("Attention", "Selectionnez une ligne à modifier", QMessageBox::Warning);
   }
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Supprimer Un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_delete_clicked()
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
           showCustomMessageBox("Success", "Data deleted from the database.", QMessageBox::Information);
       }
       else
       {
           showCustomMessageBox("Error", "Failed to delete data from the database.", QMessageBox::Critical);
       }
   }
   else
   {
       showCustomMessageBox("Warning", "Please select a row to delete.", QMessageBox::Warning);
   }
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************FONCTIONNALITES**************************************************************************************/

/*-----------------------------------------------------------------------Rechercher Un Invite----------------------------------------------------------------------------------*/

void MainWindow::on_pushButton_rechercher_clicked()
{
   QString searchTerm = ui->lineEdit_rechercher->text();
   updateTableWidget(searchTerm);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Trier Un Invite-----------------------------------------------------------------------------------*/
void MainWindow::on_pushButton_trier_clicked()
{
    Invite i;
    if (i.trierParNbAbonnes(ui->tableWidget)) {
        emit dataUpdated();  // Emit the signal to update the table
        showCustomMessageBox("Success", "Data sorted by Nombre d'abonnes.", QMessageBox::Information);
    } else {
        showCustomMessageBox("Error", "Failed to sort data.", QMessageBox::Critical);
    }
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Exporter les informations d'un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_exporter_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");

    if (!filePath.isEmpty())
    {
        Invite i;
        if (i.exportToPDF(filePath, ui->tableWidget))
        {
            showCustomMessageBox("Success", "Table exported to PDF.", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Error", "Failed to export table to PDF.", QMessageBox::Critical);
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Effacer le formulaire-------------------------------------------------------------------*/
void MainWindow::on_pushButton_annuler_clicked()
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
       QString query = QString("SELECT * FROM INVITE WHERE Nom LIKE '%1%' OR Prenom LIKE '%1%' OR Profession LIKE '%1%' OR Email LIKE '%1%'").arg(filter);
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

// Fonction de validation du nombre d'abonnés
void MainWindow::validateNbAbonnes(const QString &text) {
    bool isValid = !text.isEmpty() && text.toInt() > 0;
    ui->label_nbAbonnesError->setVisible(!isValid);
    ui->label_nbAbonnesError->setStyleSheet(isValid ? "" : "color: red;");
}

// Fonction de validation du nombre de participations
void MainWindow::validateNbParticipation(const QString &text) {
    bool isValid = !text.isEmpty() && text.toInt() > 0;
    ui->label_nbParticipationError->setVisible(!isValid);
     ui->label_nbParticipationError->setStyleSheet(isValid ? "" : "color: red;");
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

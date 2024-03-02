#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "invite.h"
#include "connection.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent),
     ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget())); // Connect the signal to the slot

   // Initialize the table widget
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
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    Invite i;
    i.setnom(ui->lineEdit_nom->text());
    i.setprenom(ui->lineEdit_prenom->text());
    i.setprofession(ui->lineEdit_profession->text());
    i.setemail(ui->lineEdit_email->text());
    i.setnbAbonnes(ui->lineEdit_nbAbonnes->text());
    i.setnbParticipation(ui->lineEdit_nbParticipation->text());

    Connection c;
    if (c.createconnect()) {
        if (i.ajouterInvite()) {
            updateTableWidget(ui->lineEdit_rechercher->text());
            showCustomMessageBox("Success", "Data inserted into the database.", QMessageBox::Information);
        } else {
            showCustomMessageBox("Error", "Failed to insert data into the database.", QMessageBox::Critical);
        }
    } else {
        showCustomMessageBox("Error", "Failed to connect to the database.", QMessageBox::Critical);
    }
}


void MainWindow::on_pushButton_annuler_clicked()
{
   ui->lineEdit_nom->clear();
   ui->lineEdit_prenom->clear();
   ui->lineEdit_profession->clear();
   ui->lineEdit_email->clear();
   ui->lineEdit_nbAbonnes->clear();
   ui->lineEdit_nbParticipation->clear();
}

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



void MainWindow::on_pushButton_delete_clicked()
{
   QItemSelectionModel *select = ui->tableWidget->selectionModel();
   QModelIndexList selectedIndexes = select->selectedIndexes();

   if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
       QTableWidgetItem *item = ui->tableWidget->item(row, 0);
       int idToDelete = item->text().toInt();

       Invite i;
       if (i.supprimerInvite(idToDelete)) {
           // Supprimer la ligne de l'affichage
           ui->tableWidget->removeRow(row);
           emit dataUpdated(); // Mettre à jour la vue après la suppression
           showCustomMessageBox("Success", "Data deleted from the database.", QMessageBox::Information);
       } else {
           showCustomMessageBox("Error", "Failed to delete data from the database.", QMessageBox::Critical);
       }
   } else {
       showCustomMessageBox("Warning", "Please select a row to delete.", QMessageBox::Warning);
   }
}

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
       QString nbAbonnes = ui->tableWidget->item(row, 5)->text();
       QString nbParticipation = ui->tableWidget->item(row, 6)->text();

       // Appelez la fonction modifierInvite pour mettre à jour la base de données
       Invite i;
       if (i.modifierInvite(idToModify, nom, prenom, profession, email, nbAbonnes, nbParticipation)) {
           // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
           ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
           ui->tableWidget->setItem(row, 2, new QTableWidgetItem(prenom));
           ui->tableWidget->setItem(row, 3, new QTableWidgetItem(profession));
           ui->tableWidget->setItem(row, 4, new QTableWidgetItem(email));
           ui->tableWidget->setItem(row, 5, new QTableWidgetItem(nbAbonnes));
           ui->tableWidget->setItem(row, 6, new QTableWidgetItem(nbParticipation));

           emit dataUpdated(); // Mettre à jour la vue après la modification
           showCustomMessageBox("Success", "Data modified in the database.", QMessageBox::Information);
       } else {
           showCustomMessageBox("Error", "Failed to modify data in the database.", QMessageBox::Critical);
       }
   } else {
       showCustomMessageBox("Warning", "Please select a row to modify.", QMessageBox::Warning);
   }
}

void MainWindow::on_pushButton_rechercher_clicked()
{
   QString searchTerm = ui->lineEdit_rechercher->text();
   updateTableWidget(searchTerm);
}
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

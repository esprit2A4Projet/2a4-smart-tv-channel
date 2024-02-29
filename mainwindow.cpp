#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "invite.h"
#include "connection.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget())); // Connect the signal to the slot

    // Initialize the table widget
    ui->tableWidget->setColumnCount(7);
    QStringList labels{"ID", "Nom", "Prenom", "Profession", "Email", "Nombre d'abonnes", "Nombre de Participation"};
    ui->tableWidget->setHorizontalHeaderLabels(labels);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Invite i;
    i.setnom(ui->lineEdit_nom->text());
    i.setprenom(ui->lineEdit_prenom->text());
    i.setprofession(ui->lineEdit_profession->text());
    i.setemail(ui->lineEdit_email->text());
    i.setnbAbonnes(ui->lineEdit_nbAbonnes->text());
    i.setnbParticipation(ui->lineEdit_nbParticipation->text());

    Connection c;
    if (c.createconnect())
    {
        if (c.insertData(i.get_nom(), i.get_prenom(), i.get_profession(), i.get_email(), i.get_nbAbonnes(), i.get_nbParticipation()))
        {
            emit dataUpdated(); // Emit the signal after successful insertion
            QMessageBox::information(this, "Success", "Data inserted into the database.");
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to insert data into the database.");
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
    }
}

void MainWindow::updateTableWidget()
{
    Connection c;
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM INVITE");

    int RowNumber = 0;
    ui->tableWidget->setRowCount(0); // Clear existing rows

    while (model->canFetchMore())
        model->fetchMore();

    for (int row = 0; row < model->rowCount(); ++row)
    {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 7; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
        RowNumber++;
    }

    delete model; // Release the memory occupied by the model
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
            QMessageBox::information(this, "Success", "Data deleted from the database.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete data from the database.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please select a row to delete.");
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
            QMessageBox::information(this, "Success", "Data modified in the database.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to modify data in the database.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please select a row to modify.");
    }
}




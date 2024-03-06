#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include "connection.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget())); // Connect the signal to the slot

    // Initialize the table widget
    ui->tableWidget->setColumnCount(6);
    QStringList labels{"Nom", "Budget", "Pack", "Date debut", "Date fin", "Telephone"};
    ui->tableWidget->setHorizontalHeaderLabels(labels);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    Sponsor s;
    s.Setnom(ui->lineEdit_nom->text());
    s.Setbudget(ui->lineEdit_budget->text());
    s.Setpack(ui->lineEdit_pack->text());
    s.Setdate_deb(ui->dateEdit->text());
    s.Setdate_fin(ui->dateEdit_2->text());
    s.Settelephone(ui->lineEdit_tel->text());

    if (s.Getnom().isEmpty() || s.Getbudget().isEmpty() || s.Getpack().isEmpty() ||
            s.Getdate_deb().isEmpty() || s.Getdate_fin().isEmpty() || s.Gettelephone().isEmpty()) {
            QMessageBox::critical(this, "Error", "The fields must not be empty.");
            return; // Exit the function to prevent further execution
        }
        Connection c;
        if (c.createconnect())
        {
            if (c.insertData(s.Getnom(), s.Getbudget(), s.Getpack(), s.Getdate_deb(), s.Getdate_fin(), s.Gettelephone() ))
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

void MainWindow::on_pushButton_annuler_clicked()
{

       // Update the UI or clear the input fields if needed
       ui->lineEdit_nom->clear();
       ui->lineEdit_budget->clear();
       ui->lineEdit_pack->clear();
       ui->dateEdit->clear();
       ui->dateEdit_2->clear();
       ui->lineEdit_tel->clear();

}


void MainWindow::updateTableWidget()
{
    Connection c;
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SPONSOR");

    int RowNumber = 0;
    ui->tableWidget->setRowCount(0);

    while (model->canFetchMore())
        model->fetchMore();

    for (int row = 0; row < model->rowCount(); ++row)
    {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 6; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
        RowNumber++;
    }

    delete model; // Release the memory occupied by the model
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
         int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        int idToDelete = item->text().toInt();

        Sponsor s;
        if (s.supprimer(idToDelete)) {
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

void MainWindow::on_pushButton_modifier_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidget->item(row, 0)->text().toInt();

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidget->item(row, 0)->text();
        QString budget = ui->tableWidget->item(row, 1)->text();
        QString pack = ui->tableWidget->item(row, 2)->text();
        QString date_deb = ui->tableWidget->item(row, 3)->text();
        QString date_fin = ui->tableWidget->item(row, 4)->text();
        QString telephone = ui->tableWidget->item(row, 5)->text();

        // Appelez la fonction modifierInvite pour mettre à jour la base de données
        Sponsor s;
        if (s.modifier(idToModify, nom, budget, pack, date_deb, date_fin, telephone)) {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(budget));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(pack));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(date_deb));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(date_fin));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(telephone));

            emit dataUpdated(); // Mettre à jour la vue après la modification
            QMessageBox::information(this, "Success", "Data modified in the database.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to modify data in the database.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please select a row to modify.");
    }
}





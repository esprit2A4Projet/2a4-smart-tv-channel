#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transaction.h"
#include "connexion.h"
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QTableWidget>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget())); // Connect the signal to the slot

    // Initialize the table widget
    ui->tableWidget->setColumnCount(6);
    QStringList labels{"Mode de paiement", "Type", "Categorie", "Date de transaction", "montant","ID"};
    ui->tableWidget->setHorizontalHeaderLabels(labels);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::updateTableWidget()
{
    Connection c;
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM TRANSACTION");

        // Efface le contenu actuel du tableau
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0); // Réinitialise les lignes du tableau

        // Met à jour les données triées dans le tableau
        for (int row = 0; row < model->rowCount(); ++row)
        {
            // Insertion d'une nouvelle ligne dans le tableau
            ui->tableWidget->insertRow(row);

            // Ajout des données dans les cellules de la ligne
            for (int col = 0; col < 6; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
                ui->tableWidget->setItem(row, col, item);
            }
        }

        // Libère la mémoire occupée par le modèle
        delete model;
}


void MainWindow::on_pushButton_Ajout_clicked()
{
    // Récupérer les valeurs saisies
    QString modeDePaiement = ui->lineEdit_ModeDePaiement->currentText();
    QString type = ui->lineEdit_Type->currentText();
    QString categorie = ui->LineEdit_Categorie->currentText();
    QString dateTransaction = ui->LineEdit_Date->text();
    QString montant = ui->lineEdit_Montant->text();

    // Validation de la saisie du montant
    bool montantIsValid = false;
    QRegExp montantRegExp("^\\d+$"); // Expression régulière pour vérifier si le montant est composé uniquement de chiffres
    if (montantRegExp.exactMatch(montant)) {
        montantIsValid = true;
    }

    // Validation supplémentaire des autres champs si nécessaire

    if (!montantIsValid) {
        QMessageBox::critical(this, "Error", "Montant should contain only digits.");
        return; // Sortir de la fonction si le montant n'est pas valide
    }

    // Si tous les champs sont valides, créer une transaction et l'ajouter à la base de données
    Transaction i;
    i.set_modeDePaiement(modeDePaiement);
    i.set_type(type);
    i.set_categorie(categorie);
    i.set_dateTransaction(dateTransaction);
    i.set_montant(montant);

    Connection c;
    if (c.createconnect())
    {
        if (i.ajouterTransaction())
        {
            emit dataUpdated(); // Émettre le signal après l'insertion réussie
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




void MainWindow::on_pushButton_clicked() //modification
{
        QItemSelectionModel *select = ui->tableWidget->selectionModel();
        QModelIndexList selectedIndexes = select->selectedIndexes();

        if (!selectedIndexes.isEmpty()) {
            int row = selectedIndexes.first().row();
            int idToModify = ui->tableWidget->item(row, 5)->text().toInt(); // Récupérer l'ID de la transaction après le tri

            // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
            QString modeDePaiement = ui->tableWidget->item(row, 0)->text();
            QString type = ui->tableWidget->item(row, 1)->text();
            QString categorie = ui->tableWidget->item(row, 2)->text();
            QString dateTransaction = ui->tableWidget->item(row, 3)->text();
            QString montant = ui->tableWidget->item(row, 4)->text();

            // Appelez la fonction modifierTransaction pour mettre à jour la base de données
            Transaction i;
            if (i.modifierTransaction(idToModify, modeDePaiement, type, categorie, dateTransaction, montant))
            {
                // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(modeDePaiement));
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(type));
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(categorie));
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(dateTransaction));
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(montant));

                emit dataUpdated(); // Mettre à jour la vue après la modification
                QMessageBox::information(this, "Success", "Data modified in the database.");
            } else {
                QMessageBox::critical(this, "Error", "Failed to modify data in the database.");
            }
        } else {
            QMessageBox::warning(this, "Warning", "Please select a row to modify.");
        }
}

void MainWindow::on_pushButton_2_clicked() //suppression
{

        QItemSelectionModel *select = ui->tableWidget->selectionModel();
        QModelIndexList selectedIndexes = select->selectedIndexes();

        if (!selectedIndexes.isEmpty())
        {
            int row = selectedIndexes.first().row();
            QTableWidgetItem *item = ui->tableWidget->item(row, 5); // Vérifiez la colonne contenant l'ID après le tri
            if (item) // Vérifiez si l'élément existe
            {
                int idToDelete = item->text().toInt();

                Transaction i;
                QMessageBox::information(this, "ID to delete", "ID to delete: " + QString::number(idToDelete));
                if (i.supprimerTransaction(idToDelete))
                {
                    // Supprimer la ligne de l'affichage
                    ui->tableWidget->removeRow(row);
                    emit dataUpdated(); // Mettre à jour la vue après la suppression
                    QMessageBox::information(this, "Success", "Data deleted from the database.");
                }
                else
                {
                    QMessageBox::critical(this, "Error", "Failed to delete data from the database.");
                }
            }
            else
            {
                QMessageBox::warning(this, "Warning", "Selected row does not contain ID.");
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Please select a row to delete.");
        }
}

void MainWindow::on_Button_trierParDate_clicked() //tri par date
{
        ui->tableWidget->clearContents(); // Efface le contenu actuel du tableau
        ui->tableWidget->setRowCount(0); // Efface également les lignes existantes du tableau
        // Trier les transactions par date
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("Source_Projet2A4");
        db.setUserName("system");
        db.setPassword("Islemsallouma1");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM TRANSACTION");
        if (query.exec(str))
        {
            ui->tableWidget->setColumnCount(6);
            QStringList labels;
            labels << "Mode de paiement" << "Type" << "Categorie" << "Date de transaction" << "Montant" << "ID";
            ui->tableWidget->setHorizontalHeaderLabels(labels);
            int row = 0;
            QVector<QPair<QDate, QStringList>> sortedTransactions; // Pour stocker les transactions triées

            while (query.next())
            {
                QStringList transactionData;
                transactionData << query.value(0).toString() << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString();
                QDate date = QDate::fromString(query.value(3).toString(), "dd/MM/yyyy"); // Convertir la chaîne de date en QDate
                sortedTransactions.append(qMakePair(date, transactionData));
            }

            // Tri des transactions par date
            std::sort(sortedTransactions.begin(), sortedTransactions.end(), [](const QPair<QDate, QStringList> &a, const QPair<QDate, QStringList> &b) {
                return a.first < b.first;
            });

            // Affichage des transactions triées dans le tableau
            for (const auto &transaction : sortedTransactions)
            {
                ui->tableWidget->insertRow(row);
                for (int col = 0; col < 6; ++col)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(transaction.second[col]);
                    ui->tableWidget->setItem(row, col, item);
                }
                row++;
            }
        }
}



void MainWindow::on_pushButton_exportationFormatExcel_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save CSV File", QDir::homePath(), "CSV files (*.csv)");

        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);

                // Écrire les données
                for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
                    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
                        QTableWidgetItem *item = ui->tableWidget->item(row, col);
                        if (item)
                            out << item->text() << "\t"; // Utilise une tabulation pour séparer les valeurs dans une ligne
                    }
                    out << "\n"; // Passe à la ligne suivante après avoir écrit toutes les valeurs de la ligne actuelle
                }

                QMessageBox::information(this, "Export Successful", "Data exported successfully to CSV file.");
            } else {
                QMessageBox::critical(this, "Export Error", "Failed to open the file for writing.");
            }
        }
}

void MainWindow::on_Button_filtrer_2_clicked() //tri par montant
{
    ui->tableWidget->clearContents(); // Efface le contenu actuel du tableau
        ui->tableWidget->setRowCount(0); // Efface également les lignes existantes du tableau
        // Trier les transactions par montant
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("Source_Projet2A4");
        db.setUserName("system");
        db.setPassword("Islemsallouma1");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM TRANSACTION");
        if (query.exec(str))
        {
            ui->tableWidget->setColumnCount(6);
            QStringList labels;
            labels << "Mode de paiement" << "Type" << "Categorie" << "Date de transaction" << "Montant" << "ID";
            ui->tableWidget->setHorizontalHeaderLabels(labels);
            int row = 0;
            QVector<QPair<double, QStringList>> sortedTransactions; // Pour stocker les transactions triées par montant

            while (query.next())
            {
                QStringList transactionData;
                transactionData << query.value(0).toString() << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString();
                double montant = query.value(4).toDouble(); // Obtenez le montant en double
                sortedTransactions.append(qMakePair(montant, transactionData));
            }

            // Tri des transactions par montant
            std::sort(sortedTransactions.begin(), sortedTransactions.end(), [](const QPair<double, QStringList> &a, const QPair<double, QStringList> &b) {
                return a.first < b.first;
            });

            // Affichage des transactions triées dans le tableau
            for (const auto &transaction : sortedTransactions)
            {
                ui->tableWidget->insertRow(row);
                for (int col = 0; col < 6; ++col)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(transaction.second[col]);
                    ui->tableWidget->setItem(row, col, item);
                }
                row++;
            }
        }

}

void MainWindow::on_Button_chercherParID_clicked()
{
    QString idToSearch = ui->lineEdit_RechercheParID->text(); // Récupère l'ID à rechercher depuis le lineEdit

        bool found = false; // Indique si l'ID a été trouvé

        // Parcours des lignes du tableau pour trouver l'ID correspondant
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QTableWidgetItem *item = ui->tableWidget->item(row, 5); // Récupère l'item de la colonne d'ID (index 5)
            if (item && item->text() == idToSearch) { // Vérifie si l'item existe et s'il correspond à l'ID recherché
                ui->tableWidget->selectRow(row); // Sélectionne la ligne correspondante dans le tableau
                found = true; // Marque l'ID comme trouvé
                break; // Sort de la boucle car l'ID a été trouvé
            }
        }

        // Si l'ID n'a pas été trouvé, affiche un message d'erreur
        if (!found) {
            QMessageBox::critical(this, "Erreur", "L'ID spécifié n'existe pas dans la table.");
        }
}

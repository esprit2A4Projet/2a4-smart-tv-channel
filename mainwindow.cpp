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
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QStringListModel>
#include <QListView>
#include <QStyledItemDelegate>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        //-----------------------------------installation des reflexes selon les signaux--------------------------------//
        ui->lineEdit_Montant_Transaction->installEventFilter(this);
        installEventFilter(this);
        connect(ui->table_de_transactions, &QTableWidget::cellDoubleClicked, this, &MainWindow::preventCellEdit_Transactions);
        connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::showEditChoices_Transactions);
        connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::handleCellClicked_Transactions);
        connect(ui->lineEdit_RechercheParMONTANT_Transaction, &QLineEdit::textChanged, this, &MainWindow::searchTransactionsByAmount_Transaction);
        connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::editMontantCell_Transactions);
        connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget()));
        ui->listView->hide();
        connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::afficherListeNotifications);
        //---------------------------------------------------------------------------------------------------------------//







//--------------------------------Initialisation du tableau et ses champs----------------------------------------------//
        ui->table_de_transactions->setColumnCount(6);
        QStringList labels{"Mode de paiement", "Type", "Categorie", "Date de transaction", "Montant en dinar","ID"};
        ui->table_de_transactions->setHorizontalHeaderLabels(labels);
//---------------------------------------------------------------------------------------------------------------------//
    }

//--------------------------------------------DESTRUCTEUR---------------------------------------------------------//
MainWindow::~MainWindow()
{
    delete ui;
}
//----------------------------------------------------------------------------------------------------------------//



//-----------------------------Fonction pour mettre le tableau a jour apres les operations------------------------//
void MainWindow::update_table_de_transactions()
{
        Connection c;
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM TRANSACTIONS");

        // Efface le contenu actuel du tableau
        ui->table_de_transactions->clearContents();
        ui->table_de_transactions->setRowCount(0); // Réinitialise les lignes du tableau

        // Met à jour les données triées dans le tableau
        for (int row = 0; row < model->rowCount(); ++row)
        {
            // Insertion d'une nouvelle ligne dans le tableau
            ui->table_de_transactions->insertRow(row);

            // Ajout des données dans les cellules de la ligne
            for (int col = 0; col < 6; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
                ui->table_de_transactions->setItem(row, col, item);
            }
        }

        // Libère la mémoire occupée par le modèle
        delete model;
}
//-----------------------------------------------------------------------------------------------------------------//


//-----------------------------------FONCTION D'AJOUT--------------------------------------------------------------//
void MainWindow::on_pushButton_Ajout_Transaction_clicked()
{
        // Récupérer les valeurs saisies
        QString modeDePaiement = ui->lineEdit_ModeDePaiement_Transaction->currentText();
        QString type = ui->lineEdit_Type_Transaction->currentText();
        QString categorie = ui->LineEdit_Categorie_Transaction->currentText();
        QString dateTransaction = ui->LineEdit_Date_Transaction->text();
        QString montant = ui->lineEdit_Montant_Transaction->text();
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
                update_table_de_transactions(); // Émettre le signal après l'insertion réussie
                QMessageBox::information(this, "Succès", "Données insérées dans la base de données.");
            }
            else
            {
                QMessageBox::critical(this, "Erreur", "Échec de l'insertion des données dans la base de données.");
            }
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec de la connexion à la base de données.");
        }
}
//---------------------------------------------------------------------------------------------------------------//


//-------------------------------FONCTION DE MODIFICATION--------------------------------------------------------//
void MainWindow::on_pushButton_Modification_Transaction_clicked()
{
    QItemSelectionModel *select = ui->table_de_transactions->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->table_de_transactions->item(row, 5)->text().toInt(); // Récupérer l'ID de la transaction après le tri

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString modeDePaiement = ui->table_de_transactions->item(row, 0)->text();
        QString type = ui->table_de_transactions->item(row, 1)->text();
        QString categorie = ui->table_de_transactions->item(row, 2)->text();
        QString dateTransaction = ui->table_de_transactions->item(row, 3)->text();
        QString montant = ui->table_de_transactions->item(row, 4)->text();

        // Appelez la fonction modifierTransaction pour mettre à jour la base de données
        Transaction i;
        if (i.modifierTransaction(idToModify, modeDePaiement, type, categorie, dateTransaction, montant))
        {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->table_de_transactions->setItem(row, 0, new QTableWidgetItem(modeDePaiement));
            ui->table_de_transactions->setItem(row, 1, new QTableWidgetItem(type));
            ui->table_de_transactions->setItem(row, 2, new QTableWidgetItem(categorie));
            ui->table_de_transactions->setItem(row, 3, new QTableWidgetItem(dateTransaction));
            ui->table_de_transactions->setItem(row, 4, new QTableWidgetItem(montant));

            emit signal_table_de_transaction_mise_a_jour(); // Mettre à jour la vue après la modification
            ui->champ_resultat_CRUD_Transaction->setText("Données modifiées avec succés.");
        } else {
            ui->champ_resultat_CRUD_Transaction->setText("Echec de la modification !");
        }
    } else {
        ui->champ_resultat_CRUD_Transaction->setText("Veuillez selectionner une case à modifier !");
    }
    for (int r = 0; r < ui->table_de_transactions->rowCount(); ++r)
    {
        for (int c = 0; c < ui->table_de_transactions->columnCount(); ++c)
        {
            QWidget *cellWidget = ui->table_de_transactions->cellWidget(r, c);
            if (cellWidget) {
                cellWidget->deleteLater();
            }
        }
    }
    update_table_de_transactions();
}

//-------------------------------------------------------------------------------------------------------------//


//----------------------------------------FONCTION DE SUPPRESSION----------------------------------------------//
void MainWindow::on_pushButton_Suppression_Transaction_clicked()
{
    QItemSelectionModel *select = ui->table_de_transactions->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty())
    {
        int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->table_de_transactions->item(row, 5); // Vérifiez la colonne contenant l'ID après le tri
        if (item) // Vérifiez si l'élément existe
        {
            int idToDelete = item->text().toInt();

            Transaction i;
            if (i.supprimerTransaction(idToDelete))
            {
                // Supprimer les widgets de cellule associés à cette ligne
                for (int c = 0; c < ui->table_de_transactions->columnCount(); ++c) {
                    QWidget *cellWidget = ui->table_de_transactions->cellWidget(row, c);
                    if (cellWidget) {
                        cellWidget->deleteLater();
                    }
                }

                // Supprimer la ligne de l'affichage
                ui->table_de_transactions->removeRow(row);
                emit signal_table_de_transaction_mise_a_jour(); // Mettre à jour la vue après la suppression
                ui->champ_resultat_CRUD_Transaction->setText("Donnée supprimée avec succès.");
            }
            else
            {
                ui->champ_resultat_CRUD_Transaction->setText("Echec de la suppression !");
            }
        }
        else
        {
            ui->champ_resultat_CRUD_Transaction->setText("Selected row does not contain ID.");
        }
    }
    else
    {
        ui->champ_resultat_CRUD_Transaction->setText("Veuillez sélectionner une ligne à supprimer !");
    }
}

//-------------------------------------------------------------------------------------------------------------//


//--------------------------------------FONCTION DE TRI PAR DATE-----------------------------------------------//
void MainWindow::on_Button_trierParDate_Transaction_clicked()
{
        ui->table_de_transactions->clearContents(); // Efface le contenu actuel du tableau
        ui->table_de_transactions->setRowCount(0); // Efface également les lignes existantes du tableau
        // Trier les transactions par date
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("Source_Projet2A4");
        db.setUserName("louayzorai");
        db.setPassword("louayzorai");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM TRANSACTIONS");
        if (query.exec(str))
        {
            ui->table_de_transactions->setColumnCount(6);
            QStringList labels;
            labels << "Mode de paiement" << "Type" << "Categorie" << "Date de transaction" << "Montant" << "ID";
            ui->table_de_transactions->setHorizontalHeaderLabels(labels);
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
                ui->table_de_transactions->insertRow(row);
                for (int col = 0; col < 6; ++col)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(transaction.second[col]);
                    ui->table_de_transactions->setItem(row, col, item);
                }
                row++;
            }
        }
}
//----------------------------------------------------------------------------------------------------------------------------------------------//


//-------------------------------------FONCTION D'EXPORTATION EXCEL-----------------------------------------------------------------------------//
void MainWindow::on_pushButton_exportationFormatExcel_Transaction_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save CSV File", QDir::homePath(), "CSV files (*.csv)");

        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);

                // Écrire les données
                for (int row = 0; row < ui->table_de_transactions->rowCount(); ++row) {
                    for (int col = 0; col < ui->table_de_transactions->columnCount(); ++col) {
                        QTableWidgetItem *item = ui->table_de_transactions->item(row, col);
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
//--------------------------------------------------------------------------------------------------------------------------------------//



//------------------------------------------FONCTION TRI PAR MONTANT--------------------------------------------------------------------//
void MainWindow::on_Button_TrierParMontant_Transaction_clicked() // tri par montant
{
    ui->table_de_transactions->clearContents(); // Efface le contenu actuel du tableau
    ui->table_de_transactions->setRowCount(0); // Efface également les lignes existantes du tableau
    // Trier les transactions par montant
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A4");
    db.setUserName("louayzorai");
    db.setPassword("louayzorai");
    db.open();
    QSqlQuery query(db);
    QString str = ("SELECT * FROM TRANSACTIONS");
    if (query.exec(str))
    {
        ui->table_de_transactions->setColumnCount(6);
        QStringList labels;
        labels << "Mode de paiement" << "Type" << "Categorie" << "Date de transaction" << "Montant" << "ID";
        ui->table_de_transactions->setHorizontalHeaderLabels(labels);
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
            ui->table_de_transactions->insertRow(row);
            for (int col = 0; col < 6; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(transaction.second[col]);
                ui->table_de_transactions->setItem(row, col, item);
            }
            row++;
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------//


//---------------------------------------------------FONCTION DE RECHERCHE PAR MONTANT---------------------------------------------------------//
void MainWindow::searchTransactionsByAmount_Transaction(const QString &amount)
{
    // Valider la saisie du montant
    bool amountIsValid = true;
    QString errorMessage;

    // Vérifier si la saisie est invalide
    QRegExp amountRegExp("^(\\d+\\.?\\d*|\\.\\d+)$");
    if (!amountRegExp.exactMatch(amount) && !amount.isEmpty()) {
        amountIsValid = false;
        errorMessage = "Montant invalide. Veuillez saisir uniquement des chiffres et un point.";
    }

    // Afficher un message d'erreur si le montant n'est pas valide
    if (!amountIsValid) {
        ui->champ_erreur_rechercheParMontant->setText(errorMessage);
        return;
    } else {
        ui->champ_erreur_rechercheParMontant->clear();
    }

    // Filtrer les transactions en fonction du montant valide
    QString filterAmount = amount.isEmpty() ? "" : amount;
    for (int row = 0; row < ui->table_de_transactions->rowCount(); ++row) {
        QString currentAmount = ui->table_de_transactions->item(row, 4)->text();
        bool showRow = currentAmount.startsWith(filterAmount, Qt::CaseInsensitive);

        // Ne pas appliquer le filtre si le montant n'est pas valide
        if (!amountIsValid) {
            showRow = true;
        }

        ui->table_de_transactions->setRowHidden(row, !showRow);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------FONCTION BOUTON ANNULER--------------------------------------------------------------------//
void MainWindow::on_pushButton_Annuler_Transaction_clicked()
{
       ui->lineEdit_Montant_Transaction->clear();
}
//----------------------------------------------------------------------------------------------------------------------------------------//



//-----------------------LES FONCTIONS DE CONTROLE DE SAISIE + GERER LES EVENEMENTS-------------------------------------------------------//
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
        if (obj == ui->lineEdit_Montant_Transaction && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease))
        {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                QString text = ui->lineEdit_Montant_Transaction->text();

                // Seuls les chiffres, un point et la touche "Backspace" sont autorisés
                if (!(keyEvent->text().isEmpty() && keyEvent->key() != Qt::Key_Backspace) && !(keyEvent->text().at(0).isDigit() || keyEvent->text() == "." || keyEvent->key() == Qt::Key_Backspace))
                {
                    ui->champs_erreur_ajout_Transaction->setText("Le montant doit contenir uniquement\ndes chiffres et un point !");
                    return true;
                }

                // On autorise un seul point décimal
                if (keyEvent->text() == "." && text.contains("."))
                {
                    ui->champs_erreur_ajout_Transaction->setText("Vous ne pouvez entrer qu'un\nseul point décimal !");
                    return true;
                }

                // Si la saisie est valide, effacez le contenu du QLabel
                if (text.isEmpty() || (text.toDouble() != 0.0 && text.toDouble() != 0.0))
                {
                    ui->champs_erreur_ajout_Transaction->clear(); // Efface le contenu du QLabel
                }
        }
        if (event->type() == QEvent::MouseButtonPress)
        {
                // Si l'événement est un clic de souris, effacez le contenu du champ label_10
                ui->champ_resultat_CRUD_Transaction->clear();
                // Si l'événement est un clic de souris, effacez les champs de modification du tableau
                for (int r = 0; r < ui->table_de_transactions->rowCount(); ++r)
                {
                    for (int c = 0; c < ui->table_de_transactions->columnCount(); ++c)
                    {
                        QWidget *cellWidget = ui->table_de_transactions->cellWidget(r, c);
                        if (cellWidget) {
                            cellWidget->deleteLater();
                        }
                    }
                }
                        ui->listView->hide();
        }
        return QObject::eventFilter(obj, event);
}

void MainWindow::preventCellEdit_Transactions()
{
    ui->table_de_transactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::showEditChoices_Transactions(int row, int column)
{
        if (column == 0)
        {
                QStringList choices = {"Virement", "Cheques", "Especes"};
                showChoicesUnderCell_Transactions(row, column, choices);
        }
        else if (column == 1)
        {
                QStringList choices = {"Depense", "Revenue"};
                showChoicesUnderCell_Transactions(row, column, choices);
        }
        else if (column == 2)
        {
                QStringList choices = {"Payement employe", "Payement facture", "Achat materiel", "Vente materiel", "Autres depenses", "Autres revenues"};
                showChoicesUnderCell_Transactions(row, column, choices);
        }
}

void MainWindow::showChoicesUnderCell_Transactions(int row, int column, const QStringList &choices)
{
    // Sauvegarder la modification actuelle dans le tableau avant de supprimer les combobox précédents
    QWidget *previousWidget = ui->table_de_transactions->cellWidget(row, column);
    if (previousWidget)
    {
        QComboBox *previousComboBox = previousWidget->findChild<QComboBox*>();
        if (previousComboBox)
        {
            QString newText = previousComboBox->currentText();
            ui->table_de_transactions->item(row, column)->setText(newText);
        } else
        {
            QLineEdit *previousLineEdit = previousWidget->findChild<QLineEdit*>();
            if (previousLineEdit) {
                QString newText = previousLineEdit->text();
                ui->table_de_transactions->item(row, column)->setText(newText);
            }
        }
    }

    // Supprimer tous les QComboBox et les lignes de saisie de date présents dans le tableau
    for (int r = 0; r < ui->table_de_transactions->rowCount(); ++r)
    {
        for (int c = 0; c < ui->table_de_transactions->columnCount(); ++c)
        {
            QWidget *cellWidget = ui->table_de_transactions->cellWidget(r, c);
            if (cellWidget) {
                cellWidget->deleteLater();
            }
        }
    }

    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    if (column == 3) { // Si c'est la colonne de la date
        QDateEdit *dateEdit = new QDateEdit();
        dateEdit->setDisplayFormat("dd/MM/yyyy"); // Format de la date
        layout->addWidget(dateEdit);
        layout->setAlignment(Qt::AlignTop);

        ui->table_de_transactions->setCellWidget(row, column, widget);

        connect(dateEdit, &QDateEdit::dateChanged, [=](const QDate &date) {
            // Mettre à jour la valeur de la cellule dans le QTableWidget avec la valeur de la date formatée
            QString formattedDate = date.toString("dd/MM/yyyy");
            ui->table_de_transactions->item(row, column)->setText(formattedDate);
        });
    } else { // Pour toutes les autres colonnes
        QComboBox *newComboBox = new QComboBox();
        newComboBox->addItems(choices);
        newComboBox->setCurrentIndex(-1); // Définir la sélection initiale sur -1

        layout->addWidget(newComboBox);
        layout->setAlignment(Qt::AlignTop);

        ui->table_de_transactions->setCellWidget(row, column, widget);

        connect(newComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=]() {
            // Mettre à jour la valeur de la cellule dans le QTableWidget avec la valeur sélectionnée dans le QComboBox
            QString newText = newComboBox->currentText();
            ui->table_de_transactions->item(row, column)->setText(newText);
        });
    }
}


void MainWindow::handleCellClicked_Transactions(int row, int column)
{
    if (column == 3) { // Si la colonne est la colonne de la date
        QStringList choices; // Vous pouvez remplir cette liste avec des choix de date si nécessaire
        showChoicesUnderCell_Transactions(row, column, choices);
    }
}
void MainWindow::editMontantCell_Transactions(int row, int column)
{
    if (column == 4)
    { // Vérifier si la colonne est la colonne "montant"
        QLineEdit *lineEdit = new QLineEdit(ui->table_de_transactions);
        QString text = ui->table_de_transactions->item(row, column)->text();
        lineEdit->setText(text);
        ui->table_de_transactions->setCellWidget(row, column, lineEdit);
        lineEdit->setFocus();
        connect(lineEdit, &QLineEdit::textChanged, [=](const QString &newText)
        {
            QRegExp regExp("^\\d*\\.?\\d*$"); // Expression régulière pour vérifier la validité du montant
            if (!regExp.exactMatch(newText)) {
                // Si la saisie est invalide, effacer le contenu du line edit
                lineEdit->setText(text);
            }
        });
        connect(lineEdit, &QLineEdit::editingFinished, [=]()
        {
            // Mettre à jour la cellule du tableau avec le nouveau montant
            QString newText = lineEdit->text();
            QTableWidgetItem *item = new QTableWidgetItem(newText);
            ui->table_de_transactions->setItem(row, column, item);
            // Supprimer le line edit après l'édition
            ui->table_de_transactions->removeCellWidget(row, column);
        });
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------//


//---------------------------------------------------------LES NOTIFICATIONS-------------------------------------------------------------------//

void MainWindow::afficherListeNotifications()
{
        // Connexion à la base de données
        Connection c;
        if (!c.createconnect()) {
            QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à la base de données.");
            return;
        }

        // Récupération des données depuis la table NOTIFICATIONS_TRANSACTION
        QSqlQuery query;
        if (!query.exec("SELECT MESSAGE_NOTIFICATION FROM NOTIFICATIONS_TRANSACTION")) {
            QMessageBox::critical(this, "Erreur de requête", "Erreur lors de l'exécution de la requête.");
            return;
        }

        // Création d'une liste pour stocker les notifications
        QStringList notificationsList;

        // Parcours des résultats de la requête et ajout des messages de notification à la liste
        while (query.next()) {
            QString message = query.value(0).toString();
            notificationsList << message;
        }

        // Création du modèle de liste avec les données récupérées
        QStringListModel *model = new QStringListModel(notificationsList, this);

        // Mise à jour du modèle de données de la listView
        ui->listView->setModel(model);
        ui->listView->show();
}







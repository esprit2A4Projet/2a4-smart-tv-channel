#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transaction.h"
#include "connexion.h"
#include "statistique_finance.h"
#include <QtWidgets>
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
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QPixmap>
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include <QImage>
#include <QBuffer>
#include <QFile>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        updateCategoryOptions();
        envoyerNofiticatiionDepassementRevenuesEtDepenses();
        AfficherEtmettreAJourPieChartTransaction();
        //-----------------------------------installation des reflexes selon les signaux--------------------------------//
        ui->lineEdit_Montant_Transaction->installEventFilter(this);
        installEventFilter(this);
        connect(ui->table_de_transactions, &QTableWidget::cellDoubleClicked, this, &MainWindow::preventCellEdit_Transactions);
        connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::showEditChoices_Transactions);
        connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::handleCellClicked_Transactions);
        connect(ui->lineEdit_Type_Transaction, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCategoryOptions);
        connect(ui->lineEdit_RechercheParMONTANT_Transaction, &QLineEdit::textChanged, this, &MainWindow::searchTransactionsByAmount_Transaction);
        connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::editMontantCell_Transactions);
        connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget()));
        //---------------------------------------------------------------------------------------------------------------//







//--------------------------------Initialisation du tableau et ses champs----------------------------------------------//
        ui->table_de_transactions->setColumnCount(6);
        QStringList labels{"Mode de paiement", "Type", "Categorie", "Date de transaction", "Montant en dinar","ID"};
        ui->table_de_transactions->setHorizontalHeaderLabels(labels);




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
        envoyerNofiticatiionDepassementRevenuesEtDepenses();
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
                ui->champ_resultat_CRUD_Transaction->setText("Ajout avec succées !");
                AfficherEtmettreAJourPieChartTransaction();

            }
            else
            {
                ui->champ_resultat_CRUD_Transaction->setText("Echec de l'ajout !");
            }
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
            AfficherEtmettreAJourPieChartTransaction();

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
                AfficherEtmettreAJourPieChartTransaction();
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
    // Créer un objet Workbook
    QXlsx::Document xlsx;

    // Obtenir le nombre de lignes et de colonnes du QTableWidget
    int rowCount = ui->table_de_transactions->rowCount();
    int columnCount = ui->table_de_transactions->columnCount();

    // Ajouter le titre "TABLE DES TRANSACTIONS" sur 4 lignes et 8 colonnes
    QColor couleurCellule(0, 32, 96);
    xlsx.mergeCells(QXlsx::CellRange(1, 1, 4, 24));
    QXlsx::Format titleFormat; // Déclaration de la variable titleFormat
    titleFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    titleFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    titleFormat.setFontBold(true);
    titleFormat.setFontSize(36);
    titleFormat.setFontColor(Qt::white);
    titleFormat.setPatternBackgroundColor(couleurCellule);
    xlsx.write(1, 1, "TABLE DES TRANSACTIONS", titleFormat);

    // Sauter 2 lignes après le titre
    xlsx.write(6, 1, ""); // Écrire une cellule vide à la ligne 6

    // Ajouter les noms de colonnes
    QXlsx::Format nomDeColonneFormat; // Déclaration de la variable titleFormat
    nomDeColonneFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    nomDeColonneFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    nomDeColonneFormat.setFontBold(true);
    nomDeColonneFormat.setFontSize(24);
    nomDeColonneFormat.setFontColor(Qt::white);
    QColor couleurCelluleNomColonne(221, 171, 213);
    nomDeColonneFormat.setPatternBackgroundColor(couleurCelluleNomColonne);

    for (int column = 0; column < columnCount; ++column) {
        QTableWidgetItem *headerItem = ui->table_de_transactions->horizontalHeaderItem(column);
        if (headerItem)
        {
            xlsx.write(8, column * 4 + 1, headerItem->text(),nomDeColonneFormat);
            xlsx.mergeCells(QXlsx::CellRange(8, column * 4 + 1, 11, column * 4 + 4)); // Étaler sur 4 colonnes et 4 lignes

            // Appliquer le format pour les noms de colonnes
            QXlsx::Format columnNameFormat;
            columnNameFormat.setFontBold(true);
            columnNameFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            columnNameFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
            columnNameFormat.setFontSize(24);
            xlsx.cellAt(8, column * 4 + 1)->format().setFont(columnNameFormat.font());
            xlsx.cellAt(8, column * 4 + 1)->format().setHorizontalAlignment(columnNameFormat.horizontalAlignment());
            xlsx.cellAt(8, column * 4 + 1)->format().setVerticalAlignment(columnNameFormat.verticalAlignment());
            xlsx.cellAt(8, column * 4 + 1)->format().setFontSize(columnNameFormat.fontSize());
        }
    }

    // Parcourir le QTableWidget pour copier les données dans le document Excel
    QXlsx::Format formatCelluleNormale; // Déclaration de la variable titleFormat
    formatCelluleNormale.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    formatCelluleNormale.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    formatCelluleNormale.setFontSize(16);
    formatCelluleNormale.setFontColor(Qt::white);
    formatCelluleNormale.setPatternBackgroundColor(couleurCellule);
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            // Obtenir le texte de la cellule dans le QTableWidget
            QTableWidgetItem *item = ui->table_de_transactions->item(row, column);
            if (item) {
                // Insérer le texte de la cellule dans le document Excel
                xlsx.write(row + 12, column * 4 + 1, item->text(),formatCelluleNormale);
                xlsx.mergeCells(QXlsx::CellRange(row + 12, column * 4 + 1, row + 13, column * 4 + 4)); // Étaler sur 4 colonnes et 2 lignes
            }
        }
    }
    // Chemin vers votre image
    QString imagePath = "C:/Users/user/Desktop/ESPRIT/2A4/2eme periode/projet/Module_Finance/logo pour excel.png";

    // Charger l'image à partir du chemin
    QImage image(imagePath);

    // Insérer l'image dans le fichier Excel
    xlsx.insertImage(0, 0, image);
    // Sauvegarder le document Excel
    QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter en Excel"), "", tr("Fichiers Excel (*.xlsx)"));
    if (!filePath.isEmpty()) {
        xlsx.saveAs(filePath);
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
        }
        return QObject::eventFilter(obj, event);
}

void MainWindow::preventCellEdit_Transactions()
{
    ui->table_de_transactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::showEditChoices_Transactions(int row, int column)
{
        if (column == 0)
        {
                QStringList choices = {"Virement", "Cheques", "Especes"};
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
void MainWindow::updateCategoryOptions()
{
    QString transactionType = ui->lineEdit_Type_Transaction->currentText();
    ui->LineEdit_Categorie_Transaction->clear(); // Effacer les choix précédents

    if (transactionType == "Depense") {
        // Ajouter les options pour les dépenses
        ui->LineEdit_Categorie_Transaction->addItem("Payement employe");
        ui->LineEdit_Categorie_Transaction->addItem("Payement facture");
        ui->LineEdit_Categorie_Transaction->addItem("Location local");
        ui->LineEdit_Categorie_Transaction->addItem("Achat materiel");
        ui->LineEdit_Categorie_Transaction->addItem("Autres depenses");
    } else if (transactionType == "Revenue") {
        // Ajouter les options pour les revenus
        ui->LineEdit_Categorie_Transaction->addItem("Vente materiel");
        ui->LineEdit_Categorie_Transaction->addItem("Autres revenues");
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------//



//NOTIFICATIONS


void MainWindow::envoyerNotificationTransaction(QString a, QString b)
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon("C:/Users/user/Desktop/ESPRIT/2A4/2eme periode/projet/Module_Finance/logo 2.png"));
    trayIcon->setToolTip("Tooltip de l'icône de plateau");
    trayIcon->show();
    trayIcon->showMessage(a, b, QSystemTrayIcon::Information, 3000);
}
//-----------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::envoyerNofiticatiionDepassementRevenuesEtDepenses()
{
    QSqlQuery query1, query2;
    query1.prepare("SELECT SUM (MONTANT) AS total_depenses FROM TRANSACTIONS WHERE TYPE = :type");
    query1.bindValue(":type", "Depense");
    query1.exec();
    double diff = 0.0;
    if (query1.next())
    {
        double sommeDepenses = query1.value("total_depenses").toDouble();
        query2.prepare("SELECT SUM (MONTANT) AS total_revenues FROM TRANSACTIONS WHERE TYPE = :type");
        query2.bindValue(":type", "Revenue");
        query2.exec();
        if (query2.next())
        {
            double sommeRevenues = query2.value("total_revenues").toDouble();
            double difference = sommeRevenues - sommeDepenses;
            diff = difference;
        }
    }
    if (diff < 0)
    {
        envoyerNotificationTransaction("ATTENTION","Les dépenses ont depassé les revenues");
    }
}


//----------------------------------------STATS-------------------------------------------------------------//
void MainWindow::AfficherEtmettreAJourPieChartTransaction()
{
    Connection c;
    c.createconnect();
    float percentPayementEmploye = 0.0;
    float percentPayementFacture = 0.0;
    float percentLocationLocal = 0.0;
    float percentAchatMateriel = 0.0;
    float percentAutresDepenses = 0.0;
    QSqlQuery sumQuery;
    float totalAmount = 0.0;
    if (sumQuery.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE type = 'Depense'"))
    {
        if (sumQuery.next()) {
            totalAmount = sumQuery.value(0).toFloat();
        }
    }
    if (totalAmount == 0.0)
    {
           QPieSeries *emptySeries = new QPieSeries();
           emptySeries->append("Aucune dépense", 0);
           QChart *chart = new QChart();
           chart->addSeries(emptySeries);
           chart->setAnimationOptions(QChart::SeriesAnimations);
           chart->setTitle("Statistiques des dépenses selon la categorie");
           chart->setTheme(QChart::ChartThemeBlueCerulean);
           QChartView *chartview = new QChartView(chart);
           chartview->setRenderHint(QPainter::Antialiasing);
           chartview->setParent(ui->statsTransactions);
           chartview->setGeometry(chartview->x(), chartview->y(), 1000, 400); // Ajustez la largeur et la hauteur selon vos besoins
           return;
    }
    QSqlQuery query;
    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Payement employe'"))
    {
        query.next();
        float totalPayementEmploye = query.value(0).toFloat();
        percentPayementEmploye = (totalPayementEmploye / totalAmount) * 100.0;
    }
    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Payement facture'"))
    {
        query.next();
        float totalPayementFacture = query.value(0).toFloat();
        percentPayementFacture = (totalPayementFacture / totalAmount) * 100.0;
    }
    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Location local'"))
    {
        query.next();
        float totalLocationLocal = query.value(0).toFloat();
        percentLocationLocal = (totalLocationLocal / totalAmount) * 100.0;
    }

    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Achat materiel'"))
    {
        query.next();
        float totalAchatMateriel = query.value(0).toFloat();
        percentAchatMateriel = (totalAchatMateriel / totalAmount) * 100.0;
    }

    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Autres depenses'"))
    {
        query.next();
        float totalAutresDepenses = query.value(0).toFloat();
        percentAutresDepenses = (totalAutresDepenses / totalAmount) * 100.0;
    }
    QString labelPayementEmploye = QString("Payement employe %1%").arg(QString::number(percentPayementEmploye, 'f', 2));
    QString labelPayementFacture = QString("Payement facture %1%").arg(QString::number(percentPayementFacture, 'f', 2));
    QString labelLocationLocal = QString("Location local %1%").arg(QString::number(percentLocationLocal, 'f', 2));
    QString labelAchatMateriel = QString("Achat materiel %1%").arg(QString::number(percentAchatMateriel, 'f', 2));
    QString labelAutresDepenses = QString("Autres depenses %1%").arg(QString::number(percentAutresDepenses, 'f', 2));
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.3);
    series->append(labelPayementEmploye, percentPayementEmploye);
    series->append(labelPayementFacture, percentPayementFacture);
    series->append(labelLocationLocal, percentLocationLocal);
    series->append(labelAchatMateriel, percentAchatMateriel);
    series->append(labelAutresDepenses, percentAutresDepenses);
    if (percentPayementEmploye != 0.0) {
        QPieSlice *slice1 = series->slices().at(0);
        slice1->setLabelVisible();
    }
    if (percentPayementFacture != 0.0)
    {
        QPieSlice *slice2 = series->slices().at(1);
        slice2->setLabelVisible();
    }
    if (percentLocationLocal != 0.0)
    {
        QPieSlice *slice3 = series->slices().at(2);
        slice3->setLabelVisible();
    }
    if (percentAchatMateriel != 0.0)
    {
        QPieSlice *slice4 = series->slices().at(3);
        slice4->setLabelVisible();
    }
    if (percentAutresDepenses != 0.0)
    {
        QPieSlice *slice5 = series->slices().at(4);
        slice5->setLabelVisible();
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Statistiques des dépenses selon la categorie");
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setParent(ui->statsTransactions);
    chartview->setGeometry(chartview->x(), chartview->y(), 1000, 400);
    QChartView *chartview2 = new QChartView(chart);
    chartview2->setRenderHint(QPainter::Antialiasing);
    chartview2->setParent(ui->statsTransactions);
    chartview2->setGeometry(chartview2->x(), chartview2->y(), 1000, 400);
}











void MainWindow::on_faceIDButton_clicked()
{
    // Chemin vers votre script Python
        QString scriptPath = "C:/Users/user/Desktop/FACE RECOGNITION/main.py";

        // Créer un processus pour exécuter le script Python
        QProcess *process = new QProcess(this);
        process->start("python", QStringList() << scriptPath);

        // Vérifier si le processus a démarré avec succès
        if (process->waitForStarted()) {
            qDebug() << "Script Python démarré avec succès.";
        } else {
            qDebug() << "Erreur lors du démarrage du script Python.";
        }

        // Lire la sortie standard et la sortie d'erreur (si nécessaire)
        connect(process, &QProcess::readyReadStandardOutput, [=]() {
            qDebug() << "Sortie du script Python:" << process->readAllStandardOutput();
        });
}

//mainwindow.c
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Materiaux.h"
#include "connection.h"
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QTableWidget>
#include <QFileDialog>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QtCharts>
#include <QTextEdit>
#include <QPrinter>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        //-----------------------------------installation des reflexes selon les signaux--------------------------------//
        ui->lineEdit_nomM->installEventFilter(this);
        installEventFilter(this);
        connect(ui->tableWidgetM, &QTableWidget::cellDoubleClicked, this, &MainWindow::preventCellEdit);
        connect(ui->tableWidgetM, &QTableWidget::cellClicked, this, &MainWindow::showEditChoices);
        connect(ui->tableWidgetM, &QTableWidget::cellClicked, this, &MainWindow::handleCellClicked);
        connect(ui->lineEdit_RechercheParType, &QLineEdit::textChanged, this, &MainWindow::searchMateriauxByType);
        connect(ui->tableWidgetM, &QTableWidget::cellClicked, this, &MainWindow::editnomCell);
        connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidgetM()));
        //-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//
        // Connexion du signal clicked() du bouton à la fonction selectMaterialsForPodcast()
            connect(ui->pushButton_selectMaterialsForPodcast, &QPushButton::clicked, this, &MainWindow::selectMaterialAndInsertIntoContenir);
            // Connectez le signal personnalisé au slot showMaterialStats()
                connect(this, &MainWindow::statisticsTabSelected, this, &MainWindow::showMaterialStats);
            // Connectez du signal bila fincancier
                //connect(ui->pushButton_calculerBilan, &QPushButton::clicked, this, &MainWindow::bilanFinancier);
                connect(ui->pushButton_calculerBilan, &QPushButton::clicked, this, &MainWindow::bilanFinancier);

                // Ajouter le bouton "Exporter Bilan"
                    QPushButton *exportButton = new QPushButton("Exporter Bilan", ui->tabWidget);
                    exportButton->setStyleSheet("background-color: #E8E8FD;");
                    ui->tabWidget->setCornerWidget(exportButton, Qt::TopRightCorner); // Positionnez le bouton dans le coin supérieur droit du QTabWidget
                    //exportButton->hide();
                    // Connexion du signal clicked du bouton à une lambda qui appelle exporterBilan avec le pointeur textEditResultats
                            connect(exportButton, &QPushButton::clicked, [=]() {
                                exporterBilan(textEditResultats);
                            });









//--------------------------------Initialisation du tableau et ses champs----------------------------------------------//
        ui->tableWidgetM->setColumnCount(6);
        QStringList labelsM{"ID","Nom", "Type", "Etat", "Quantite", "Date"};
        ui->tableWidgetM->setHorizontalHeaderLabels(labelsM);
        ui->tableWidgetM->hideColumn(0); // Indice de la colonne ID
        updateTableWidgetM();

//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//

    }

//--------------------------------------------DESTRUCTEUR---------------------------------------------------------//
MainWindow::~MainWindow()
{
    delete ui;
}
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//




//-----------------------------Fonction pour mettre le tableau a jour apres les operations------------------------//
void MainWindow::updateTableWidgetM()
{
    Connection c;
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM MATERIAUX");

        // Efface le contenu actuel du tableau
        ui->tableWidgetM->clearContents();
        ui->tableWidgetM->setRowCount(0); // Réinitialise les lignes du tableau

        // Met à jour les données triées dans le tableau
        for (int row = 0; row < model->rowCount(); ++row)
        {
            // Insertion d'une nouvelle ligne dans le tableau
            ui->tableWidgetM->insertRow(row);

            // Ajout des données dans les cellules de la ligne
            for (int col = 0; col < 6; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
                ui->tableWidgetM->setItem(row, col, item);
            }
        }

        // Libère la mémoire occupée par le modèle
        delete model;
}
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//



//-----------------------------------FONCTION D'AJOUT--------------------------------------------------------------//
void MainWindow::on_pushButton_AjoutMateriaux_clicked()
{
    // Récupérer les valeurs saisies
    QString nom = ui->lineEdit_nomM->text();
    QString type = ui->lineEdit_typeM->text();
    QString etat = ui->lineEdit_etatM->currentText();
    QString quantite = ui->lineEdit_quantiteM->text();
    QString date = ui->lineEdit_dateM->text();

    // Vérifier si tous les champs obligatoires sont remplis
    if (nom.isEmpty() || type.isEmpty() || etat.isEmpty() || quantite.isEmpty() || date.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Validation du champ "quantite" (chiffres uniquement)
    QRegularExpression regex("[0-9]+");
    if (!regex.match(quantite).hasMatch()) {
        QMessageBox::warning(this, "Quantité invalide", "La quantité doit contenir uniquement des chiffres.");
        return;
    }

    // Si tous les champs sont valides, créer un matériel et l'ajouter à la base de données
    Materiaux i;
    i.set_nom(nom);
    i.set_type(type);
    i.set_etat(etat);
    i.set_quantite(quantite);
    i.set_date(date);

    Connection c;
    if (c.createconnect()) {
        if (i.ajouterMateriaux()) {
            updateTableWidgetM(); // Émettre le signal après l'insertion réussie
            QMessageBox::information(this, "Succès", "Données insérées dans la base de données.");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'insertion des données dans la base de données.");
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la connexion à la base de données.");
    }
}


//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//



//-------------------------------FONCTION DE MODIFICATION--------------------------------------------------------//
void MainWindow::on_pushButton_ModificationMateriaux_clicked()
{
    QItemSelectionModel *select = ui->tableWidgetM->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidgetM->item(row, 0)->text().toInt(); // Récupérer l'ID du materiel après le tri

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidgetM->item(row, 1)->text();
        QString type = ui->tableWidgetM->item(row, 2)->text();
        QString etat = ui->tableWidgetM->item(row, 3)->text();
        QString quantite = ui->tableWidgetM->item(row, 4)->text();
        QString date = ui->tableWidgetM->item(row, 5)->text();

        // Appelez la fonction modifierMateriaux pour mettre à jour la base de données
        Materiaux i;
        if (i.modifierMateriaux(idToModify, nom, type, etat, quantite, date))
        {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidgetM->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidgetM->setItem(row, 2, new QTableWidgetItem(type));
            ui->tableWidgetM->setItem(row, 3, new QTableWidgetItem(etat));
            ui->tableWidgetM->setItem(row, 4, new QTableWidgetItem(quantite));
            ui->tableWidgetM->setItem(row, 5, new QTableWidgetItem(date));

            emit dataUpdated(); // Mettre à jour la vue après la modification
            ui->label_10->setText("Données modifiées avec succés.");
        } else {
            ui->label_10->setText("Echec de la modification !");
        }
    } else {
        ui->label_10->setText("Veuillez selectionner une case à modifier !");
    }
    for (int r = 0; r < ui->tableWidgetM->rowCount(); ++r)
    {
        for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c)
        {
            QWidget *cellWidget = ui->tableWidgetM->cellWidget(r, c);
            if (cellWidget) {
                cellWidget->deleteLater();
            }
        }
    }
    updateTableWidgetM();
}

//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//


//----------------------------------------FONCTION DE SUPPRESSION----------------------------------------------//
/*void MainWindow::on_pushButton_SuppressionMateriaux_clicked()
{
    QItemSelectionModel *select = ui->tableWidgetM->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty())
    {
        int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->tableWidgetM->item(row, 0); // Vérifiez la colonne contenant l'ID après le tri
        if (item) // Vérifiez si l'élément existe
        {
            int idToDelete = item->text().toInt();

            Materiaux i;
            if (i.supprimerMateriaux(idToDelete))
            {
                // Supprimer les widgets de cellule associés à cette ligne
                for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c) {
                    QWidget *cellWidget = ui->tableWidgetM->cellWidget(row, c);
                    if (cellWidget) {
                        cellWidget->deleteLater();
                    }
                }

                // Supprimer la ligne de l'affichage
                ui->tableWidgetM->removeRow(row);
                emit dataUpdated(); // Mettre à jour la vue après la suppression
                ui->label_10->setText("Donnée supprimée avec succès.");
            }
            else
            {
                ui->label_10->setText("Echec de la suppression !");
            }
        }
        else
        {
            ui->label_10->setText("Selected row does not contain ID.");
        }
    }
    else
    {
        ui->label_10->setText("Veuillez sélectionner une ligne à supprimer !");
    }
}*/
void MainWindow::on_pushButton_SuppressionMateriaux_clicked()
{
    QItemSelectionModel *select = ui->tableWidgetM->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty())
    {
        int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->tableWidgetM->item(row, 0); // Vérifiez la colonne contenant l'ID après le tri
        if (item) // Vérifiez si l'élément existe
        {
            int idToDelete = item->text().toInt();

            // Vérifiez d'abord s'il existe des enregistrements correspondants dans la table CONTENIR
            QSqlQuery queryCheck;
            queryCheck.prepare("SELECT * FROM CONTENIR WHERE ID_MATERIEL = :idMateriel");
            queryCheck.bindValue(":idMateriel", idToDelete);
            if (queryCheck.exec() && queryCheck.next())
            {
                // S'il existe des enregistrements dans CONTENIR, informez l'utilisateur et demandez confirmation pour la suppression
                QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, "Supprimer le matériel", "Il existe des enregistrements dans la table CONTENIR pour ce matériel. Voulez-vous quand même le supprimer ?", QMessageBox::Yes | QMessageBox::No);
                if (confirmDelete == QMessageBox::Yes)
                {
                    // Supprimez d'abord les enregistrements correspondants dans la table CONTENIR
                    QSqlQuery deleteQuery;
                    deleteQuery.prepare("DELETE FROM CONTENIR WHERE ID_MATERIEL = :idMateriel");
                    deleteQuery.bindValue(":idMateriel", idToDelete);
                    if (deleteQuery.exec())
                    {
                        qDebug() << "Enregistrements dans CONTENIR supprimés avec succès !";

                        // Après avoir supprimé les enregistrements dans CONTENIR, vous pouvez maintenant supprimer la ligne dans MATERIAUX
                        Materiaux i;
                        if (i.supprimerMateriaux(idToDelete))
                        {
                            // Supprimer les widgets de cellule associés à cette ligne
                            for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c) {
                                QWidget *cellWidget = ui->tableWidgetM->cellWidget(row, c);
                                if (cellWidget) {
                                    cellWidget->deleteLater();
                                }
                            }

                            // Supprimer la ligne de l'affichage
                            ui->tableWidgetM->removeRow(row);
                            emit dataUpdated(); // Mettre à jour la vue après la suppression
                            ui->label_10->setText("Donnée supprimée avec succès.");
                        }
                        else
                        {
                            ui->label_10->setText("Echec de la suppression !");
                        }
                    }
                    else
                    {
                        qDebug() << "Erreur lors de la suppression des enregistrements dans CONTENIR :" << deleteQuery.lastError().text();
                    }
                }
            }
            else
            {
                // S'il n'y a pas d'enregistrements dans CONTENIR, vous pouvez simplement supprimer la ligne dans MATERIAUX
                Materiaux i;
                if (i.supprimerMateriaux(idToDelete))
                {
                    // Supprimer les widgets de cellule associés à cette ligne
                    for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c) {
                        QWidget *cellWidget = ui->tableWidgetM->cellWidget(row, c);
                        if (cellWidget) {
                            cellWidget->deleteLater();
                        }
                    }

                    // Supprimer la ligne de l'affichage
                    ui->tableWidgetM->removeRow(row);
                    emit dataUpdated(); // Mettre à jour la vue après la suppression
                    ui->label_10->setText("Donnée supprimée avec succès.");
                }
                else
                {
                    ui->label_10->setText("Echec de la suppression !");
                }
            }
        }
        else
        {
            ui->label_10->setText("Selected row does not contain ID.");
        }
    }
    else
    {
        ui->label_10->setText("Veuillez sélectionner une ligne à supprimer !");
    }
}



//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//


//--------------------------------------FONCTION DE TRI PAR DATE-----------------------------------------------//
void MainWindow::on_Button_trierParDateMateriaux_clicked()
{
        ui->tableWidgetM->clearContents(); // Efface le contenu actuel du tableau
        ui->tableWidgetM->setRowCount(0); // Efface également les lignes existantes du tableau
        // Trier les materiaux par date
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("Source_Projet2A4");
        db.setUserName("amine");
        db.setPassword("amine");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM MATERIAUX");
        if (query.exec(str))
        {
            ui->tableWidgetM->setColumnCount(6);
            QStringList labels;
            labels << "ID" << "Nom" << "Type" << "Etat" << "Quantite" << "Date";
            ui->tableWidgetM->setHorizontalHeaderLabels(labels);
            int row = 0;
            QVector<QPair<QDate, QStringList>> sortedMateriaux; // Pour stocker les materiaux triées

            while (query.next())
            {
                QStringList materiauxData;
                materiauxData << query.value(0).toString() << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString();
                QDate date = QDate::fromString(query.value(5).toString(), "dd/MM/yyyy"); // Convertir la chaîne de date en QDate
                sortedMateriaux.append(qMakePair(date, materiauxData));
            }

            // Tri des materiaux par date
            std::sort(sortedMateriaux.begin(), sortedMateriaux.end(), [](const QPair<QDate, QStringList> &a, const QPair<QDate, QStringList> &b) {
                return a.first < b.first;
            });

            // Affichage des materiaux triées dans le tableau
            for (const auto &materiaux : sortedMateriaux)
            {
                ui->tableWidgetM->insertRow(row);
                for (int col = 0; col < 6; ++col)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(materiaux.second[col]);
                    ui->tableWidgetM->setItem(row, col, item);
                }
                row++;
            }
        }
}
//----------------------------------------------------------------------------------------------------------------------------------------------//


//-------------------------------------FONCTION D'EXPORTATION EXCEL-----------------------------------------------------------------------------//
void MainWindow::on_pushButton_exportationFormatExcelMateriel_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save CSV File", QDir::homePath(), "CSV files (*.csv)");

        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);

                // Écrire les données
                for (int row = 0; row < ui->tableWidgetM->rowCount(); ++row) {
                    for (int col = 0; col < ui->tableWidgetM->columnCount(); ++col) {
                        QTableWidgetItem *item = ui->tableWidgetM->item(row, col);
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


//---------------------------------------------------FONCTION DE RECHERCHE PAR TYPE---------------------------------------------------------//
void MainWindow::searchMateriauxByType(const QString &type)
{
    // Nettoyer et convertir en minuscules la chaîne de recherche
    QString filterType = type.trimmed().toLower();

    // Parcourir toutes les lignes du tableau
    for (int row = 0; row < ui->tableWidgetM->rowCount(); ++row) {
        // Récupérer le type du matériau dans la troisième colonne (indice 2)
        QString currentType = ui->tableWidgetM->item(row, 2)->text().trimmed().toLower();

        // Vérifier si le type actuel contient la chaîne de recherche
        bool showRow = currentType.contains(filterType, Qt::CaseInsensitive);

        // Afficher ou masquer la ligne en fonction de la correspondance du type
        ui->tableWidgetM->setRowHidden(row, !showRow);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------FONCTION BOUTON ANNULER--------------------------------------------------------------------//
void MainWindow::on_pushButton_AnnulerMateriaux_clicked()
{
    ui->lineEdit_nomM->clear();
    ui->lineEdit_typeM->clear();
    ui->lineEdit_etatM->clear();
    ui->lineEdit_quantiteM->clear();
    ui->lineEdit_dateM->clear();
}
//----------------------------------------------------------------------------------------------------------------------------------------//


//-----------------------LES FONCTIONS DE CONTROLE DE SAISIE + GERER LES EVENEMENTS-------------------------------------------------------//
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEdit_quantiteM && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease))
    {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            QString text = ui->lineEdit_quantiteM->text();

            // Seuls les chiffres, un point et la touche "Backspace" sont autorisés
            if (!(keyEvent->text().isEmpty() && keyEvent->key() != Qt::Key_Backspace) &&
                !(keyEvent->text().at(0).isDigit() || keyEvent->text() == "." || keyEvent->key() == Qt::Key_Backspace)) {
                ui->label_5->setText("La quantite doit contenir uniquement\ndes chiffres et un point !");
                return true;
            }

            // On autorise un seul point décimal
            if (keyEvent->text() == "." && text.contains(".")) {
                ui->label_5->setText("Vous ne pouvez entrer qu'un\nseul point décimal !");
                return true;
            }

            // Si la saisie est valide, effacez le contenu du QLabel
            if (text.isEmpty() || (text.toDouble() != 0.0 && text.toDouble() != 0.0)) {
                ui->label_5->clear(); // Efface le contenu du QLabel
            }
        }
    if (event->type() == QEvent::MouseButtonPress) {
            // Si l'événement est un clic de souris, effacez le contenu du champ label_10
            ui->label_10->clear();
        }
        return QObject::eventFilter(obj, event);
}

void MainWindow::preventCellEdit()
{
    ui->tableWidgetM->setEditTriggers(QAbstractItemView::NoEditTriggers);
}



void MainWindow::showEditChoices(int row, int column)
{
    if (column == 3) {
            QStringList choices = {"bonne etat", "en reparation", "en panne"};
            showChoicesUnderCell(row, column, choices);
        }
}







void MainWindow::showChoicesUnderCell(int row, int column, const QStringList &choices) {
    // Sauvegarder la modification actuelle dans le tableau avant de supprimer les combobox précédents
    QWidget *previousWidget = ui->tableWidgetM->cellWidget(row, column);
    if (previousWidget) {
        QComboBox *previousComboBox = previousWidget->findChild<QComboBox*>();
        if (previousComboBox) {
            QString newText = previousComboBox->currentText();
            ui->tableWidgetM->item(row, column)->setText(newText);
        } else {
            QLineEdit *previousLineEdit = previousWidget->findChild<QLineEdit*>();
            if (previousLineEdit) {
                QString newText = previousLineEdit->text();
                ui->tableWidgetM->item(row, column)->setText(newText);
            }
        }
    }

    // Supprimer tous les QComboBox et les lignes de saisie de date présents dans le tableau
    for (int r = 0; r < ui->tableWidgetM->rowCount(); ++r)
    {
        for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c)
        {
            QWidget *cellWidget = ui->tableWidgetM->cellWidget(r, c);
            if (cellWidget) {
                cellWidget->deleteLater();
            }
        }
    }

    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    if (column == 5) { // Si c'est la colonne de la date
        QDateEdit *dateEdit = new QDateEdit();
        dateEdit->setDisplayFormat("dd/MM/yyyy"); // Format de la date
        layout->addWidget(dateEdit);
        layout->setAlignment(Qt::AlignTop);

        ui->tableWidgetM->setCellWidget(row, column, widget);

        connect(dateEdit, &QDateEdit::dateChanged, [=](const QDate &date) {
            // Mettre à jour la valeur de la cellule dans le QTableWidget avec la valeur de la date formatée
            QString formattedDate = date.toString("dd/MM/yyyy");
            ui->tableWidgetM->item(row, column)->setText(formattedDate);
        });
    } else { // Pour toutes les autres colonnes
        QComboBox *newComboBox = new QComboBox();
        newComboBox->addItems(choices);
        newComboBox->setCurrentIndex(-1); // Définir la sélection initiale sur -1

        layout->addWidget(newComboBox);
        layout->setAlignment(Qt::AlignTop);

        ui->tableWidgetM->setCellWidget(row, column, widget);

        //connect(newComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        connect(newComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=]() {
            // Mettre à jour la valeur de la cellule dans le QTableWidget avec la valeur sélectionnée dans le QComboBox
            QString newText = newComboBox->currentText();
            ui->tableWidgetM->item(row, column)->setText(newText);
        });
    }
}






void MainWindow::handleCellClicked(int row, int column) {
    if (column == 1 || column == 2 || column == 4) { // Colonnes nom, type et quantité
        ui->tableWidgetM->editItem(ui->tableWidgetM->item(row, column)); // Activer l'édition de la cellule
    } else if (column == 3) { // Colonnes état
        showEditChoices(row, column); // Afficher les choix du combo box
    } else if (column == 5) { // Colonne date
        QDateEdit *dateEdit = new QDateEdit(); // Créer un QDateEdit pour éditer la date
        dateEdit->setDate(QDate::fromString(ui->tableWidgetM->item(row, column)->text(), "dd-MM-yyyy")); // Récupérer et afficher la date actuelle dans le QDateEdit
        ui->tableWidgetM->setCellWidget(row, column, dateEdit); // Afficher le QDateEdit dans la cellule

        connect(dateEdit, &QDateEdit::dateChanged, this, [=](const QDate &date) {
            ui->tableWidgetM->item(row, column)->setText(date.toString("dd-MM-yyyy")); // Mettre à jour le texte de la cellule avec la nouvelle date
            ui->tableWidgetM->removeCellWidget(row, column); // Supprimer le QDateEdit après l'édition
        });
    }
}


/*void MainWindow::handleCellClicked(int row, int column) {
    if (column == 3) { // Si la colonne est la colonne de la date
        QStringList choices; // Vous pouvez remplir cette liste avec des choix de date si nécessaire
        showChoicesUnderCell(row, column, choices);
    }
}*/

void MainWindow::editnomCell(int row, int column)
{
    if (column == 0) { // Vérifier si la colonne est la colonne "nom"
        QLineEdit *lineEdit = new QLineEdit(ui->tableWidgetM);
        QString text = ui->tableWidgetM->item(row, column)->text();
        lineEdit->setText(text);
        ui->tableWidgetM->setCellWidget(row, column, lineEdit);
        lineEdit->setFocus();
        connect(lineEdit, &QLineEdit::textChanged, [=](const QString &newText) {
            QRegExp regExp("^\\d*\\.?\\d*$"); // Expression régulière pour vérifier la validité du nom
            if (!regExp.exactMatch(newText)) {
                // Si la saisie est invalide, effacer le contenu du line edit
                lineEdit->setText(text);
            }
        });
        connect(lineEdit, &QLineEdit::editingFinished, [=]() {
            // Mettre à jour la cellule du tableau avec le nouveau montant
            QString newText = lineEdit->text();
            QTableWidgetItem *item = new QTableWidgetItem(newText);
            ui->tableWidgetM->setItem(row, column, item);
            // Supprimer le line edit après l'édition
            ui->tableWidgetM->removeCellWidget(row, column);
        });
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------//

void MainWindow::selectMaterialAndInsertIntoContenir()
{
    // Afficher une liste de podcasts disponibles
    QSqlQuery queryPodcasts("SELECT ID_PODCAST, NOM FROM PODCASTS");

    QStringList availablePodcasts;

    while (queryPodcasts.next())
    {
        availablePodcasts << queryPodcasts.value(1).toString(); // Ajouter le nom du podcast à la liste
    }

    // Afficher une liste de matériaux disponibles avec leur type
    QSqlQuery queryMaterials("SELECT ID_MATERIEL, NOM, TYPE FROM MATERIAUX");

    QStringList availableMaterials;

    while (queryMaterials.next())
    {
        QString materialNameWithType = queryMaterials.value(1).toString() + " - " + queryMaterials.value(2).toString();
        availableMaterials << materialNameWithType; // Ajouter le nom du matériau suivi de son type à la liste
    }

    // Afficher une boîte de dialogue permettant à l'utilisateur de sélectionner un podcast et un matériau
    bool ok;
    QString selectedPodcast = QInputDialog::getItem(this, "Sélection du podcast", "Sélectionnez un podcast :", availablePodcasts, 0, false, &ok);
    QString selectedMaterialWithType = QInputDialog::getItem(this, "Sélection du matériau", "Sélectionnez un matériau :", availableMaterials, 0, false, &ok);

    if (ok)
    {
        // Extraire le nom du matériau sélectionné
        QString selectedMaterial = selectedMaterialWithType.split(" - ").at(0);

        // Récupérer l'ID du podcast sélectionné
        QSqlQuery podcastQuery;
        podcastQuery.prepare("SELECT ID_PODCAST FROM PODCASTS WHERE NOM = :nom");
        podcastQuery.bindValue(":nom", selectedPodcast);
        if (podcastQuery.exec() && podcastQuery.next())
        {
            int podcastId = podcastQuery.value(0).toInt();

            // Récupérer l'ID du matériau sélectionné
            QSqlQuery materialQuery;
            materialQuery.prepare("SELECT ID_MATERIEL FROM MATERIAUX WHERE NOM = :nom");
            materialQuery.bindValue(":nom", selectedMaterial);
            if (materialQuery.exec() && materialQuery.next())
            {
                int materialId = materialQuery.value(0).toInt();

                // Insérer un nouvel enregistrement dans la table CONTENIR pour le matériau et le podcast sélectionnés
                QSqlQuery insertQuery;
                insertQuery.prepare("INSERT INTO CONTENIR (ID_MATERIEL, ID_PODCAST) VALUES (:idMateriel, :idPodcast)");
                insertQuery.bindValue(":idMateriel", materialId);
                insertQuery.bindValue(":idPodcast", podcastId);
                if (insertQuery.exec())
                {
                    qDebug() << "Matériau inséré avec succès dans la table CONTENIR !";
                }
                else
                {
                    qDebug() << "Erreur lors de l'insertion du matériau dans la table CONTENIR :" << insertQuery.lastError().text();
                }
            }
            else
            {
                qDebug() << "Erreur lors de la récupération de l'ID du matériau :" << materialQuery.lastError().text();
            }
        }
        else
        {
            qDebug() << "Erreur lors de la récupération de l'ID du podcast :" << podcastQuery.lastError().text();
        }
    }
    else
    {
        qDebug() << "La sélection du podcast ou du matériau a été annulée.";
    }
}




/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

void MainWindow::on_tabWidget_currentChanged(int index)
{
    // Si l'index correspond à l'onglet des statistiques (par exemple, index 1), émettez le signal
    if (index == 1)
    {
        emit statisticsTabSelected(); // Émettre le signal
    }
}

void MainWindow::showMaterialStats()
{
    // Récupérer les données depuis la base de données
    QSqlQuery queryEtat;
    queryEtat.exec("SELECT etat FROM MATERIAUX");

    // Utiliser un map pour stocker le nombre de matériaux par état
    QMap<QString, int> etatCountMap;

    while (queryEtat.next()) {
        QString etat = queryEtat.value(0).toString();
        // Vérifier si l'état existe déjà dans le map
        if (etatCountMap.contains(etat)) {
            // Incrémenter le nombre de matériaux pour cet état
            etatCountMap[etat]++;
        } else {
            // Si l'état n'existe pas encore, l'ajouter au map avec un compteur initialisé à 1
            etatCountMap.insert(etat, 1);
        }
    }

    // Créer un graphique à secteurs pour les états
    QtCharts::QPieSeries *pieSeries = new QtCharts::QPieSeries();

    // Parcourir le map pour ajouter les données au graphique
    QMapIterator<QString, int> i(etatCountMap);
    while (i.hasNext()) {
        i.next();
        QtCharts::QPieSlice *slice = pieSeries->append(i.key(), i.value());
        slice->setLabel(QString("%1: %2").arg(i.key()).arg(i.value()));
    }

    QtCharts::QChart *pieChart = new QtCharts::QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Répartition des matériaux par état");

    // Ajouter une animation à la transition lors de l'affichage du graphique
    pieChart->setAnimationOptions(QtCharts::QChart::AllAnimations); // Activer toutes les animations disponibles

    QtCharts::QChartView *pieChartView = new QtCharts::QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing); // Activer l'anti-aliasing pour des graphiques plus lisses

    // Assurez-vous d'avoir correctement nommé votre tabWidget, supposons que c'est "tabWidget"
    QTabWidget *tabWidget = ui->tabWidget;
    QWidget *statWidget = tabWidget->widget(1); // Onglet de statistiques

    // Créez un layout pour l'onglet de statistiques
    QVBoxLayout *statLayout = new QVBoxLayout(statWidget);
    statLayout->addWidget(pieChartView); // Ajoutez le graphique à l'onglet de statistiques

    // Définir le layout pour l'onglet de statistiques
    statWidget->setLayout(statLayout);
}


/*///////////////////////////////////////////BILAN FINANCIER//////////////////////////////////////////////////////////*/

void MainWindow::bilanFinancier() {
    QSqlQuery query;
    query.prepare("SELECT SUM(MATERIAUX.QUANTITE), "
                  "SUM(TO_NUMBER(TRANSACTIONS.MONTANT)), "
                  "SUM(CASE WHEN TRANSACTIONS.TYPE = 'revenue' THEN TO_NUMBER(TRANSACTIONS.MONTANT) ELSE 0 END) AS totalRevenus, "
                  "SUM(CASE WHEN TRANSACTIONS.TYPE = 'depense' THEN TO_NUMBER(TRANSACTIONS.MONTANT) ELSE 0 END) AS totalDepenses, "
                  "LISTAGG(TRANSACTIONS.MODEDEPAIEMENT, ', ') WITHIN GROUP (ORDER BY TRANSACTIONS.MODEDEPAIEMENT) AS modesDePaiement "
                  "FROM MATERIAUX "
                  "JOIN TRANSACTIONS ON MATERIAUX.ID_MATERIEL = TRANSACTIONS.ID_TRANSACTION");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des données du bilan financier:" << query.lastError().text();
        return;
    }

    double totalMateriaux = 0;
    double totalTransactions = 0;
    double totalRevenus = 0;
    double totalDepenses = 0;
    QString modesDePaiement;

    if (query.next()) {
        totalMateriaux = query.value(0).toDouble();
        totalTransactions = query.value(1).toDouble();
        totalRevenus = query.value(2).toDouble();
        totalDepenses = query.value(3).toDouble();
        modesDePaiement = query.value(4).toString();
    }

    // Créer une nouvelle page d'onglet pour afficher les résultats du bilan financier
    QTextEdit *textEditResultats = new QTextEdit();
    ui->tabWidget->addTab(textEditResultats, "Résultats financiers");

    // Afficher les résultats du bilan financier dans le QTextEdit
    afficherResultatsBilanFinancier(textEditResultats, totalMateriaux, totalTransactions, totalRevenus, totalDepenses, modesDePaiement);

}

QString MainWindow::getTypePaiement(const QString& mode) {
    QSqlQuery query;
    query.prepare("SELECT TYPE FROM TRANSACTIONS WHERE MODEDEPAIEMENT = :mode");
    query.bindValue(":mode", mode);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        return "Type non trouvé";
    }
}
QString MainWindow::getIdTransaction(const QString& mode) {
    QSqlQuery query;
    query.prepare("SELECT ID_TRANSACTION FROM TRANSACTIONS WHERE MODEDEPAIEMENT = :mode");
    query.bindValue(":mode", mode);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        return "Non trouvé";
    }
}

QString MainWindow::filtrerModesPaiement(const QString& type, const QString& modesDePaiement) {
    QStringList modes = modesDePaiement.split(", ");
    QStringList modesFiltres;
    foreach (const QString &mode, modes) {
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM TRANSACTIONS WHERE MODEDEPAIEMENT = :mode AND TYPE = :type");
        query.bindValue(":mode", mode);
        query.bindValue(":type", type);
        if (query.exec() && query.next()) {
            int count = query.value(0).toInt();
            if (count > 0) {
                modesFiltres.append(mode);
            }
        }
    }
    return modesFiltres.join(", ");
}

void MainWindow::analyserTransactionsParCategorie(QTableWidget *tableWidget) {
    QSqlQuery query;
    query.prepare("SELECT TRANSACTIONS.CATEGORIE, SUM(TRANSACTIONS.MONTANT) "
                  "FROM MATERIAUX "
                  "JOIN TRANSACTIONS ON MATERIAUX.ID_MATERIEL = TRANSACTIONS.ID_TRANSACTION "
                  "GROUP BY TRANSACTIONS.CATEGORIE");

    if (query.exec()) {
        int row = tableWidget->rowCount();
        while (query.next()) {
            QString categorie = query.value(0).toString();
            double montantTotal = query.value(1).toDouble();

            // Insérer les valeurs directement dans le tableWidget
            tableWidget->insertRow(row);
            tableWidget->setItem(row, 0, new QTableWidgetItem("Montant total pour la catégorie " + categorie));
            tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(montantTotal)));

            row++;
        }
    } else {
        qDebug() << "Erreur lors de l'analyse des transactions par catégorie :" << query.lastError().text();
    }
}

void MainWindow::afficherMateriauxParTransaction(int idTransaction, QTextEdit *textEdit) {
    QString rapportTexte;

    rapportTexte += "<h2>Matériaux correspondant à la transaction " + QString::number(idTransaction) + "</h2>";

    QSqlQuery query;
    query.prepare("SELECT NOM FROM MATERIAUX WHERE ID_TRANSACTION = :idTransaction");
    query.bindValue(":idTransaction", idTransaction);

    if (query.exec()) {
        rapportTexte += "<ul>";
        while (query.next()) {
            QString nomMateriel = query.value(0).toString();
            rapportTexte += "<li>" + nomMateriel + "</li>";
        }
        rapportTexte += "</ul>";
    } else {
        rapportTexte += "<p>Erreur lors de la récupération des matériaux.</p>";
    }

    textEdit->setHtml(rapportTexte);
}

void MainWindow::afficherResultatsBilanFinancier(QTextEdit *textEdit, double totalMateriaux, double totalTransactions, double totalRevenus, double totalDepenses, const QString& modesDePaiement) {
    QString rapportTexte;

    // En-tête du rapport
    rapportTexte += "<h2>Résultats financiers</h2>";

    // Section des totaux
    rapportTexte += "<h3>Totaux</h3>";
    rapportTexte += "<ul>";
    rapportTexte += "<li>Total des matériaux : " + QString::number(totalMateriaux) + "</li>";
    rapportTexte += "<li>Total des transactions : " + QString::number(totalTransactions) + "</li>";
    rapportTexte += "<li>Modes de paiement :</li>"; // Nouvelle ligne pour commencer la liste des modes de paiement
    QStringList modes = modesDePaiement.split(", ");
    QMap<int, QString> transactions; // Utiliser une QMap pour trier les transactions par leur ID
    for(const QString& mode : modes) {
        QString type = getTypePaiement(mode); // Fonction à créer pour obtenir le type de paiement
        QString idTransaction = getIdTransaction(mode); // Fonction à créer pour obtenir l'ID de transaction
        transactions.insert(idTransaction.toInt(), mode.trimmed() + " : " + type + " : transaction numero " + idTransaction);
    }
    for (auto it = transactions.begin(); it != transactions.end(); ++it) {
        rapportTexte += "<ul>"; // Commence une sous-liste pour chaque mode de paiement
        rapportTexte += "<li>" + it.value();
        rapportTexte += "</ul>"; // Termine la sous-liste pour chaque mode de paiement
    }
        rapportTexte += "<li>Revenus totaux : " + QString::number(totalRevenus) + "</li>";
        rapportTexte += "<li>Dépenses totales : " + QString::number(totalDepenses) + "</li>";
        rapportTexte += "</ul>";

    // Section de l'analyse par catégorie
    rapportTexte += "<h3>Analyse des transactions par catégorie</h3>";
    rapportTexte += "<ul>";
    QSqlQuery query;
    query.prepare("SELECT TRANSACTIONS.CATEGORIE, SUM(TRANSACTIONS.MONTANT) "
                  "FROM MATERIAUX "
                  "JOIN TRANSACTIONS ON MATERIAUX.ID_MATERIEL = TRANSACTIONS.ID_TRANSACTION "
                  "GROUP BY TRANSACTIONS.CATEGORIE");

    if (query.exec()) {
        while (query.next()) {
            QString categorie = query.value(0).toString();
            double montantTotal = query.value(1).toDouble();
            rapportTexte += "<li>Montant total pour la catégorie " + categorie + " : " + QString::number(montantTotal) + "</li>";
        }
    } else {
        rapportTexte += "<li>Erreur lors de l'analyse des transactions par catégorie :" + query.lastError().text() + "</li>";
    }
    rapportTexte += "</ul>";

    // Afficher les matériaux pour chaque transaction
    QSqlQuery transactionQuery;
    transactionQuery.prepare("SELECT ID_TRANSACTION FROM TRANSACTIONS");

    if (transactionQuery.exec()) {
        while (transactionQuery.next()) {
            int idTransaction = transactionQuery.value(0).toInt();
            afficherMateriauxParTransaction(idTransaction, textEdit); // Appeler la fonction pour afficher les matériaux de chaque transaction
        }
    } else {
        qDebug() << "Erreur lors de la récupération des transactions :" << transactionQuery.lastError().text();
    }

    // Afficher le rapport en texte dans le QTextEdit
    textEdit->setHtml(rapportTexte);
}
// Fonction pour exporter le bilan financier au format PDF
void MainWindow::exporterBilan(QTextEdit *textEditResultats) {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer le fichier PDF"), "", "PDF (*.pdf)");

    if (!fileName.isEmpty()) {
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(fileName);

        QTextDocument *document = textEditResultats->document();
        document->print(&printer);
    }
}
/*void MainWindow::exporterBilanCSV(QTextEdit *textEditResultats) {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer le fichier CSV"), "", "CSV (*.csv)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            // Récupérer le contenu HTML du QTextEdit
            QString htmlContent = textEditResultats->toHtml();

            // Supprimer les balises HTML pour obtenir un texte brut
            QString plainText = Qt::convertFromPlainText(htmlContent);

            // Écrire le texte brut dans le fichier CSV
            out << plainText;

            file.close();
        } else {
            qDebug() << "Erreur lors de l'ouverture du fichier :" << file.errorString();
        }
    }
}*/


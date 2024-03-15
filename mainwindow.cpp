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
        // Si tous les champs sont valides, créer un materiel et l'ajouter à la base de données
        Materiaux i;
        i.set_nom(nom);
        i.set_type(type);
        i.set_etat(etat);
        i.set_quantite(quantite);
        i.set_date(date);
        Connection c;
        if (c.createconnect())
        {
            if (i.ajouterMateriaux())
            {
                updateTableWidgetM(); // Émettre le signal après l'insertion réussie
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

/*void MainWindow::on_pushButton_ModificationMateriaux_clicked()
{
   QItemSelectionModel *select = ui->tableWidgetM->selectionModel();
   QModelIndexList selectedIndexes = select->selectedIndexes();

   if (!selectedIndexes.isEmpty()) {
       int row = selectedIndexes.first().row();
       int idToModify = ui->tableWidgetM->item(row, 0)->text().toInt();

       // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
       QString nom = ui->tableWidgetM->item(row, 1)->text();
       QString type = ui->tableWidgetM->item(row, 2)->text();
       QString etat = ui->tableWidgetM->item(row, 3)->text();
       QString quantitie = ui->tableWidgetM->item(row, 4)->text();
       QString date = ui->tableWidgetM->item(row, 5)->text();


       // Appelez la fonction modifierMateriel pour mettre à jour la base de données
       Materiaux i;
       if (i.modifierMateriaux(idToModify, nom, type, etat, quantitie, type)) {
           // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
           ui->tableWidgetM->setItem(row, 1, new QTableWidgetItem(nom));
           ui->tableWidgetM->setItem(row, 2, new QTableWidgetItem(type));
           ui->tableWidgetM->setItem(row, 3, new QTableWidgetItem(etat));
           ui->tableWidgetM->setItem(row, 4, new QTableWidgetItem(quantitie));
           ui->tableWidgetM->setItem(row, 4, new QTableWidgetItem(date));


           emit dataUpdated(); // Mettre à jour la vue après la modification
           QMessageBox::information(this, "Succés", "Succés d'ajout dans la base de données");
       }
       else
       {
           QMessageBox::critical(this, "Erreur", "Connexion impossible à la base de données.");
       }
   }
   else
   {
       QMessageBox::critical(this, "Attention", "Selectionnez une ligne à modifier");
   }
}*/


//----------------------------------------FONCTION DE SUPPRESSION----------------------------------------------//
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

        connect(newComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
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


/*void MainWindow::selectMaterialsForPodcast()
{
    // Afficher une liste de matériaux disponibles
    QSqlQuery query("SELECT * FROM MATERIAUX WHERE UtiliseDansPodcast = 0"); // Sélectionner les matériaux non utilisés dans un podcast
    QStringList availableMaterials;

    while (query.next())
    {
        availableMaterials << query.value(1).toString(); // Supposons que le nom du matériau est dans la colonne 1
    }

    // Afficher une boîte de dialogue permettant à l'utilisateur de sélectionner les matériaux
    bool ok;
    QStringList selectedMaterials = QInputDialog::getItems(this, "Sélection des matériaux pour le podcast", "Sélectionnez les matériaux à utiliser :", availableMaterials, 0, false, &ok);

    if (ok)
    {
        // Marquer les matériaux sélectionnés comme étant utilisés dans le podcast
        for (const QString& material : selectedMaterials)
        {
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE MATERIAUX SET UtiliseDansPodcast = 1 WHERE Nom = :nom");
            updateQuery.bindValue(":nom", material);
            if (!updateQuery.exec())
            {
                qDebug() << "Erreur lors de la mise à jour de l'utilisation du matériau :" << updateQuery.lastError().text();
            }
        }
        qDebug() << "Matériaux sélectionnés avec succès pour le podcast !";
    }
    else
    {
        qDebug() << "La sélection des matériaux pour le podcast a été annulée.";
    }
}
*/


/*void MainWindow::on_stats_clicked()
{
    // Récupérer les données depuis la base de données
    QSqlQuery queryNationalite;
    queryNationalite.exec("SELECT nationalite FROM MATERIAUX");

    // Utiliser un map pour stocker le nombre d'artistes par nationalité
    QMap<QString, int> nationaliteCountMap;

    while (queryNationalite.next()) {
        QString nationalite = queryNationalite.value(0).toString();
        // Vérifier si la nationalité existe déjà dans le map
        if (nationaliteCountMap.contains(nationalite)) {
            // Incrémenter le nombre d'artistes pour cette nationalité
            nationaliteCountMap[nationalite]++;
        } else {
            // Si la nationalité n'existe pas encore, l'ajouter au map avec un compteur initialisé à 1
            nationaliteCountMap.insert(nationalite, 1);
        }
    }

    // Créer un dialogue pour afficher les statistiques
    QDialog dialog(this);
    dialog.resize(800, 600);
    dialog.setWindowTitle("Statistiques des artistes");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Créer un graphique à barres pour les nationalités
    QtCharts::QBarSet *barSet = new QtCharts::QBarSet("Nombre d'artistes");
    QtCharts::QBarSeries *barSeries = new QtCharts::QBarSeries();

    // Parcourir le map pour ajouter les données au graphique
    QMapIterator<QString, int> i(nationaliteCountMap);
    while (i.hasNext()) {
        i.next();
        *barSet << i.value();
    }
    barSeries->append(barSet);

    QtCharts::QChart *barChart = new QtCharts::QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Répartition des artistes par nationalité");

    QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
    axisX->append(nationaliteCountMap.keys());
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    QtCharts::QChartView *barChartView = new QtCharts::QChartView(barChart);
    layout->addWidget(barChartView);
}*/

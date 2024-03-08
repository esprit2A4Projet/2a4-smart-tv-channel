#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "podcast.h"
#include "connection.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QAxObject>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QVariant>
#include <QThread>
#include <QDebug>
#include <QTextStream>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QPainter>
#include <QPrinter>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    podcast E;

           model = new QStandardItemModel(this);
            ui->tableView->setModel(model);

              model->setColumnCount(5); // Nous avons 5 colonnes dans notre TableView
                 model->setHorizontalHeaderLabels({"ID", "Nom", "Duree", "Lieu", "Categorie", "Date"});
                 ui->tableView->setModel(model);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    QString nom = ui->lineEdit_nom->text();
    QString duree = ui->lineEdit_duree->text();
    QString lieu = ui->lineEdit_lieu->text();
    QString categorie = ui->lineEdit_categorie->text();
    QString date_pod = ui->dateEdit->text();
    int id = ui->lineEdit_7->text().toInt(); // Assuming this is the employee ID

    // Vérifier si les champs obligatoires sont vides
    if (nom.isEmpty() || duree.isEmpty() || lieu.isEmpty() || categorie.isEmpty() || date_pod.isEmpty()) {
        QMessageBox::warning(this, "Warning", "veuillez remplir toutes les cases.");
        return;
    }

    // Ajouter ici les expressions régulières pour la validation des champs si nécessaire

    podcast p(nom, duree, lieu, categorie, date_pod);
    bool success = false;

    if (id == 0) {
         // Si l'ID est 0, cela signifie que nous ajoutons un nouvel enregistrement
         success = p.ajouter();
    } else {
         // Sinon, nous mettons à jour un enregistrement existant
         success = p.update(id,nom, duree, lieu, categorie, date_pod);
         ui->lineEdit_7->clear();
         QMessageBox::information(nullptr, QObject::tr("OK"),
                       QObject::tr("Modification  effectué\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);
    }
   QMessageBox msgBox;
    if (success) {
      //  msgBox.setText("Operation successful.");
        ui->tableView->setModel(p.afficher());
        ui->lineEdit_nom->clear();
        ui->lineEdit_duree->clear();
        ui->lineEdit_lieu->clear();
        ui->lineEdit_categorie->clear();
        ui->dateEdit->clear();
    } else {
        msgBox.setText("Operation failed.");
          msgBox.exec();
    }
    // Validation du champ "duree" (chiffres uniquement)
    QRegularExpression regex("[0-9]+");
    if (!regex.match(duree).hasMatch()) {
        QMessageBox::warning(this, "Warning", "Invalid duree format. veuillez entrez des numeros.");
        return;
    }
}

void MainWindow::on_pushButton_annuler_clicked()
{
    ui->lineEdit_nom->clear();
    ui->lineEdit_duree->clear();
    ui->lineEdit_lieu->clear();
    ui->lineEdit_categorie->clear();
    ui->dateEdit->clear();
}


void MainWindow::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_7->text().toInt();
       // Obtenez l'ID entré
          podcast p;
          QSqlQuery query;
          query.prepare("SELECT id_podcast,nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE id_podcast = ?");
          query.addBindValue(id);

          if (query.exec() && query.next()) {
              // Récupérez les valeurs de la requête
              QString nom = query.value(0).toString();
              QString duree = query.value(1).toString();
              QString lieu = query.value(2).toString();
              QString categorie = query.value(2).toString();
              QString date_pod = query.value(4).toString();


              podcast(nom,duree,lieu,categorie,date_pod);
          // Récupérer les informations du podcast pour l'ID donné
          if (p.supprimer((id))) {
               ui->tableView->setModel(p.afficher());
              QMessageBox::information(nullptr, QObject::tr("OK"),
                            QObject::tr("Suppression  effectué\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
              } else {
                  // Afficher un message indiquant que le podcast est introuvable
                  QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                        QObject::tr("Podcast inexistant.\n"
                                                    "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
              }
          } else {
              // Afficher un message indiquant que le podcast est introuvable
              QMessageBox::critical(nullptr, QObject::tr("Error"),
                                    QObject::tr("Podcast inexistant.\n"
                                                "Click sur Cancel pour quitter."), QMessageBox::Cancel);
}
}


void MainWindow::on_pushButton_modifier_clicked()
{

        int id = ui->lineEdit_7->text().toInt();
        // Obtenez l'ID entré
        podcast p;
        QSqlQuery query;
        query.prepare("SELECT id_podcast, nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE id_podcast = ?");
        query.addBindValue(id);

        if (query.exec() && query.next()) {
            // Récupérez les valeurs de la requête
            QString nom = query.value(1).toString();
            QString duree = query.value(2).toString();
            QString lieu = query.value(3).toString();
            QString categorie = query.value(4).toString();
            QString date_pod = query.value(5).toString();

            // Affichez les informations du podcast dans les champs de saisie pour permettre la modification
            ui->lineEdit_nom->setText(nom);
            ui->lineEdit_duree->setText(duree);
            ui->lineEdit_lieu->setText(lieu);
            ui->lineEdit_categorie->setText(categorie);
            ui->dateEdit->setDate(QDate::fromString(date_pod, "yyyy-MM-dd"));

            // Mettez à jour le podcast avec les nouvelles valeurs si l'utilisateur modifie les champs et appuie sur un bouton de confirmation
            if (ui->pushButton_ajouter->isEnabled()) {
                QString newNom = ui->lineEdit_nom->text();
                QString newDuree = ui->lineEdit_duree->text();
                QString newLieu = ui->lineEdit_lieu->text();
                QString newCategorie = ui->lineEdit_categorie->text();
                QString newDatePod = ui->dateEdit->text();

                if (p.update(id, newNom, newDuree, newLieu, newCategorie, newDatePod)) {
                    ui->tableView->setModel(p.afficher());
                    QMessageBox::information(nullptr, QObject::tr("OK"),
                                             QObject::tr("Modification effectuée.\n"
                                                         "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
                } else {
                    QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                          QObject::tr("Échec de la modification du podcast.\n"
                                                      "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
                }
            }
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("Podcast introuvable.\n"
                                              "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
        }
    }



void MainWindow::on_pushButton_expo_clicked()
{
    // Obtenir le chemin du fichier de sauvegarde
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter vers PDF", QString(), "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty())
        return;

    // Définir le format du fichier PDF
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Ouvrir le fichier PDF pour l'impression
    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier PDF pour l'impression.");
        return;
    }

    // Écrire le contenu dans le fichier PDF
    painter.drawText(100, 100, "Liste des podcasts :\n\n");

    // Parcourir les enregistrements de podcast et les écrire dans le fichier PDF
    QSqlQuery query("SELECT * FROM PODCASTS");
    int yPos = 150; // Position verticale de départ
    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString duree = query.value(2).toString();
        QString lieu = query.value(3).toString();
        QString categorie = query.value(4).toString();
        QString date_pod = query.value(5).toString();

        // Écrire les données de chaque podcast dans le fichier PDF
        painter.drawText(100, yPos, "ID : " + QString::number(id));
        painter.drawText(200, yPos, "Nom : " + nom);
        painter.drawText(300, yPos, "Durée : " + duree);
        painter.drawText(400, yPos, "Lieu : " + lieu);
        painter.drawText(500, yPos, "Catégorie : " + categorie);
        painter.drawText(600, yPos, "Date du Podcast : " + date_pod);

        yPos += 50; // Augmenter la position verticale pour la prochaine entrée
    }

    // Fermer le fichier PDF
    painter.end();

    QMessageBox::information(this, "Succès", "Les données ont été exportées vers PDF avec succès.");
}



void MainWindow::on_pushButton_historique_clicked()
{
    // Ouvrir une boîte de dialogue pour choisir l'emplacement et le nom du fichier
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer l'historique des modifications", QString(), "Fichiers texte (*.txt)");

    // Vérifier si l'utilisateur a annulé la boîte de dialogue
    if (filePath.isEmpty())
        return;

    // Ouverture du fichier texte pour l'historique des modifications en écriture (mode Append pour ajouter à la fin)
    QFile file(filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open history file for writing.";
        return;
    }

    // Création du texte de l'historique à partir des données de la fonction de modification
    QString historyText;
    historyText += "Historique des modifications :\n";
    historyText += "-------------------------------------\n";

    // Obtention de la date et de l'heure actuelles
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timestamp = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    // Ajout des informations de modification dans l'historique
    historyText += "Timestamp : " + timestamp + "\n";
    historyText += "ID du podcast modifié : " + ui->lineEdit_7->text() + "\n";
    historyText += "Nouveau nom : " + ui->lineEdit_nom->text() + "\n";
    historyText += "Nouvelle durée : " + ui->lineEdit_duree->text() + "\n";
    historyText += "Nouveau lieu : " + ui->lineEdit_lieu->text() + "\n";
    historyText += "Nouvelle catégorie : " + ui->lineEdit_categorie->text() + "\n";
    historyText += "Nouvelle date du podcast : " + ui->dateEdit->text() + "\n";
    historyText += "-------------------------------------\n\n";

    // Écriture de l'historique dans le fichier texte
    QTextStream out(&file);
    out << historyText;

    // Fermeture du fichier
    file.close();

    // Affichage d'un message pour informer l'utilisateur que l'historique a été enregistré avec succès
    QMessageBox::information(this, "Historique enregistré", "L'historique des modifications a été enregistré avec succès dans le fichier.");
}


void MainWindow::on_pushButton_recherche_clicked() // chercher
{
    podcast p;

    int id = ui->lineEdit_8->text().toInt();
   // Obtenez l'ID entré
    if (id > 0 && id < 99999999)
    {
      QSqlQuery query;
      query.prepare("SELECT id,nom, duree, lieu, categorie, poste FROM PODCASTS WHERE id = ?");
      query.addBindValue(id);

      if (query.exec() && query.next()) {
          // Récupérez les valeurs de la requête
          QString nom = query.value(0).toString();
          QString duree = query.value(1).toString();
          QString lieu = query.value(2).toString();
          QString categorie = query.value(2).toString();
          QString date_pod = query.value(4).toString();


          podcast(nom,duree,lieu,categorie,date_pod);
          if (p.Rechercher(id)) {
               ui->tableView->setModel(p.Rechercher(id));
              QMessageBox::information(nullptr, QObject::tr("OK"),
                            QObject::tr("podcast  Trouvé\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
              } else {
                  // Afficher un message indiquant que le podcast est introuvable
                  QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                        QObject::tr("podcast inexistant.\n"
                                                    "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
              }
          } else {
              // Afficher un message indiquant que le podcast est introuvable
              QMessageBox::critical(nullptr, QObject::tr("Error"),
                                    QObject::tr("podcast inexistant.\n"
                                                "Click sur Cancel pour quitter."), QMessageBox::Cancel);
          }


    ui->lineEdit_8->clear();
    }
    else
    {
        // Afficher un message indiquant que l'ID n'est pas valide
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                            QObject::tr("Veuillez saisir un entier valide.\n"
                                        "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}


void MainWindow::on_pushButton_trier_clicked() //tri
{
    podcast p;

      ui->tableView->setModel(p.tri());

      bool test=p.tri();
      if(test){
          QMessageBox::information(nullptr,QObject::tr("OK"),
                                   QObject::tr("tri effectué. \n"
                                               "click Cancel to exist."),QMessageBox::Cancel);

      }
      else
      {
          QMessageBox::critical(nullptr, QObject::tr("not OK"),
                      QObject::tr("tri non effectué.\n"
                                  "Click Cancel to exit."), QMessageBox::Cancel);
      }
}


/*

QString filePath = QFileDialog::getSaveFileName(this, "Export to Excel", QString(), "CSV Files (*.csv)");

if (filePath.isEmpty())
    return;

QFile file(filePath);
if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to open file for writing.";
    return;
}

QTextStream out(&file);

// Write header
QStringList headerData = {"Nom", "Duree", "Lieu", "Categorie", "Date_Pod"};
out << headerData.join(",") << "\n";

// Retrieve data from the database
QSqlQuery query;
if (!query.exec("SELECT nom, duree, lieu, categorie, date_pod FROM tableView")) {
    qDebug() << "Error executing SQL query:" << query.lastError().text();
    return;
}

// Write data to file
while (query.next()) {
    for (int col = 0; col < query.record().count(); ++col) {
        out << query.value(col).toString();
        if (col < query.record().count() - 1)
            out << ",";
    }
    out << "\n";
}

file.close();

qDebug() << "Database exported to Excel successfully.";

*/


/*
* QString filePath = QFileDialog::getSaveFileName(this, "Export to Text File", QString(), "Text Files (*.txt)");

    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing.";
        return;
    }

    QTextStream out(&file);

    // Retrieve data from the database
    QSqlQuery query;
    if (!query.exec("SELECT nom, duree, lieu, categorie, date_pod FROM tableView")) {
        qDebug() << "Error executing SQL query:" << query.lastError().text();
        return;
    }

    // Write data to file
    while (query.next()) {
        for (int col = 0; col < query.record().count(); ++col) {
            out << query.value(col).toString();
            if (col < query.record().count() - 1)
                out << "\t"; // Use tab as a delimiter for text file
        }
        out << "\n";
    }

    file.close();

    qDebug() << "Database exported to text file successfully.";

    */

/*

QString filePath = QFileDialog::getSaveFileName(this, "Export to Excel", QString(), "Excel Files (*.xlsx)");

if (filePath.isEmpty())
    return;

QAxObject *excel = new QAxObject("Excel.Application", this);

if (excel->isNull()) {
    // Handle the case when Excel is not available
    delete excel;
    return;
}

QAxObject *workbooks = excel->querySubObject("Workbooks");
QAxObject *workbook = workbooks->querySubObject("Add()");
QAxObject *worksheets = workbook->querySubObject("Worksheets(int)", 1);
QAxObject *worksheet = worksheets->querySubObject("Cells(int, int)", 1, 1);

QStringList headerData = {"Nom", "Duree", "Lieu", "Categorie", "Date"}; // New header names

// Write headers
int col = 1;
foreach (const QString &header, headerData)
{
    worksheet->querySubObject("Cells(int, int)", 1, col++)->setProperty("Value", header);
}

// Write data
int row = 2; // Start from the second row

QString nom = ui->lineEdit_nom->text();
QString duree = ui->lineEdit_duree->text();
QString lieu = ui->lineEdit_lieu->text();
QString categorie = ui->lineEdit_categorie->text();
QString date_pod = ui->dateEdit->text();

worksheet->querySubObject("Cells(int, int)", row, 1)->setProperty("Value", nom);
worksheet->querySubObject("Cells(int, int)", row, 2)->setProperty("Value", duree);
worksheet->querySubObject("Cells(int, int)", row, 3)->setProperty("Value", lieu);
worksheet->querySubObject("Cells(int, int)", row, 4)->setProperty("Value", categorie);
worksheet->querySubObject("Cells(int, int)", row, 5)->setProperty("Value", date_pod);

workbook->dynamicCall("SaveAs(const QString&)", filePath);
workbook->dynamicCall("Close()");
excel->dynamicCall("Quit()");

// Introduce a delay before deleting or moving the file
//QThread::msleep(1000); // 1000 milliseconds (1 second) delay

delete excel;

qDebug() << "Debug Information";
*/

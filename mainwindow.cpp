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
    ui->lineEdit_nomp->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
    //ui->lineEdit_categoriep->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
    ui->lineEdit_dureep->setValidator(new QRegExpValidator(QRegExp("[0-9]*\\.?([0-9]+)?"), this));
    //ui->lineEdit_7p->setValidator(new QIntValidator(0, 99999999, this));
    //ui->lineEdit_8p->setValidator(new QIntValidator(0, 99999999, this));

    ui->comboBoxp->addItem("Education");
    ui->comboBoxp->addItem("Sports");
    ui->comboBoxp->addItem("Finance");
    ui->comboBoxp->addItem("Politiques");


    podcast p;

           model = new QStandardItemModel(this);
            ui->tableViewp->setModel(model);

              model->setColumnCount(5); // Nous avons 5 colonnes dans notre tableViewp
                 model->setHorizontalHeaderLabels({"ID", "Nom du Podcast", "Duree (min)", "Lieu", "Categorie", "Date Podcast"});
                 ui->tableViewp->setModel(model);
                 ui->tableViewp->hideColumn(0);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ajouterpod_clicked()
{

    /*
    QString nom = ui->lineEdit_nomp->text();
    QString duree = ui->lineEdit_dureep->text();
    QString lieu = ui->lineEdit_lieup->text();
    //QString categorie = ui->lineEdit_categoriep->text();
    QString categorie = ui->comboBoxp->currentText();
    QString date_pod = ui->dateEditp->text();
    int id = ui->lineEdit_7p->text().toInt(); // Assuming this is the employee ID

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
         success = p.ajouterp();
    } else {
         // Sinon, nous mettons à jour un enregistrement existant
         success = p.update(id,nom, duree, lieu, categorie, date_pod);
         ui->lineEdit_7p->clear();
         QMessageBox::information(nullptr, QObject::tr("OK"),
                       QObject::tr("Modification  effectué\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);
    }
   QMessageBox msgBox;
    if (success) {
      //  msgBox.setText("Operation successful.");
        ui->tableViewp->setModel(p.afficherp());
        ui->lineEdit_nomp->clear();
        ui->lineEdit_dureep->clear();
        ui->lineEdit_lieup->clear();
        //ui->lineEdit_categoriep->clear();
        ui->dateEditp->clear();
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
    */

}

void MainWindow::on_pushButton_annulerpod_clicked()
{
    ui->lineEdit_nomp->clear();
    ui->lineEdit_dureep->clear();
    ui->lineEdit_lieup->clear();
    //ui->lineEdit_categoriep->clear();
    ui->dateEditp->clear();
}

void MainWindow::on_pushButton_supprimerpod_clicked()
{

    QString nom = ui->lineEdit_7p->text();
    // Obtenez le nom entré

    podcast p;
    QSqlQuery query;
    //query.prepare("SELECT id_podcast, nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE nom = ?");
    query.prepare("SELECT nom FROM PODCASTS WHERE nom = ?");
    query.addBindValue(nom);

    if (query.exec() && query.next()) {
        // Récupérez les valeurs de la requête
        //int id = query.value(0).toInt(); // Récupérez l'ID du podcast pour la suppression
        QString nom = query.value(0).toString();
        QString duree = query.value(1).toString();
        QString lieu = query.value(2).toString();
        QString categorie = query.value(2).toString();
        QString date_pod = query.value(4).toString();


        // Utilisez les valeurs récupérées pour instancier un objet podcast
        p = podcast(nom, duree, lieu, categorie, date_pod);

        // Supprimer le podcast
        if (p.supprimerp(nom)) {
            ui->tableViewp->setModel(p.afficherp());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                QObject::tr("Suppression effectuée\n"
                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            // Afficher un message indiquant que le podcast est introuvable
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                QObject::tr("Podcast inexistant.\n"
                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    } else {
        // Afficher un message indiquant que le podcast est introuvable
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
            QObject::tr("Podcast inexistant.\n"
            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}



void MainWindow::on_pushButton_modifierpod_clicked()
{
    QString nom = ui->lineEdit_7p->text();
    // Obtenez le nom entré

    podcast p;
    QSqlQuery query;
    query.prepare("SELECT  nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE nom = ?");
    query.addBindValue(nom);

    if (query.exec() && query.next()) {
        // Récupérez les valeurs de la requête
       // int id = query.value(0).toInt(); // Récupérez le nom du podcast pour la modification
        QString nom = query.value(0).toString();
        QString duree = query.value(1).toString();
        QString lieu = query.value(2).toString();
        QString categorie = query.value(2).toString();
        QString date_pod = query.value(4).toString();

        // Affichez les informations du podcast dans les champs de saisie pour permettre la modification
        ui->lineEdit_nomp->setText(nom);
        ui->lineEdit_dureep->setText(duree);
        ui->lineEdit_lieup->setText(lieu);
        //ui->lineEdit_categoriep->setText(categorie);
        ui->comboBoxp->setCurrentText(categorie);
        ui->dateEditp->setDate(QDate::fromString(date_pod, "yyyy-MM-dd"));

        // Mettez à jour le podcast avec les nouvelles valeurs si l'utilisateur modifie les champs et appuie sur un bouton de confirmation
        if (ui->pushButton_ajouterpod->isEnabled()) {
            QString newNom = ui->lineEdit_nomp->text();
            QString newDuree = ui->lineEdit_dureep->text();
            QString newLieu = ui->lineEdit_lieup->text();
            //QString newCategorie = ui->lineEdit_categoriep->text();
            QString newDatePod = ui->dateEditp->text();

            /*
            if (p.update(nom, newNom, newDuree, newLieu, newCategorie, newDatePod)) {
                ui->tableViewp->setModel(p.afficherp());
                */
                QMessageBox::information(nullptr, QObject::tr("OK"),
                                         QObject::tr("Modification effectuée.\n"
                                                     "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
            } else {
                QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                      QObject::tr("Échec de la modification du podcast.\n"
                                                  "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
            }
        }
     else {
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
    historyText += "ID du podcast modifié : " + ui->lineEdit_7p->text() + "\n";
    historyText += "Nouveau nom : " + ui->lineEdit_nomp->text() + "\n";
    historyText += "Nouvelle durée : " + ui->lineEdit_dureep->text() + "\n";
    historyText += "Nouveau lieu : " + ui->lineEdit_lieup->text() + "\n";
    //historyText += "Nouvelle catégorie : " + ui->lineEdit_categoriep->text() + "\n";
    historyText += "Nouvelle catégorie : " + ui->comboBoxp->currentText() + "\n";
    historyText += "Nouvelle date du podcast : " + ui->dateEditp->text() + "\n";
    historyText += "-------------------------------------\n\n";

    // Écriture de l'historique dans le fichier texte
    QTextStream out(&file);
    out << historyText;

    // Fermeture du fichier
    file.close();

    // Affichage d'un message pour informer l'utilisateur que l'historique a été enregistré avec succès
    QMessageBox::information(this, "Historique enregistré", "L'historique des modifications a été enregistré avec succès dans le fichier.");
}

void MainWindow::on_pushButton_recherchep_clicked()
{
    podcast p;

    QString nom = ui->lineEdit_8p->text(); // Obtenez le nom entré

    QSqlQuery query;
    query.prepare("SELECT nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE nom = ?");
    query.addBindValue(nom);

    if (query.exec() && query.next()) {
        // Récupérez les valeurs de la requête
        QString nom = query.value(0).toString();
        QString duree = query.value(1).toString();
        QString lieu = query.value(2).toString();
        QString categorie = query.value(3).toString();
        QString date_pod = query.value(4).toString();

        // Affichez les informations récupérées où vous le souhaitez, par exemple :
        qDebug() << "Nom du podcast:" << nom;
        qDebug() << "Durée:" << duree;
        qDebug() << "Lieu:" << lieu;
        qDebug() << "Catégorie:" << categorie;
        qDebug() << "Date du podcast:" << date_pod;

        // Maintenant, affichez ces informations dans votre TableView
        // Vous pouvez les mettre dans un modèle ou directement dans votre TableView


        QMessageBox::information(nullptr, QObject::tr("OK"),
                        QObject::tr("Podcast trouvé.\n"
                                    "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    } else {
        // Afficher un message indiquant que le podcast est introuvable
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                QObject::tr("Podcast introuvable.\n"
                                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    ui->lineEdit_8p->clear();
}


void MainWindow::on_pushButton_trierpod_clicked() //tri
{
    podcast p;

      ui->tableViewp->setModel(p.trip());

      bool test=p.trip();
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




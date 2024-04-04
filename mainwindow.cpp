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
#include <QFile>
#include <QRegularExpression>
#include <QPainter>
#include <QPrinter>
#include <QStringList>
#include <QDateTime>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QPdfWriter>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->lineEdit_nomp->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
    ui->lineEdit_dureep->setValidator(new QIntValidator(0, 999, this));
    ui->lineEdit_lieup->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
    ui->comboBoxp->currentText();
    ui->dateEditp->text();
    ui->lineEdit_7p->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
    ui->lineEdit_8p->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));


    ui->comboBoxp->addItem("Education");
    ui->comboBoxp->addItem("Sports");
    ui->comboBoxp->addItem("Finance");
    ui->comboBoxp->addItem("Politiques");


    podcast p;

           model = new QStandardItemModel(this);
            ui->tableViewp->setModel(model);

              model->setColumnCount(5); // Nous avons 5 colonnes dans notre tableViewp
                 model->setHorizontalHeaderLabels({"ID", "Nom du Podcast", "Durée min", "Lieu", "Categorie", "Date Podcast"});
                 ui->tableViewp->setModel(model);
                 ui->tableViewp->hideColumn(0);
}


MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_ajouterpod_clicked()
{
    QString nom = ui->lineEdit_nomp->text();
    QString duree = ui->lineEdit_dureep->text();
    QString lieu = ui->lineEdit_lieup->text();
    QString categorie = ui->comboBoxp->currentText();
    QString date_pod = ui->dateEditp->text();
    //int id = ui->lineEdit_7p->text().toInt(); // Assuming this is the employee ID
    int id = ui->lineEdit_7p->text().toInt();
    // Vérifier si les champs obligatoires sont vides
    if (nom.isEmpty() || duree.isEmpty() || lieu.isEmpty() || categorie.isEmpty() || date_pod.isEmpty()) {
        QMessageBox::warning(this, "Warning", "veuillez remplir toutes les cases.");
        return;
    }

    // Ajouter ici les expressions régulières pour la validation des champs si nécessaire

    podcast p(nom, duree, lieu, categorie, date_pod);
    bool success = false;

    if (ui->lineEdit_7p->text() == "") {
         // Si l'ID est 0, cela signifie que nous ajoutons un nouvel enregistrement
         success = p.ajouterp();
    } else {
         // Sinon, nous mettons à jour un enregistrement existant

         success = p.update(id,nom, duree, lieu, categorie, date_pod);
        //success = p.update(nom, newNom, newDuree, newLieu, newCategorie, newDatePod);
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
    QFile file("C:/Users/ISMAIL/Desktop/crud/Historique.txt");
            if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            {
                QTextStream cout(&file);
                QString d_info = QDateTime::currentDateTime().toString();
                QString message = d_info + " - Un PODCAST avec le Nom  :" + nom + " a été supprimé\n";
                cout << message;
                file.close();
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
        QString nom = query.value(0).toString();
        QString duree = query.value(1).toString();
        QString lieu = query.value(2).toString();
        QString categorie = query.value(2).toString();
        QString date_pod = query.value(4).toString();

        // Affichez les informations du podcast dans les champs de saisie pour permettre la modification
        ui->lineEdit_nomp->setText(nom);
        ui->lineEdit_dureep->setText(duree);
        ui->lineEdit_lieup->setText(lieu);
        ui->comboBoxp->setCurrentText(categorie);
        ui->dateEditp->setDate(QDate::fromString(date_pod, "yyyy-MM-dd"));

        // Mettez à jour le podcast avec les nouvelles valeurs si l'utilisateur modifie les champs et appuie sur un bouton de confirmation
        if (ui->pushButton_ajouterpod->isEnabled()) {
            QString newNom = ui->lineEdit_nomp->text();
            QString newDuree = ui->lineEdit_dureep->text();
            QString newLieu = ui->lineEdit_lieup->text();
            QString newDatePod = ui->dateEditp->text();


           // if (p.update(nom, newNom, newDuree, newLieu, newCategorie, newDatePod)) {
              //  ui->tableViewp->setModel(p.afficherp());


            // Use more professional-looking QMessageBox dialogs
            QMessageBox::StandardButton reply = QMessageBox::question(this, QObject::tr("Confirmation"),
                                                                      QObject::tr("Voulez-vous vraiment modifier le podcast sélectionné ?"),
                                                                      QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                   // Mettre à jour le podcast avec les nouvelles valeurs
                   QString newNom = ui->lineEdit_nomp->text();
                   QString newDuree = ui->lineEdit_dureep->text();
                   QString newLieu = ui->lineEdit_lieup->text();
                   QString newDatePod = ui->dateEditp->text();
            }

        }
    } else {
        // If the podcast was not found
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Le podcast sélectionné est introuvable."), QMessageBox::Ok);
    }


    QFile file("C:/Users/ISMAIL/Desktop/crud/Historique.txt");
                                if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                                    QTextStream cout(&file);
                                    QString d_info = QDateTime::currentDateTime().toString();
                                    QString message = d_info + " - Modification d'une entrée dans la table PODCAST avec le Nom :" + QString(nom) + "\n";
                                    cout << message;
                                    file.close();
                                } else {
                                    qDebug() << "Failed to open history file for writing";
                                }


}





void MainWindow::on_pushButton_expo_clicked()
{
    // Obtenir le chemin du fichier de sauvegarde
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter vers PDF", QString(), "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty())
        return;

    // Créer un QPdfWriter avec le chemin du fichier
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4)); // Définir la taille de la page

    // Ouvrir le fichier PDF pour l'écriture
    QPainter painter;
    if (!painter.begin(&pdfWriter)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier PDF pour l'impression.");
        return;
    }

    // Définir les styles de peinture
    QPen pen(Qt::black); // Stylo noir pour les traits
    QFont font("Arial", 12); // Police Arial, taille 12 pour le contenu principal
    QFont headerFont("Arial", 12, QFont::Bold); // Police Arial, taille 12 en gras pour l'en-tête du tableau
    QColor headerColor("#C0C0C0"); // Couleur de fond de l'en-tête

    // Appliquer les styles de peinture
    painter.setPen(pen);
    painter.setFont(font);

    // Écrire le titre dans le fichier PDF
    painter.drawText(100, 50, "Liste des podcasts :\n\n");

    // Calculer la taille de la page
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();

    // Calculer les dimensions des colonnes et des lignes en fonction de la taille de la page
    int columnWidth = pageWidth / 6; // Diviser la largeur de la page par le nombre de colonnes
    int rowHeight = pageHeight / 30; // Diviser la hauteur de la page par le nombre de lignes

    // Parcourir les enregistrements de podcast et les écrire dans le fichier PDF
    QSqlQuery query("SELECT * FROM PODCASTS");
    int yPos = pageHeight / 10; // Position verticale de départ
    int xPos = pageWidth / 80; // Position horizontale de départ

    // Dessiner l'en-tête du tableau
    painter.setFont(headerFont);
    painter.setBrush(headerColor); // Définir la couleur de fond de l'en-tête
    painter.drawRect(xPos, yPos - rowHeight, 6 * columnWidth, rowHeight); // Dessiner un rectangle pour l'en-tête

    // Écrire les en-têtes de colonne
    QStringList headers = {"ID", "Nom", "Durée", "Lieu", "Catégorie", "Date du Podcast"};
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(xPos + i * columnWidth, yPos - rowHeight, columnWidth, rowHeight, Qt::AlignCenter, headers[i]);
    }

    // Réinitialiser les styles pour le contenu
    painter.setFont(font);
    painter.setBrush(Qt::NoBrush); // Réinitialiser la couleur de fond

    // Parcourir les enregistrements de podcast et les écrire dans le fichier PDF
    while (query.next()) {
        for (int i = 0; i < 6; ++i) {
            QString value = query.value(i).toString();
            painter.drawText(xPos + i * columnWidth, yPos, columnWidth, rowHeight, Qt::AlignCenter, value);
        }
        yPos += rowHeight; // Aller à la ligne suivante
    }

    // Fermer le fichier PDF
    painter.end();

    QMessageBox::information(this, "Succès", "Les données ont été exportées vers PDF avec succès.");

    // Enregistrer l'historique dans un fichier
    QFile historyFile("C:/Users/ISMAIL/Desktop/crud/Historique.txt");
    if (historyFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&historyFile);
        QString timestamp = QDateTime::currentDateTime().toString();
        QString logMessage = timestamp + " - Exporté les données du table PODCAST en PDF\n";
        out << logMessage;
        historyFile.close();
    }
}





void MainWindow::on_pushButton_historique_clicked()
{
    p.afficherHistorique();
}






void MainWindow::on_pushButton_recherchep_clicked()
{
    QString nom = ui->lineEdit_8p->text(); // Obtenez le nom entré

    QSqlQuery query;
    query.prepare("SELECT nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE nom = ?");
    query.addBindValue(nom);

    if (query.exec() && query.next()) {
        // Récupérer les valeurs de la requête
        QString nomTrouve = query.value(0).toString();
        QString duree = query.value(1).toString();
        QString lieu = query.value(2).toString();
        QString categorie = query.value(3).toString();
        QString date_pod = query.value(4).toString();

        // Création d'un modèle pour stocker les résultats
        QStandardItemModel *model = new QStandardItemModel(1, 6, this);
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Nom")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Nom du Podcast")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Durée")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Lieu")));
        model->setHorizontalHeaderItem(4, new QStandardItem(QString("Catégorie")));
        model->setHorizontalHeaderItem(5, new QStandardItem(QString("Date du podcast")));

        // Ajouter les valeurs récupérées au modèle
        model->setItem(0, 0, new QStandardItem(nomTrouve));
        model->setItem(0, 1, new QStandardItem(nomTrouve));
        model->setItem(0, 2, new QStandardItem(duree));
        model->setItem(0, 3, new QStandardItem(lieu));
        model->setItem(0, 4, new QStandardItem(categorie));
        model->setItem(0, 5, new QStandardItem(date_pod));

        // Appliquer le modèle à votre TableView
        ui->tableViewp->setModel(model);
    } else {
        // Afficher un message indiquant que le podcast est introuvable
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                QObject::tr("Podcast introuvable.\n"
                                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    ui->lineEdit_8p->clear();
    QFile file("C:/Users/ISMAIL/Desktop/crud/Historique.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString();
        QString logMessage = timestamp + " - Recherche effectuée avec le Nom  : " + nom + "\n";
        out << logMessage;
        file.close();
    }
}




//

void MainWindow::on_pushButton_trierpod_clicked() //tri
{
    podcast p;

    // Effectuer le tri et mettre à jour le modèle de la vue
    ui->tableViewp->setModel(p.trip());

    // Vérifier si le tri a été effectué avec succès
    bool test=p.trip();

    // Afficher un message indiquant si le tri a été effectué ou non
    if(test){
        QMessageBox::information(nullptr, tr("OK"), tr("Tri ascendant effectué. \nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
    else {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Tri ascendant non effectué.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    // Enregistrer l'historique dans un fichier
    QFile file("C:/Users/ISMAIL/Desktop/crud/Historique.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString();
        QString logMessage = timestamp + " - Tri effectué en ordre ascendant.\n";
        out << logMessage;
        file.close();
    }
    else {
        // Gérer l'échec de l'ouverture du fichier si nécessaire
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Impossible d'ouvrir le fichier d'historique.\nVérifiez les autorisations et les chemins."), QMessageBox::Cancel);
    }
}




void MainWindow::on_pushButton_trierpod_desc_clicked()
{
    podcast p;

    // Effectuer le tri descendant et mettre à jour le modèle de la vue
    ui->tableViewp->setModel(p.tripDesc());

    // Vérifier si le tri a été effectué avec succès
    bool test =p.tripDesc();

    // Afficher un message indiquant que le tri a été effectué ou non
    if(test){
        QMessageBox::information(nullptr, tr("OK"), tr("Tri descendant effectué. \nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
    else {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Tri descendant non effectué.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    // Enregistrer l'historique dans un fichier
    QFile file("C:/Users/ISMAIL/Desktop/crud/Historique.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString();
        QString logMessage = timestamp + " - Tri effectué en ordre descendant.\n";
        out << logMessage;
        file.close();
    }
    else {
        // Gérer l'échec de l'ouverture du fichier si nécessaire
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Impossible d'ouvrir le fichier d'historique.\nVérifiez les autorisations et les chemins."), QMessageBox::Cancel);
    }
}


/*
void MainWindow::on_pushButton_chatbox_clicked()
{
    QString message = ui->lineEdit_chatbox->text();

    if (message.isEmpty()) {
        QMessageBox::warning(this, tr("Message vide"), tr("Veuillez saisir un message à envoyer."), QMessageBox::Ok);
        return;
    }

    QModelIndexList selectedIndexes = ui->listView_chat->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, tr("Destinataire non sélectionné"), tr("Veuillez sélectionner un destinataire dans la liste."), QMessageBox::Ok);
        return;
    }

    QModelIndex index = selectedIndexes.first();
    QString destinataire = index.data(Qt::DisplayRole).toString();

    // Affichage du message pour le test
    QMessageBox::information(this, tr("Message envoyé"), tr("Message envoyé à : ") + destinataire + tr("\nContenu : ") + message, QMessageBox::Ok);

    // Effacer le contenu du lineEdit_chatbox après l'envoi du message
    ui->lineEdit_chatbox->clear();
}
*/


void MainWindow::on_pushButton_chatbox_clicked() {
    // Get the message from lineEdit_chatbox
    QString message = ui->lineEdit_chatbox->text().trimmed(); // Trimmed to remove leading and trailing spaces

    // Validate message
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a message.");
        return;
    }

    // Get current time in hours and minutes
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm");

    // Create a new list widget item with formatted message and timestamp
    QListWidgetItem *item = new QListWidgetItem(message + " [" + timestamp + "]");
    item->setTextAlignment(Qt::AlignLeft); // Align message to the left
    QFont font("Arial", 9); // Italian font
    item->setFont(font);
    item->setTextColor(Qt::black); // Black text color

    // Add the item to listWidget_chat
    ui->listWidget_chat_2->addItem(item);

    // Clear lineEdit_chatbox after adding the message
    ui->lineEdit_chatbox->clear();

    // Scroll to the bottom of the list widget
    ui->listWidget_chat->scrollToBottom();
}

void MainWindow::on_pushButton_chatbox_2_clicked() {
    // Get the message from lineEdit_chatbox_2
    QString message = ui->lineEdit_chatbox_2->text().trimmed(); // Trimmed to remove leading and trailing spaces

    // Validate message
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a message.");
        return;
    }

    // Get current time in hours and minutes
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm");

    // Create a new list widget item with formatted message and timestamp
    QListWidgetItem *item = new QListWidgetItem(message + " [" + timestamp + "]");
    item->setTextAlignment(Qt::AlignLeft); // Align message to the left
    QFont font("Arial", 9); // Italian font
    item->setFont(font);
    item->setTextColor(Qt::black); // Black text color

    // Add the item to listWidget_chat_2
    ui->listWidget_chat->addItem(item);

    // Clear lineEdit_chatbox_2 after adding the message
    ui->lineEdit_chatbox_2->clear();

    // Scroll to the bottom of the list widget
    ui->listWidget_chat_2->scrollToBottom();
}

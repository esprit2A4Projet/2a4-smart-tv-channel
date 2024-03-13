#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "employee.h"
#include "invite.h"
#include "sponsor.h"
#include "materiaux.h"
#include "connection.h"
#include <QInputDialog>
#include <QTextStream>

#include <QDesktopServices>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextDocument>

#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QTableWidget>

#include <QKeyEvent>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Employee E;
       ui->setupUi(this);
       ui->lineEdit_nom->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
       ui->lineEdit_prenom->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
       ui->lineEdit_salaire->setValidator(new QRegExpValidator(QRegExp("[0-9]*\\.?([0-9]+)?"), this));
       ui->lineEdit_cin->setValidator(new QIntValidator(0, 99999999, this));
       ui->lineEdit_3->setValidator(new QIntValidator(0, 99999999, this));
       ui->lineEdit_8->setValidator(new QIntValidator(0, 99999999, this));

       ui->comboBox->addItem("Ressources Humaines");
       ui->comboBox->addItem("R. podcast");
       ui->comboBox->addItem("R. sponsoring");
       ui->comboBox->addItem("tresorier");
       ui->comboBox->addItem("C.Invités ");
       ui->comboBox->addItem("R.logistique ");
       ui->comboBox->addItem("Autre");
       // Connectez le signal textChanged à une fonction de vérification personnalisée
       connect(ui->lineEdit_email, &QLineEdit::textChanged, this, &MainWindow::checkEmailFormat);


       model = new QStandardItemModel(this);
       ui->tableView->setModel(model);

       model->setColumnCount(6); // Nous avons 5 colonnes dans notre TableView
       model->setHorizontalHeaderLabels({ "Nom", "Prénom","email", "Date Embauche", "Salaire", "Poste","cin"});
       ui->tableView->setModel(model);

// LOGIN :

     //ui->tableView->setModel(e.afficher());
       ui->label_gif_animation->setVisible(true);
       // Spécifiez le chemin absolu complet du fichier GIF
       QString gifPath = "C:/Users/MSI/OneDrive - ESPRIT/Bureau/conneter.gif";
       // Créez le QMovie en utilisant le chemin absolu complet
       QMovie *gifanimation = new QMovie(gifPath);
       // Définissez le QMovie sur le QLabel
       ui->label_gif_animation->setMovie(gifanimation);
       // Démarrez l'animation
       gifanimation->start();


 // Ibtissem :

       connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget())); // connecter le signal envers le slot

       //initialiser la table
       ui->tableWidget->setColumnCount(7);
       QStringList labels{"ID", "Nom", "Prenom", "Profession", "Email", "Nombre d'abonnes", "Nombre de Participation"};
       ui->tableWidget->setHorizontalHeaderLabels(labels);

       Connection c;
       if (c.createconnect()) {
           QString searchTerm =ui->lineEdit_rechercherInvite->text();
           updateTableWidgetInvite(searchTerm);
       } else {
          // showCustomMessageBox("Error", "Failed to connect to the database.");
       }

       connect(ui->pushButton_annulerInvite, &QPushButton::clicked, this, &MainWindow::on_pushButton_annulerInvite_clicked);
       //controle de saisie

       ui->label_emailError->setVisible(false);
       ui->label_nbAbonnesError->setVisible(false);
       ui->label_nbParticipationError->setVisible(false);
       ui->label_nomError->setVisible(false);
       ui->label_prenomError->setVisible(false);
       ui->label_professionError->setVisible(false);

       connect(ui->lineEdit_emailI, &QLineEdit::textChanged, this, &MainWindow::validateEmail);
       connect(ui->lineEdit_nbAbonnesI, &QLineEdit::textChanged, this, &MainWindow::validateNbAbonnes);
       connect(ui->lineEdit_nbParticipationI, &QLineEdit::textChanged, this, &MainWindow::validateNbParticipation);
       connect(ui->lineEdit_nomI, &QLineEdit::textChanged, this, &MainWindow::validateNom);
       connect(ui->lineEdit_prenomI, &QLineEdit::textChanged, this, &MainWindow::validatePrenom);
       connect(ui->lineEdit_professionI, &QLineEdit::textChanged, this, &MainWindow::validateProfession);

       ui->tableWidget->hideColumn(0); // Cette ligne masque la première colonne

   // Mokh :
       connect(this, SIGNAL(dataUpdatedS()), this, SLOT(updateTableWidget_S())); // Connect the signal to the slot

       // Initialize the table widget
       ui->tableWidget_S->setColumnCount(7);
       QStringList labelsS{"ID","Nom", "Budget", "Pack", "Date debut", "Date fin", "Telephone"};
       ui->tableWidget_S->setHorizontalHeaderLabels(labelsS);

    // Zoh :
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


       // louay :
       //-----------------------------------installation des reflexes selon les signaux--------------------------------//
       ui->lineEdit_Montant_Transaction->installEventFilter(this);
       installEventFilter(this);
       connect(ui->table_de_transactions, &QTableWidget::cellDoubleClicked, this, &MainWindow::preventCellEdit_Transactions);
       connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::showEditChoices_Transactions);
       connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::handleCellClicked_Transactions);
       connect(ui->lineEdit_RechercheParMONTANT_Transaction, &QLineEdit::textChanged, this, &MainWindow::searchTransactionsByAmount_Transaction);
       connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::editMontantCell_Transactions);
       connect(this, SIGNAL(signal_table_de_transaction_mise_a_jour()), this, SLOT(update_table_de_transactions()));
     //  ui->listView->hide();
     //  connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::afficherListeNotifications);
       //---------------------------------------------------------------------------------------------------------------//

//--------------------------------Initialisation du tableau et ses champs----------------------------------------------//
       ui->table_de_transactions->setColumnCount(6);
       QStringList labelsT{"Mode de paiement", "Type", "Categorie", "Date de transaction", "Montant en dinar","ID"};
       ui->table_de_transactions->setHorizontalHeaderLabels(labelsT);
//---------------------------------------------------------------------------------------------------------------------//
// Ismail :
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

// Feriel :
// Définition de la fonction de vérification personnalisée
void MainWindow::checkEmailFormat(const QString &text)
{
    if (!text.contains('@')) {

        // Si l'email ne contient pas '@', ne rien effacer
        ui->lineEdit_email->setStyleSheet("background-color: red"); // Mettre en évidence en rouge pour indiquer une erreur

    } else {
        ui->lineEdit_email->setStyleSheet("background-color: white"); // Rétablir le style par défaut
    }
}




void MainWindow::on_pushButton_23_clicked() // Bouton Valider
{
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString date = ui->dateEdit->text();
    QString email = ui->lineEdit_email->text();
    int cin = ui->lineEdit_cin->text().toInt();
    int salaire = ui->lineEdit_salaire->text().toInt();
    QString poste = ui->comboBox->currentText();

    int id = ui->lineEdit_3->text().toInt(); // Assuming this is the employee ID
    Employee e(nom, prenom,email, date, salaire, poste,cin);
    bool success = false;

    if (id == 0)
    {
         // If the ID is 0, it means we are creating a new employee
         success = e.ajouter();
    } else
    {
         // Otherwise, we are updating an existing employee
         success = e.update(id);
         ui->lineEdit_3->clear();
         QMessageBox::information(nullptr, QObject::tr("OK"),
                       QObject::tr("Modification  effectué\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);
    }
   QMessageBox msgBox;
    if (success)
    {
      //  msgBox.setText("Operation successful.");
        ui->tableView->setModel(e.afficher());
        ui->lineEdit_nom->clear();
        ui->lineEdit_prenom->clear();
        ui->lineEdit_email->clear();
        ui->dateEdit->clear();
        ui->lineEdit_salaire->clear();
        ui->lineEdit_cin->clear();
    } else {
        msgBox.setText("Operation failed.");
          msgBox.exec();
    }

}


void MainWindow::on_pushButton_22_clicked() // Button annuler
{
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->dateEdit->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_cin->clear();
    ui->lineEdit_email->clear();
}



void MainWindow::on_pushButton_71_clicked() // Afficher
{ Employee E;
      ui->tableView->setModel(E.afficher());
}

void MainWindow::on_pushButton_67_clicked()  // bouton supprimer
{
    // Obtenez l'ID entré
       int id = ui->lineEdit_3->text().toInt();


           Employee E;
           QSqlQuery query;
           query.prepare("SELECT nom, prenom,email, date_embauche, salaire, poste,cin FROM employes WHERE cin = ?");
           query.addBindValue(id);

           if (query.exec() && query.next()) {
               // Récupérez les valeurs de la requête
               QString nom = query.value(0).toString();
               QString prenom = query.value(1).toString();
               QString email = query.value(2).toString();
               QString date_embauche = query.value(3).toString();
               int salaire = query.value(4).toInt();
               QString poste = query.value(5).toString();
               int cin = query.value(6).toInt();
               Employee(nom,prenom,email,date_embauche,salaire,poste,cin);

               if (E.supprimer(id))
               {
                   ui->tableView->setModel(E.afficher());
                   QMessageBox::information(nullptr, QObject::tr("OK"),
                               QObject::tr("Suppression effectuée\n"
                                           "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
               } else
               {
                   // Afficher un message indiquant que la suppression a échoué
                   QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                       QObject::tr("La suppression a échoué.\n"
                                                   "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
               }
           }
           else
           {
               // Afficher un message indiquant que l'employé est introuvable
               QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                   QObject::tr("Employé introuvable.\n"
                                               "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
           }
           ui->lineEdit_3->clear();



}



void MainWindow::on_pushButton_69_clicked() // Tri
{
    Employee E;

      ui->tableView->setModel(E.tri());

      bool test=E.tri();
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


void MainWindow::on_pushButton_68_clicked() {  //Bouton  MODIFIER

    int id = ui->lineEdit_3->text().toInt();

    // Obtenez l'ID entré
    Employee E;
    QSqlQuery query;
    query.prepare("SELECT nom, prenom,email, date_embauche, salaire, poste ,cin FROM employes WHERE cin = ?");
    query.addBindValue(id);

    if (query.exec() && query.next())
    {
        // Récupérez les valeurs de la requête
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        QString email = query.value(2).toString();
        QString date_embauche = query.value(3).toString();
        int salaire = query.value(4).toInt();
        QString poste = query.value(5).toString();
        int cin = query.value(6).toInt();


        // Afficher les informations de l'employé dans les champs de texte
        ui->lineEdit_nom->setText(nom);
        ui->lineEdit_prenom->setText(prenom);
        ui->lineEdit_email->setText(email);
        ui->dateEdit->setDate(QDate::fromString(date_embauche, "yyyy-MM-dd"));
        ui->lineEdit_salaire->setText(QString::number(salaire));
        ui->comboBox->setCurrentText(poste);
        ui->lineEdit_cin->setText(QString::number(cin));
        Employee(nom,prenom,email,date_embauche,salaire,poste,cin);
    // Récupérer les informations de l'employé pour l'ID donné
         /*   if (E.update(id))
            {
                 ui->tableView->setModel(E.afficher());
             }
            else
            {
                    // Afficher un message indiquant que l'employé est introuvable
                    QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                          QObject::tr("Employé inexistant.\n"
                                                      "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            }*/
     }
      else
    {
                // Afficher un message indiquant que l'employé est introuvable
                QMessageBox::critical(nullptr, QObject::tr("Error"),
                                      QObject::tr("Employé inexistant !!!.\n"
                                                  "Click Cancel to exit."), QMessageBox::Cancel);
     }


}






void MainWindow::on_pushButton_70_clicked() // chercher
{
    Employee E;

    int id = ui->lineEdit_8->text().toInt();
   // Obtenez l'ID entré

      QSqlQuery query;
      query.prepare("SELECT nom, prenom,email, date_embauche, salaire, poste,cin FROM employes WHERE cin = ?");
      query.addBindValue(id);

      if (query.exec() && query.next()) {
          // Récupérez les valeurs de la requête
          QString nom = query.value(0).toString();
          QString prenom = query.value(1).toString();
          QString email = query.value(2).toString();
          QString date_embauche = query.value(3).toString();
          int salaire = query.value(4).toInt();
          QString poste = query.value(5).toString();
          int cin = query.value(6).toInt();


          Employee(nom,prenom,email,date_embauche,salaire,poste,cin);
          if (E.Rechercher(id)) {
               ui->tableView->setModel(E.Rechercher(id));
              QMessageBox::information(nullptr, QObject::tr("OK"),
                            QObject::tr("Employé  Trouvé\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
              } else {
                  // Afficher un message indiquant que l'employé est introuvable
                  QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                        QObject::tr("Employé inexistant.\n"
                                                    "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
              }
          } else {
              // Afficher un message indiquant que l'employé est introuvable
              QMessageBox::critical(nullptr, QObject::tr("Error"),
                                    QObject::tr("Employee inexistant.\n"
                                                "Click sur Cancel pour quitter."), QMessageBox::Cancel);
          }
       ui->lineEdit_8->clear();




}

void MainWindow::on_pushButton_92_clicked()  // Exportation PDF :
{
    QString strStream;
               strStream = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
                   if (QFileInfo(strStream).suffix().isEmpty())
                       { strStream.append(".pdf"); }

                   QPrinter printer(QPrinter::PrinterResolution);
                   printer.setOutputFormat(QPrinter::PdfFormat);
                   printer.setPaperSize(QPrinter::A4);
                   printer.setOutputFileName(strStream);

                         QTextStream out(&strStream);

                         const int rowCount = ui->tableView->model()->rowCount();
                         const int columnCount = ui->tableView->model()->columnCount();
                         QString TT = QDateTime::currentDateTime().toString();
                         out <<"<html>\n"
                               "<head>\n"
                                "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                             << "<title>ERP - COMmANDE LIST<title>\n "
                             << "</head>\n"
                             "<body bgcolor=#ffffff link=#5000A0>\n"
                                "<h1 style=\"text-align: center;\"><strong> "+TT+"</strong></h1>"
                               +"<img src=C:\\Users\\asus\\Desktop\\pdf\\logo_projet />" //path
                             "<h1 style=\"text-align: center;\"><strong> ****LISTE DES EMPLOYES **** </strong></h1>"

                             "<table style=\"text-align: center; font-size: 10px;\" border=1>\n "
                               "</br> </br>";
                         // headers
                         out << "<thead><tr bgcolor=#d6e5ff>";
                         for (int column = 0; column < columnCount; column++)
                             if (!ui->tableView->isColumnHidden(column))
                                 out << QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
                         out << "</tr></thead>\n";

                         // data table
                         for (int row = 0; row < rowCount; row++) {
                             out << "<tr>";
                             for (int column = 0; column < columnCount; column++)
                           {
                                 if (!ui->tableView->isColumnHidden(column)) {
                                     QString data =ui->tableView->model()->data(ui->tableView->model()->index(row, column)).toString().simplified();
                                     out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;")); //pas compriis
                                 }
                             }
                             out << "</tr>\n";
                         }
                         out <<  "</table>\n"
                             "</body>\n"
                             "</html>\n";

                         QTextDocument document;
                         document.setHtml(strStream);
                         document.print(&printer);


}

//  Ibtissem
/************************************************************************CRUD***********************************************************************************/

/*-----------------------------------------------------------------------Ajouter Un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_ajouterInvite_clicked() {
    // Valider tous les champs
    validateEmail(ui->lineEdit_emailI->text());
    validateNbAbonnes(ui->lineEdit_nbAbonnesI->text());
    validateNbParticipation(ui->lineEdit_nbParticipationI->text());
    validateNom(ui->lineEdit_nomI->text());
    validatePrenom(ui->lineEdit_prenomI->text());
    validateProfession(ui->lineEdit_professionI->text());

    // Vérifier si toutes les validations sont réussies et si tous les champs sont remplis
    if (ui->label_emailError->isVisible() ||
        ui->label_nbAbonnesError->isVisible() ||
        ui->label_nbParticipationError->isVisible() ||
        ui->label_nomError->isVisible() ||
        ui->label_prenomError->isVisible() ||
        ui->label_professionError->isVisible() ||
        ui->lineEdit_emailI->text().isEmpty() ||
        ui->lineEdit_nbAbonnesI->text().isEmpty() ||
        ui->lineEdit_nbParticipationI->text().isEmpty() ||
        ui->lineEdit_nomI->text().isEmpty() ||
        ui->lineEdit_prenomI->text().isEmpty() ||
        ui->lineEdit_professionI->text().isEmpty()) {
        showCustomMessageBox("Erreur", "Veuillez vérifier les données saisies.", QMessageBox::Critical);
        return;
    }

    // Si toutes les validations réussissent et tous les champs sont remplis, ajouter l'invite à la base de données
    Invite i;
    i.setnom(ui->lineEdit_nomI->text());
    i.setprenom(ui->lineEdit_prenomI->text());
    i.setprofession(ui->lineEdit_professionI->text());
    i.setemail(ui->lineEdit_emailI->text());
    i.setnbAbonnes(ui->lineEdit_nbAbonnesI->text().toInt());
    i.setnbParticipation(ui->lineEdit_nbParticipationI->text().toInt());

    Connection c;
    if (c.createconnect()) {
        if (i.ajouterInvite()) {
            updateTableWidgetInvite(ui->lineEdit_rechercherInvite->text());
            showCustomMessageBox("Succès", "Succès d'ajout dans la base de données", QMessageBox::Information);
        } else {
            showCustomMessageBox("Erreur", "Erreur de l'insertion dans la base de données", QMessageBox::Critical);
        }
    } else {
        showCustomMessageBox("Erreur", "Connexion impossible à la base de données.", QMessageBox::Critical);
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Mettre à jour les informations d'un Invite-------------------------------------------------------------------*/

void MainWindow::on_pushButton_updateInvite_clicked()
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
        int nbAbonnes = ui->tableWidget->item(row, 5)->text().toInt();
        int nbParticipation = ui->tableWidget->item(row, 6)->text().toInt();

        // Contrôle de saisie pour le nom et le prénom (max 10 caractères)
        if (nom.length() > 10 || prenom.length() > 10) {
            showCustomMessageBox("Erreur", "Le nom et le prénom ne doivent pas dépasser 10 caractères.", QMessageBox::Critical);
            return;
        }

        // Contrôle de saisie pour la profession (commencer par une majuscule)
        if (!profession.isEmpty() && !profession.at(0).isUpper()) {
            showCustomMessageBox("Erreur", "La profession doit commencer par une lettre majuscule.", QMessageBox::Critical);
            return;
        }

        // Contrôle de saisie pour l'email (format valide)
        if (!email.isEmpty()) {
            QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
            if (!emailRegex.exactMatch(email)) {
                showCustomMessageBox("Erreur", "Format d'email invalide.", QMessageBox::Critical);
                return;
            }
        }

        // Contrôle de saisie pour nbAbonnes et nbParticipation (entiers)
        bool nbAbonnesOk, nbParticipationOk;
        nbAbonnesOk = true;
        nbParticipationOk = true;

        if (ui->tableWidget->currentColumn() == 5) {
            nbAbonnesOk = true;
        } else if (ui->tableWidget->currentColumn() == 6) {
            nbParticipation = ui->tableWidget->item(row, 6)->text().toInt(&nbParticipationOk);
        }

        if (!nbAbonnesOk || !nbParticipationOk) {
            showCustomMessageBox("Erreur", "Veuillez entrer des nombres valides pour Nombre d'abonnes et Nombre de Participation.", QMessageBox::Critical);
            return;
        }

        // Appelez la fonction modifierInvite pour mettre à jour la base de données
        Invite i;
        if (i.modifierInvite(idToModify, nom, prenom, profession, email, nbAbonnes, nbParticipation)) {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(prenom));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(profession));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(email));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(nbAbonnes)));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(nbParticipation)));

            emit dataUpdated(); // Mettre à jour la vue après la modification
            showCustomMessageBox("Succès", "Succès de la modification dans la base de données", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Erreur", "Connexion impossible à la base de données.", QMessageBox::Critical);
        }
    }
    else
    {
        showCustomMessageBox("Attention", "Sélectionnez une ligne à modifier", QMessageBox::Warning);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Supprimer Un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_deleteInvite_clicked()
{
   QItemSelectionModel *select = ui->tableWidget->selectionModel();
   QModelIndexList selectedIndexes = select->selectedIndexes();

   if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
       QTableWidgetItem *item = ui->tableWidget->item(row, 0);
       int idToDelete = item->text().toInt();

       Invite i;
       if (i.supprimerInvite(idToDelete))
       {
           // Supprimer la ligne de l'affichage
           ui->tableWidget->removeRow(row);
           emit dataUpdated(); // Mettre à jour la vue après la suppression
           showCustomMessageBox("Success", "Data deleted from the database.", QMessageBox::Information);
       }
       else
       {
           showCustomMessageBox("Error", "Failed to delete data from the database.", QMessageBox::Critical);
       }
   }
   else
   {
       showCustomMessageBox("Warning", "Please select a row to delete.", QMessageBox::Warning);
   }
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************FONCTIONNALITES**************************************************************************************/

/*-----------------------------------------------------------------------Rechercher Un Invite----------------------------------------------------------------------------------*/

void MainWindow::on_pushButton_rechercherInvite_clicked()
{
   QString searchTerm = ui->lineEdit_rechercherInvite->text();
   updateTableWidgetInvite(searchTerm);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Trier Un Invite-----------------------------------------------------------------------------------*/
void MainWindow::on_pushButton_trierInvite_clicked()
{
    Invite i;
    if (i.trierParNbAbonnes(ui->tableWidget)) {
        emit dataUpdated();  // Emit the signal to update the table
        showCustomMessageBox("Success", "Data sorted by Nombre d'abonnes.", QMessageBox::Information);
    } else {
        showCustomMessageBox("Error", "Failed to sort data.", QMessageBox::Critical);
    }
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Exporter les informations d'un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_exporterInvite_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");

    if (!filePath.isEmpty())
    {
        Invite i;
        if (i.exportToPDF(filePath, ui->tableWidget))
        {
            showCustomMessageBox("Success", "Table exported to PDF.", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Error", "Failed to export table to PDF.", QMessageBox::Critical);
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Effacer le formulaire-------------------------------------------------------------------*/
void MainWindow::on_pushButton_annulerInvite_clicked()
{
   ui->lineEdit_nomI->clear();
   ui->lineEdit_prenomI->clear();
   ui->lineEdit_professionI->clear();
   ui->lineEdit_emailI->clear();
   ui->lineEdit_nbAbonnesI->clear();
   ui->lineEdit_nbParticipationI->clear();
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------Rafraichir la table apres chaque modification--------------------------------------------------------------------------------------*/

void MainWindow::updateTableWidgetInvite(const QString &filter)
{
   Connection c;
   QSqlQueryModel *model = new QSqlQueryModel();

   if (filter.isEmpty()) {
       // Si le filtre de recherche est vide, récupérer toutes les lignes
       model->setQuery("SELECT * FROM INVITE");
   } else {
       // Si un filtre de recherche est spécifié, utiliser la requête filtrée
       QString query = QString("SELECT * FROM INVITE WHERE Nom LIKE '%1%' OR Prenom LIKE '%1%' OR Profession LIKE '%1%' OR Email LIKE '%1%'").arg(filter);
       model->setQuery(query);
   }

   int RowNumber = 0;
   ui->tableWidget->setRowCount(0);

   while (model->canFetchMore()) {
       model->fetchMore();
   }

   for (int row = 0; row < model->rowCount(); ++row) {
           // Ajouter la ligne uniquement si le filtre est vide ou si c'est la première ligne de la recherche
           ui->tableWidget->insertRow(RowNumber);
           for (int col = 0; col < 7; ++col) {
               QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
               ui->tableWidget->setItem(RowNumber, col, item);
           }
           RowNumber++;
       }

   delete model;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------Modifier le style du Message Box-------------------------------------------------------------------*/

void MainWindow::showCustomMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon)
{
   // Créer la boîte de message avec le texte et l'icône fournis
   QMessageBox messageBox(icon, title, message, QMessageBox::Ok, this);

   // Obtenir le bouton Ok
   QAbstractButton *okButton = messageBox.button(QMessageBox::Ok);

   // Styles pour le bouton Ok
   QString okButtonStyle = "QPushButton {"
                           "background-color: #D1D0FB;"
                           "color: white;"
                           "border: 1px solid #C4C4C4;"
                           "border-radius: 5px;"
                           "padding: 5px;"
                           "margin: 4px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: #D3D3D3;"
                           "}";

   okButton->setStyleSheet(okButtonStyle);

   // Appliquer les styles directement au QLabel (texte)
   QLabel *label = messageBox.findChild<QLabel*>("qt_msgbox_label");
   if (label) {
       label->setStyleSheet("min-width: 300px; text-align: center;");
   }

   // Styles pour l'ensemble de la QMessageBox
   QString messageBoxStyle = "QMessageBox {"
                             "background-color: #E8E8FD;"
                             "color: white;"
                             "font-size: 16px;"
                             "font-family: 'Space Grotesk';"
                             "}"
                             "QPushButton:pressed {"
                             "background-color: #A5A5A5;"
                             "}";

   messageBox.setStyleSheet(messageBoxStyle);

   // Connecter l'événement hover personnalisé pour le bouton Ok
   connect(okButton, &QAbstractButton::pressed, [=]() {
       okButton->setStyleSheet("background-color: #A5A5A5;");
   });
   connect(okButton, &QAbstractButton::released, [=]() {
       okButton->setStyleSheet("background-color: #D3D3D3;");
   });

   // Ajuster la taille de la boîte de message
   messageBox.setMinimumSize(300, 100);

   // Afficher la boîte de message
   messageBox.exec();
}
/*---------------------------------------------------------------------------Controle de Saisie-----------------------------------------------------------------------------------*/
// Fonction de validation de l'e-mail
void MainWindow::validateEmail(const QString &text) {
    QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
    bool isValid = emailRegex.exactMatch(text);
    ui->label_emailError->setVisible(!isValid);
    ui->label_emailError->setStyleSheet(isValid ? "" : "color: red;");
}

// Fonction de validation du nombre d'abonnés
void MainWindow::validateNbAbonnes(const QString &text) {
    bool isValid = !text.isEmpty() && text.toInt() > 0;
    ui->label_nbAbonnesError->setVisible(!isValid);
    ui->label_nbAbonnesError->setStyleSheet(isValid ? "" : "color: red;");
}

// Fonction de validation du nombre de participations
void MainWindow::validateNbParticipation(const QString &text) {
    bool isValid = !text.isEmpty() && text.toInt() > 0;
    ui->label_nbParticipationError->setVisible(!isValid);
     ui->label_nbParticipationError->setStyleSheet(isValid ? "" : "color: red;");
}
void MainWindow::validateNom(const QString &text) {
    bool isValid = !text.isEmpty() && text.length() <= 10; // Le nom ne doit pas être vide et ne doit pas dépasser 10 caractères
    ui->label_nomError->setVisible(!isValid);
    ui->label_nomError->setStyleSheet(isValid ? "" : "color: red;");
}

void MainWindow::validatePrenom(const QString &text) {
    bool isValid = !text.isEmpty() && text.length() <= 10; // Le prénom ne doit pas être vide et ne doit pas dépasser 10 caractères
    ui->label_prenomError->setVisible(!isValid);
    ui->label_prenomError->setStyleSheet(isValid ? "" : "color: red;");
}

void MainWindow::validateProfession(const QString &text) {
    bool isValid = !text.isEmpty() && text.at(0).isUpper(); // La profession ne doit pas être vide et doit commencer par une lettre majuscule
    ui->label_professionError->setVisible(!isValid);
    ui->label_professionError->setStyleSheet(isValid ? "" : "color: red;");
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

// Mokh :


void MainWindow::on_pushButton_ajouterS_clicked()
{
    Sponsor s;
    s.Setnom(ui->lineEdit_nomS->text());
    s.Setbudget(ui->lineEdit_budget->text());
    s.Setpack(ui->comboBox_pack->currentText());
    s.Setdate_deb(ui->dateEdit_SD->text());
    s.Setdate_fin(ui->dateEdit_SF->text());
    s.Settelephone(ui->lineEdit_tel->text());

    if (s.Getnom().isEmpty() || s.Getbudget().isEmpty() || s.Getpack().isEmpty() ||
            s.Getdate_deb().isEmpty() || s.Getdate_fin().isEmpty() || s.Gettelephone().isEmpty()) {
            QMessageBox::critical(this, "Error", "The fields must not be empty.");
            return; // Exit the function to prevent further execution
        }
    // Input validation
        QRegExp rx("^[a-zA-Z]+$");
        if (s.Getnom().length() > 20 || !rx.exactMatch(s.Getnom())) {
            QMessageBox::critical(this, "Error", "Le nom ne doit pas dépasser 20 caractères et doit être aplphabétique.");
            return;
        }

        bool budgetConversionOk;
        double budgetValue = s.Getbudget().toDouble(&budgetConversionOk);
        if (!budgetConversionOk || s.Getbudget().length() > 20) {
            QMessageBox::critical(this, "Error", "Le budget doit être un nombre valide.");
            return;
        }

        /*QStringList validPacks = {"bronze", "silver", "gold"};
        if (!validPacks.contains(s.Getpack().toLower())) {
            QMessageBox::critical(this, "Error", "Le pack doit être en bronze, en argent ou en or.");
            return;
        }*/

        // Date validation
        QDate dateDeb = ui->dateEdit_SD->date();
        QDate dateFin = ui->dateEdit_SF->date();
        if (dateFin < dateDeb) {
            QMessageBox::critical(this, "Error", "Plage de dates non valide. Date_fin ne devrait pas l’être avant date_deb.");
            return;
        }

        // Telephone validation
        if (s.Gettelephone().length() != 8 || !s.Gettelephone().toInt()) {
            QMessageBox::critical(this, "Error", "Le téléphone doit être composé de 8 chiffres.");
            return;
        }


        Connection c;
        if (c.createconnect())
        {
            if (c.insertDataS(s.Getnom(), s.Getbudget(), s.Getpack(), s.Getdate_deb(), s.Getdate_fin(), s.Gettelephone() ))
            {
                emit dataUpdatedS(); // Emit the signal after successful insertion
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

void MainWindow::on_pushButton_annulerS_clicked()
{

       // Update the UI or clear the input fields if needed
       ui->lineEdit_nomS->clear();
       ui->lineEdit_budget->clear();
       ui->comboBox_pack->clear();
       ui->dateEdit_SD->clear();
       ui->dateEdit_SF->clear();
       ui->lineEdit_tel->clear();

}


void MainWindow::updateTableWidget_S()
{
    Connection c;
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SPONSOR");

    int RowNumber = 0;
    ui->tableWidget_S->setRowCount(0);

    while (model->canFetchMore())
        model->fetchMore();

    for (int row = 0; row < model->rowCount(); ++row)
    {
        ui->tableWidget_S->insertRow(row);
        for (int col = 0; col < 7; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget_S->setItem(row, col, item);
        }
        RowNumber++;
    }

    delete model; // Release the memory occupied by the model
}

void MainWindow::on_pushButton_supprimerS_clicked()
{
    QItemSelectionModel *select = ui->tableWidget_S->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
         int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->tableWidget_S->item(row, 0);
        int idToDelete = item->text().toInt();

        Sponsor s;
        if (s.supprimer(idToDelete))
        {
            // Supprimer la ligne de l'affichage
            ui->tableWidget_S->removeRow(row);
            emit dataUpdatedS(); // Mettre à jour la vue après la suppression
            QMessageBox::information(this, "Success", "Data deleted from the database.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete data from the database.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please select a row to delete.");
    }
}

void MainWindow::on_pushButton_modifierS_clicked()
{
    QItemSelectionModel *select = ui->tableWidget_S->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidget_S->item(row, 0)->text().toInt();

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidget_S->item(row, 1)->text();
        QString budget = ui->tableWidget_S->item(row, 2)->text();
        QString pack = ui->tableWidget_S->item(row, 3)->text();
        QString date_deb = ui->tableWidget_S->item(row, 4)->text();
        QString date_fin = ui->tableWidget_S->item(row, 5)->text();
        QString telephone = ui->tableWidget_S->item(row, 6)->text();

        // Contrôles de saisie
        if (nom.isEmpty() || budget.isEmpty() || pack.isEmpty() ||
                date_deb.isEmpty() || date_fin.isEmpty() || telephone.isEmpty()) {
            QMessageBox::critical(this, "Error", "Les champs ne doivent pas être vides.");
            return;
        }

        // Validation des saisies
        QRegExp rx("^[a-zA-Z]+$");
        if (nom.length() > 20 || !rx.exactMatch(nom)) {
            QMessageBox::critical(this, "Error", "Le nom ne doit pas dépasser 20 caractères et doit être alphabétique.");
            return;
        }

        bool budgetConversionOk;
        double budgetValue = budget.toDouble(&budgetConversionOk);
        if (!budgetConversionOk || budget.length() > 20) {
            QMessageBox::critical(this, "Error", "Le budget doit être un nombre valide.");
            return;
        }

        QStringList validPacks = {"bronze", "silver", "gold"};
        if (!validPacks.contains(pack.toLower())) {
            QMessageBox::critical(this, "Error", "Le pack doit être en bronze, en argent ou en or.");
            return;
        }

        // Date validation
        QDate dateDeb = QDate::fromString(date_deb, "dd/MM/yyyy");
        QDate dateFin = QDate::fromString(date_fin, "dd/MM/yyyy");
        if (!dateDeb.isValid() || !dateFin.isValid() || dateFin < dateDeb) {
            QMessageBox::critical(this, "Error", "Plage de dates non valide. Date_fin ne devrait pas l’être avant date_deb.");
            return;
        }

        // Telephone validation
        if (telephone.length() != 8 || !telephone.toInt()) {
            QMessageBox::critical(this, "Error", "Le téléphone doit être composé de 8 chiffres.");
            return;
        }

        // Appelez la fonction modifierInvite pour mettre à jour la base de données
        Sponsor s;
        if (s.modifier(idToModify, nom, budget, pack, date_deb, date_fin, telephone))
        {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidget_S->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidget_S->setItem(row, 2, new QTableWidgetItem(budget));
            ui->tableWidget_S->setItem(row, 3, new QTableWidgetItem(pack));
            ui->tableWidget_S->setItem(row, 4, new QTableWidgetItem(date_deb));
            ui->tableWidget_S->setItem(row, 5, new QTableWidgetItem(date_fin));
            ui->tableWidget_S->setItem(row, 6, new QTableWidgetItem(telephone));

            emit dataUpdatedS(); // Mettre à jour la vue après la modification
            QMessageBox::information(this, "Success", "Données modifiées dans la base de données.");
        }
        else
        {
            QMessageBox::critical(this, "Error", "Échec de la modification des données dans la base de données.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Veuillez sélectionner une ligne à modifier.");
    }
}



void MainWindow::on_pushButton_reset_clicked()
{
    Sponsor s;
    s.afficher(ui->tableWidget_S);
}


void MainWindow::on_pushButton_rechercherS_clicked()
{
    bool conversionOk;
    int id_sponsor = ui->lineEdit_rechercherS->text().toInt(&conversionOk);

    if (conversionOk)
    {
        // Perform the search by id_sponsor
        Sponsor s;
        emit dataUpdatedS();
        s.rechercher(id_sponsor, ui->tableWidget_S);

    }
    else
    {
        QMessageBox::warning(this, "Error", "Please enter a valid ID for the search.");
    }
}


void MainWindow::on_pushButton_trierS_clicked()
{
    Sponsor s;
    if (s.trierParPack(ui->tableWidget_S)) {
        QMessageBox::information(this,"Success", "Data sorted by Packs");
    } else {
        QMessageBox::critical(this,"Error", "Failed to sort data.");
    }
}

void MainWindow::on_PDF_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableWidget_S->rowCount();
    const int columnCount = ui->tableWidget_S->columnCount();

    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           << QString("<title>%1</title>\n").arg("Liste des sponsors")
           << "</head>\n"
              "<body bgcolor=#ffffff link=#5000A0>\n"
              "<center> <H1>Liste Des Sponsors </H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 1; column < columnCount; column++)
        if (!ui->tableWidget_S->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(ui->tableWidget_S->horizontalHeaderItem(column)->text());
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr> <td bkcolor=0>" << row + 1 << "</td>";
        for (int column = 1; column < columnCount; column++)
        {
            if (!ui->tableWidget_S->isColumnHidden(column))
            {
                QTableWidgetItem *item = ui->tableWidget_S->item(row, column);
                QString data = (item) ? item->text().simplified() : QString("&nbsp;");
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out << "</table> </center>\n"
           "</body>\n"
           "</html>\n";

    QString fileName = QFileDialog::getSaveFileName((QWidget *)0, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);
}


// Zoh :



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
        int idToModify = ui->tableWidgetM->item(row, 5)->text().toInt(); // Récupérer l'ID du materiel après le tri

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidgetM->item(row, 0)->text();
        QString type = ui->tableWidgetM->item(row, 1)->text();
        QString etat = ui->tableWidgetM->item(row, 2)->text();
        QString quantite = ui->tableWidgetM->item(row, 3)->text();
        QString date = ui->tableWidgetM->item(row, 4)->text();

        // Appelez la fonction modifierMateriaux pour mettre à jour la base de données
        Materiaux i;
        if (i.modifierMateriaux(idToModify, nom, type, etat, quantite, date))
        {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidgetM->setItem(row, 0, new QTableWidgetItem(nom));
            ui->tableWidgetM->setItem(row, 1, new QTableWidgetItem(type));
            ui->tableWidgetM->setItem(row, 2, new QTableWidgetItem(etat));
            ui->tableWidgetM->setItem(row, 3, new QTableWidgetItem(quantite));
            ui->tableWidgetM->setItem(row, 4, new QTableWidgetItem(date));

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
            labels << "Nom" << "Type" << "Etat" << "Quantite" << "Date" << "ID";
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
bool MainWindow::eventFilterT(QObject *obj, QEvent *event)
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
    if (column == 3) { // Si la colonne est la colonne de la date
        QStringList choices; // Vous pouvez remplir cette liste avec des choix de date si nécessaire
        showChoicesUnderCell(row, column, choices);
    }
}

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
// Louay :


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
        db.setUserName("feriel");
        db.setPassword("feriel");
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
    db.setUserName("feriel");
    db.setPassword("feriel");
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
                       // ui->listView->hide();
        }
        return QObject::eventFilter(obj, event);
}

void MainWindow::preventCellEdit_Transactions()
{
    ui->table_de_transactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
   // ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
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
    if (previousWidget) {
        QComboBox *previousComboBox = previousWidget->findChild<QComboBox*>();
        if (previousComboBox) {
            QString newText = previousComboBox->currentText();
            ui->table_de_transactions->item(row, column)->setText(newText);
        } else {
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
      //  ui->listView->setModel(model);
      //  ui->listView->show();
}



// Ismail :


void MainWindow::on_pushButton_ajouterpod_clicked()
{
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
    int id = ui->lineEdit_7p->text().toInt();
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
             if (p.supprimerp((id))) {
                  ui->tableView->setModel(p.afficherp());
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
  /*  QString nom = ui->lineEdit_7p->text();
    // Obtenez le nom entré

    podcast p;
    QSqlQuery query;
    query.prepare("SELECT id_podcast, nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE nom = ?");
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
    }*/




void MainWindow::on_pushButton_modifierpod_clicked()
{


    int id = ui->lineEdit_7p->text().toInt();
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
        ui->lineEdit_nomp->setText(nom);
        ui->lineEdit_dureep->setText(duree);
        ui->lineEdit_lieup->setText(lieu);
         ui->comboBoxp->setCurrentText(categorie);

        ui->dateEditp->setDate(QDate::fromString(date_pod, "yyyy-MM-dd"));


        // Mettez à jour le podcast avec les nouvelles valeurs si l'utilisateur modifie les champs et appuie sur un bouton de confirmation
                 if (ui->pushButton_ajouterpod->isEnabled()) {
                    /* QString newNom = ui->lineEdit_nomp->text();
                     QString newDuree = ui->lineEdit_dureep->text();
                     QString newLieu = ui->lineEdit_lieup->text();
                     QString newCategorie = ui->comboBoxp->setCurrentText(categorie);
                     QString newDatePod = ui->dateEditp->text();*/

                     if (p.update(id, nom, duree, lieu, categorie, date_pod)) {
                         ui->tableView->setModel(p.afficherp());
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

   /* QString id= ui->lineEdit_7p->text();
    // Obtenez le nom entré

    podcast p;
    QSqlQuery query;
    query.prepare("SELECT  nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE id_podcast = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        // Récupérez les valeurs de la requête
       // int id = query.value(0).toInt(); // Récupérez le nom du podcast pour la modification
        QString nom = query.value(0).toString();
        QString duree = query.value(1).toString();
        QString lieu = query.value(2).toString();
        QString categorie = query.value(3).toString();
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


          //  if (p.update(nom, newNom, newDuree, newLieu, newCategorie, newDatePod)) {
           //     ui->tableViewp->setModel(p.afficherp());

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
    }*/
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




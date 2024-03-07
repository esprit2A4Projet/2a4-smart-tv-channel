#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "employee.h"
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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Employee E;
       ui->setupUi(this);


       // Ancien :

       model = new QStandardItemModel(this);
        ui->tableView->setModel(model);
        // ui->tableView->setModel(E.afficher());

          model->setColumnCount(5); // Nous avons 5 colonnes dans notre TableView
             model->setHorizontalHeaderLabels({"ID", "Nom", "Prénom", "Date Embauche", "Salaire", "Poste"});
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
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_23_clicked() // Bouton Valider
{
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString date = ui->dateEdit->text();

        bool ok = false;
        int salaire = ui->lineEdit_salaire->text().toInt(&ok);

        while (!ok)
        {
            QString salaireText = QInputDialog::getText(this, "Saisir le salaire", "Entrez le salaire:");
            if (salaireText.isEmpty())
            {
                return; // L'utilisateur a annulé la saisie, retournez simplement
            }
            // Tentative de conversion du texte en entier
            salaire = salaireText.toInt(&ok);
            if (!ok)
            {
                // Afficher un message d'erreur à l'utilisateur
                QMessageBox::warning(this, "Erreur", "Veuillez saisir un entier valide pour le salaire.");
            }
        }
    QString poste = ui->lineEdit_poste->text();
    int id = ui->lineEdit_3->text().toInt(); // Assuming this is the employee ID
    Employee e(nom, prenom, date, salaire, poste);
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
        ui->dateEdit->clear();
        ui->lineEdit_salaire->clear();
        ui->lineEdit_poste->clear();
    } else {
        msgBox.setText("Operation failed.");
          msgBox.exec();
    }

}



void MainWindow::on_pushButton_67_clicked()  // bouton supprimer
{
    // Obtenez l'ID entré
    int id = ui->lineEdit_3->text().toInt();

    if (id > 0 && id < 99999999)
    {
        Employee E;
        QSqlQuery query;
        query.prepare("SELECT id,nom, prenom, date_embauche, salaire, poste FROM employes WHERE id = ?");
        query.addBindValue(id);

        if (query.exec() && query.next()) {
            // Récupérez les valeurs de la requête
            QString nom = query.value(0).toString();
            QString prenom = query.value(1).toString();
            QString date_embauche = query.value(2).toString();
            double salaire = query.value(3).toDouble();
            QString poste = query.value(4).toString();

            Employee(nom,prenom,date_embauche,salaire,poste);

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
    } else {
        // Afficher un message indiquant que l'ID n'est pas valide
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                            QObject::tr("Veuillez saisir un entier valide.\n"
                                        "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}




void MainWindow::on_pushButton_22_clicked() // Button annuler
{
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->dateEdit->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_poste->clear();
}

void MainWindow::on_pushButton_68_clicked() {  //Bouton  MODIFIER

    int id = ui->lineEdit_3->text().toInt();
 if (id > 0 && id < 99999999)
 {
    // Obtenez l'ID entré
    Employee E;
    QSqlQuery query;
    query.prepare("SELECT nom, prenom, date_embauche, salaire, poste FROM employes WHERE id = ?");
    query.addBindValue(id);

    if (query.exec() && query.next())
    {
        // Récupérez les valeurs de la requête
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        QString date_embauche = query.value(2).toString();
        double salaire = query.value(3).toDouble();
        QString poste = query.value(4).toString();

        // Afficher les informations de l'employé dans les champs de texte
        ui->lineEdit_nom->setText(nom);
        ui->lineEdit_prenom->setText(prenom);
        ui->dateEdit->setDate(QDate::fromString(date_embauche, "yyyy-MM-dd"));
        ui->lineEdit_salaire->setText(QString::number(salaire));
        ui->lineEdit_poste->setText(poste);
        Employee(nom,prenom,date_embauche,salaire,poste);
    // Récupérer les informations de l'employé pour l'ID donné
            if (E.update(id))
            {
                 ui->tableView->setModel(E.afficher());
             }
            else
            {
                    // Afficher un message indiquant que l'employé est introuvable
                    QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                          QObject::tr("Employé inexistant.\n"
                                                      "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            }
        }
      else
    {
                // Afficher un message indiquant que l'employé est introuvable
                QMessageBox::critical(nullptr, QObject::tr("Error"),
                                      QObject::tr("Employé inexistant.\n"
                                                  "Click Cancel to exit."), QMessageBox::Cancel);
     }
 }
 else
 {
     // Afficher un message indiquant que l'ID n'est pas valide
     QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                         QObject::tr("Veuillez saisir un entier valide.\n"
                                     "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
 }
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

void MainWindow::on_pushButton_70_clicked() // chercher
{
    Employee E;

    int id = ui->lineEdit_8->text().toInt();
   // Obtenez l'ID entré
    if (id > 0 && id < 99999999)
    {
      QSqlQuery query;
      query.prepare("SELECT id,nom, prenom, date_embauche, salaire, poste FROM employes WHERE id = ?");
      query.addBindValue(id);

      if (query.exec() && query.next()) {
          // Récupérez les valeurs de la requête
          QString nom = query.value(0).toString();
          QString prenom = query.value(1).toString();
          QString date_embauche = query.value(2).toString();
          double salaire = query.value(3).toDouble();
          QString poste = query.value(4).toString();


          Employee(nom,prenom,date_embauche,salaire,poste);
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
  //    ui->tableView->setModel(E.afficher());



    }
    else
    {
        // Afficher un message indiquant que l'ID n'est pas valide
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                            QObject::tr("Veuillez saisir un entier valide.\n"
                                        "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}



void MainWindow::on_pushButton_92_clicked()  // Exportation PDF :
{


}

void MainWindow::on_pushButton_71_clicked() // Afficher
{ Employee E;
      ui->tableView->setModel(E.afficher());
}

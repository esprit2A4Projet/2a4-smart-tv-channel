#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "employee.h"
#include "connection.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Employee E;
       ui->setupUi(this);

       model = new QStandardItemModel(this);
        ui->tableView->setModel(model);

          model->setColumnCount(5); // Nous avons 5 colonnes dans notre TableView
             model->setHorizontalHeaderLabels({"ID", "Nom", "Prénom", "Date Embauche", "Salaire", "Poste"});
             ui->tableView->setModel(model);


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
    int salaire = ui->lineEdit_salaire->text().toInt();
    QString poste = ui->lineEdit_poste->text();
    int id = ui->lineEdit_3->text().toInt(); // Assuming this is the employee ID

    Employee e(nom, prenom, date, salaire, poste);
    bool success = false;

    if (id == 0) {
         // If the ID is 0, it means we are creating a new employee
         success = e.ajouter();
    } else {
         // Otherwise, we are updating an existing employee
         success = e.update(id);
         ui->lineEdit_3->clear();
    }
    QMessageBox msgBox;
    if (success) {
        msgBox.setText("Operation successful.");
        ui->tableView->setModel(e.afficher());
        ui->lineEdit_nom->clear();
        ui->lineEdit_prenom->clear();
        ui->dateEdit->clear();
        ui->lineEdit_salaire->clear();
        ui->lineEdit_poste->clear();
    } else {
        msgBox.setText("Operation failed.");
    }

    msgBox.exec();
}

void MainWindow::on_pushButton_67_clicked()  // bouton supprimer
{
    Employee E1;

       bool test=E1.supprimer(ui->lineEdit_3->text().toInt());
       if(test)
       {
                    QMessageBox::information(nullptr, QObject::tr("OK"),
                                  QObject::tr("Suppression  effectué\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
                    ui->tableView->setModel(E1.afficher());
                      ui->lineEdit_3->clear();
       }
       else
                    QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Suppression non effectué\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
}


void MainWindow::on_pushButton_22_clicked() // Button annuler
{
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->dateEdit->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_poste->clear();
}

void MainWindow::on_pushButton_68_clicked() {  // MODIFIER

    int id = ui->lineEdit_3->text().toInt();

    // Obtenez l'ID entré
    Employee E;
    QSqlQuery query;
    query.prepare("SELECT nom, prenom, date_embauche, salaire, poste FROM employes WHERE id = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
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
    if (E.update(id)) {
        } else {
            // Afficher un message indiquant que l'employé est introuvable
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("Employé introuvable.\n"
                                              "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    } else {
        // Afficher un message indiquant que l'employé est introuvable
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Employee not found.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
}



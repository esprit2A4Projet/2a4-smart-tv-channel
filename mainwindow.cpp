#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include "connection.h"
#include <QMainWindow>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
       ui->tableView->setModel(model);

       model->setColumnCount(7); // Nous avons 5 colonnes dans notre TableView
          model->setHorizontalHeaderLabels({"ID", "Nom", "Budget", "Pack", "Date debut", "Date fin", "Telephone"});
          ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked() // BV
{
    QString nom = ui->lineEdit_nom->text();
    QString budget = ui->lineEdit_budget->text();
    QString date_deb = ui->dateEdit->text();
    QString date_fin = ui->dateEdit_2->text();
    QString telephone = ui->lineEdit_tel->text(); // Assuming this is the employee ID
    //int id = ui->lineEdit_id->text().toInt();

    Sponsor s(nom, budget, date_deb, date_fin, telephone);

    bool success = s.ajouter();

    /*if (id == 0) {
        // If the ID is 0, it means we are creating a new employee
        success = s.ajouter();
    } else {
        // Otherwise, we are updating an existing employee
        //success = s.update(id);
    }

    if (test) {
        // If the ID is 0, it means we are creating a new employee
        QMessageBox::information(nullptr, QObject::tr("ok")),
                QObject::tr("ajout effectué!\n"
                            "Click cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("not ok")),
                QObject::tr("ajout non effectué!\n"
                            "Click cancel to exit."), QMessageBox::Cancel);
    }*/
    QMessageBox msgBox;
    if (success) {
        msgBox.setText("Operation successful.");
        ui->tableView->setModel(s.afficher());
        ui->lineEdit_nom->clear();
        ui->lineEdit_budget->clear();
        ui->dateEdit->clear();
        ui->dateEdit_2->clear();
        ui->lineEdit_tel->clear();
    } else {
        msgBox.setText("Operation failed.");
    }

    msgBox.exec();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Materiaux.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_ajout_clicked()
{
    Materiaux i;
    i.setnom(ui->lineEdit_nom->text());
    i.settype(ui->lineEdit_type->text());
    i.setetat(ui->lineEdit_etat->text());
    i.setquantite(ui->lineEdit_quantite->text());
    i.setdate(ui->lineEdit_date->text());

    Connection c;
        if (c.createconnect())
        {
            if (c.insertData(i.get_nom(), i.get_type(), i.get_etat(), i.get_quantite(), i.get_date()))
            {
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
/*void MainWindow::on_pushButton_ajout_clicked()
{
    //int id=ui->lineEdit_id->text().toInt();
    QString nom=ui->lineEdit_nom->text();
    QString type=ui->lineEdit_type->text();
    QString etat=ui->lineEdit_etat->text();
    QString quantite=ui->lineEdit_quantite->text();
    QString date=ui->lineEdit_date->text();

    Materiaux M(nom,type,etat,quantite,date);

    bool connect=M.ajouter();
    if(c.connect())
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Ajout effectué\n""Click Cancel  to exit."),QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),QObject::tr("Ajout non effectué.\n""Click Cancel to exit."),QMessageBox::Cancel);
}*/
void MainWindow::on_pushButton_supprimer_clicked()
{
    QString nom=ui->lineEdit_nom->text();
    Materiaux Etmp;
    bool test=Etmp.supprimer(nom);
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Suppression effectué\n""Click Cancel  to exit."),QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),QObject::tr("Suppression non effectué.\n""Click Cancel to exit."),QMessageBox::Cancel);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "transaction.h"
#include "connexion.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
       ui->setupUi(this);
       model = new QStandardItemModel(this);
       ui->tableView->setModel(model);
       model->setColumnCount(5); // Nous avons 5 colonnes dans notre TableView
       ui->tableView->setModel(t.afficher());
       model->setHorizontalHeaderLabels({"ID", "Mode de paiement", "type", "categorie", "montant", "date de transaction"});
       ui->tableView->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_Ajout_clicked()
{
    QString modeDePaiement = ui->lineEdit_ModeDePaiement->currentText();
    QString type = ui->lineEdit_Type->currentText();
    QString categorie = ui->lineEdit_Categorie->text();
    QString montant = ui->lineEdit_Montant->text();
    QString dateTransaction = ui->LineEdit_Date->text();
    //int id = ui->lineEdit_3->text().toInt(); // Assuming this is the employee ID

    Transaction t(modeDePaiement, type, categorie, montant, dateTransaction);
    bool success = false;
    success = t.ajouter();
    QMessageBox msgBox;
    if (success) {
        msgBox.setText("Operation successful.");

        ui->tableView->setModel(t.afficher());
        ui->lineEdit_ModeDePaiement->clear();
        ui->lineEdit_Type->clear();
        ui->lineEdit_Categorie->clear();
        ui->lineEdit_Montant->clear();
        ui->LineEdit_Date->clear();
    } else {
        msgBox.setText("Operation failed.");
    }

    msgBox.exec();
}





/*void MainWindow::on_pushButton_67_clicked()  // b s
{
    Transaction t1;

       bool test=t1.supprimer(ui->lineEdit_3->text().toInt());

       if(test)
       {
                    QMessageBox::information(nullptr, QObject::tr("OK"),
                                  QObject::tr("Suppression  effectué\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
                    ui->tableView->setModel(t1.afficher());
       }
       else
                    QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Suppression non effectué\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
}
*/
/*void MainWindow::on_pushButton_68_clicked() {
    int id = ui->lineEdit_3->text().toInt(); // Obtenez l'ID entré
    Transaction t;
    QSqlQuery query;

        query.prepare("SELECT modeDePaiement, type, categorie, montant, dateTransaction FROM employes WHERE id = ?");
        query.addBindValue(id);
//}
}
*/



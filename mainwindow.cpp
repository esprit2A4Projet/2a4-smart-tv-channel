#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Materiaux.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(Etmp.afficher());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_ajout_clicked()
{
    Materiaux i;
    i.setid(ui->lineEdit_id->text().toInt());
    i.setnom(ui->lineEdit_nom->text());
    i.settype(ui->lineEdit_type->text());
    i.setetat(ui->lineEdit_etat->text());
    i.setquantite(ui->lineEdit_quantite->text());
    i.setdate(ui->lineEdit_date->text());

    Connection c;
        if (c.createconnect())
        {
            if (c.insertData(i.get_id(), i.get_nom(), i.get_type(), i.get_etat(), i.get_quantite(), i.get_date()))
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
    // Récupérer les valeurs saisies dans les champs de texte
    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString type = ui->lineEdit_type->text();
    QString etat = ui->lineEdit_etat->text();
    QString quantite = ui->lineEdit_quantite->text();
    QString date = ui->lineEdit_date->text();

    // Vérifier si les champs obligatoires sont vides
    if (nom.isEmpty() || type.isEmpty() || etat.isEmpty() || quantite.isEmpty() || date.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Créer une instance de la classe Materiaux avec les valeurs saisies
    Materiaux m(id, nom, type, etat, quantite, date);

    // Instancier la connexion à la base de données
    Connection c;

    // Vérifier la connexion à la base de données
    if (c.createconnect())
    {
        bool success = false;

        // Vérifier si l'ID existe déjà dans la base de données
        if (id == 0)
        {
            // Si l'ID est 0, cela signifie qu'il n'existe pas encore dans la base de données
            // Appeler la méthode ajouter() pour ajouter un nouveau matériau
            success = m.ajouter();
        }
        else
        {
            // Si l'ID est différent de 0, cela signifie qu'il existe déjà dans la base de données
            // Appeler la méthode afficher() pour récupérer tous les matériaux
            QSqlQueryModel* model = m.afficher();

            // Parcourir tous les matériaux pour trouver celui qui correspond à l'ID donné
            for (int i = 0; i < model->rowCount(); ++i)
            {
                if (model->record(i).value("id").toInt() == id)
                {
                    // Récupérer les informations du matériau à mettre à jour
                    QString nom = model->record(i).value("nom").toString();
                    QString type = model->record(i).value("type").toString();
                    QString etat = model->record(i).value("etat").toString();
                    QString quantite = model->record(i).value("quantite").toString();
                    QString date = model->record(i).value("date").toString();

                    // Appeler la méthode update() pour mettre à jour les informations du matériau existant
                    success = m.update(id, nom, type, etat, quantite, date);
                    break;
                }
            }

            // Libérer la mémoire du modèle
            delete model;

            if (!success)
            {
                QMessageBox::critical(this, "Error", "Le matériau avec cet ID n'existe pas.");
            }
        }

        // Afficher un message en fonction du succès de l'opération
        if (success)
        {
            QMessageBox::information(this, "Success", "Opération réussie.");
            // Effacer les champs de saisie après un ajout ou une mise à jour réussi
            ui->lineEdit_id->clear();
            ui->lineEdit_nom->clear();
            ui->lineEdit_type->clear();
            ui->lineEdit_etat->clear();
            ui->lineEdit_quantite->clear();
            ui->lineEdit_date->clear();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Opération échouée.");
        }

        // Rafraîchir le modèle de TableView avec les données mises à jour
        ui->tableView->setModel(m.afficher());
    }
    else
    {
        QMessageBox::critical(this, "Error", "Impossible de se connecter à la base de données.");
    }
}*/


/*void MainWindow::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    Materiaux Etmp;
    bool test = Etmp.supprimer(id);
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Suppression effectué\n""Click Cancel  to exit."),QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),QObject::tr("Suppression non effectué.\n""Click Cancel to exit."),QMessageBox::Cancel);
}
*/
void MainWindow::on_pushButton_supprimer_clicked() {
    int id = ui->lineEdit_recherche->text().toInt();
    Materiaux M;
    QSqlQuery query;
    query.prepare("SELECT ID_MATERIEL FROM MATERIAUX WHERE ID_MATERIEL = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        if (M.supprimer(id)) {
            ui->tableView->setModel(M.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                QObject::tr("Suppression effectuée\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                QObject::tr("Échec de la suppression.\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
            QObject::tr("Materiaux inexistant.\n"
                        "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}


/*void MainWindow::on_pushButton_modifier_clicked() {
    int id = ui->lineEdit_recherche->text().toInt();

    // Obtenez l'ID entré
    Materiaux M;
    QSqlQuery query;
    query.prepare("SELECT ID_MATERIEL, NOM, TYPE, ETAT, QUANTITE, DATE_MISE FROM MATERIAUX WHERE ID_MATERIEL = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        // Récupérez les valeurs de la requête
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString type = query.value(2).toString();
        QString etat = query.value(3).toString();
        QString quantite = query.value(4).toString();
        QString date = query.value(5).toString();

        // Afficher les informations de l'employé dans les champs de texte
        ui->lineEdit_nom->setText(nom);
        ui->lineEdit_type->setText(type);
        ui->lineEdit_etat->setText(etat);
        ui->lineEdit_quantite->setText(quantite);
        ui->lineEdit_date->setDate(QDate::fromString(date, "yyyy-MM-dd"));

        // Mettre à jour les valeurs dans l'objet Materiaux
        M.setid(id);
        M.setnom(nom);
        M.settype(type);
        M.setetat(etat);
        M.setquantite(quantite);
        M.setdate(date);

        // Mettre à jour l'enregistrement dans la base de données
        if (M.update(id)) {
            ui->tableView->setModel(M.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                QObject::tr("Modification effectuée\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            // Afficher un message d'erreur si la mise à jour a échoué
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                QObject::tr("Échec de la modification.\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    } else {
        // Afficher un message si l'ID spécifié n'existe pas dans la base de données
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
            QObject::tr("Materiaux inexistant.\n"
                        "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}*/


void MainWindow::on_pushButton_modifier_clicked()
{

        int id = ui->lineEdit_recherche->text().toInt();
        // Obtenez l'ID entré
        Materiaux M;
        QSqlQuery query;
        query.prepare("SELECT ID_MATERIEL, NOM, TYPE, ETAT, QUANTITE, DATE_MISE FROM MATERIAUX WHERE ID_MATERIEL = ?");
        query.addBindValue(id);

        if (query.exec() && query.next()) {
            // Récupérez les valeurs de la requête
            int id = query.value(0).toInt();
            QString nom = query.value(1).toString();
            QString type = query.value(2).toString();
            QString etat = query.value(3).toString();
            QString quantite = query.value(4).toString();
            QString date = query.value(5).toString();

            // Afficher les informations du materiel dans les champs de texte
            ui->lineEdit_nom->setText(nom);
            ui->lineEdit_type->setText(type);
            ui->lineEdit_etat->setText(etat);
            ui->lineEdit_quantite->setText(quantite);
            ui->lineEdit_date->setDate(QDate::fromString(date, "yyyy-MM-dd"));

            // Mettez à jour le materiel avec les nouvelles valeurs si l'utilisateur modifie les champs et appuie sur un bouton de confirmation
            if (ui->pushButton_ajout->isEnabled()) {
                QString newnom = ui->lineEdit_nom->text();
                QString newtype = ui->lineEdit_type->text();
                QString newetat = ui->lineEdit_etat->text();
                QString newquantite = ui->lineEdit_quantite->text();
                QString newdate = ui->lineEdit_date->text();

                if (M.update(id, newnom, newtype, newetat, newquantite, newdate)) {
                    ui->tableView->setModel(M.afficher());
                    QMessageBox::information(nullptr, QObject::tr("OK"),
                                             QObject::tr("Modification effectuée.\n"
                                                         "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
                } else {
                    QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                          QObject::tr("Échec de la modification du materiel.\n"
                                                      "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
                }
            }
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("materiel introuvable.\n"
                                              "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
        }
    }

/*void MainWindow::on_pushButton_modifier_clicked()
{
    int id = ui->lineEdit_recherche->text().toInt();

    // **Vérification si l'utilisateur a modifié les champs**
    bool isModified = false;
    QString newnom = ui->lineEdit_nom->text();
    QString newtype = ui->lineEdit_type->text();
    QString newetat = ui->lineEdit_etat->text();
    QString newquantite = ui->lineEdit_quantite->text();
    QString newdate = ui->lineEdit_date->text();

    Materiaux M;
    QSqlQuery query;
    query.prepare("SELECT NOM, TYPE, ETAT, QUANTITE, DATE_MISE FROM MATERIAUX WHERE ID_MATERIEL = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        QString nomDB = query.value(0).toString();
        QString typeDB = query.value(1).toString();
        QString etatDB = query.value(2).toString();
        QString quantiteDB = query.value(3).toString();
        QString dateDB = query.value(4).toString();

        // Comparer les valeurs actuelles avec celles de la base de données
        if (newnom != nomDB || newtype != typeDB || newetat != etatDB || newquantite != quantiteDB || newdate != dateDB) {
            isModified = true;
        }
    }

    if (isModified) {
        // Mettre à jour le materiel avec les nouvelles valeurs
        if (M.update(id, newnom, newtype, newetat, newquantite, newdate)) {
            ui->tableView->setModel(M.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                             QObject::tr("Modification effectuée.\n"
                                                         "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                          QObject::tr("Échec de la modification du materiel.\n"
                                                      "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::information(nullptr, QObject::tr("Information"),
                                 QObject::tr("Aucune modification n'a été détectée.\n"
                                             "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    }
}
*/

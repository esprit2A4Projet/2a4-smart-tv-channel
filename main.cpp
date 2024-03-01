#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include <QTableView>


int main(int argc, char *argv[])
{
    Materiaux M;
    QApplication a(argc, argv);
    MainWindow w;
    QTableView *tableView = w.findChild<QTableView *>();
    Connection c;
    bool test=c.createconnect();
    if(test)
    {w.show();
        QSqlQueryModel *model = M.afficher();
        // Vérifier si le modèle a été récupéré avec succès
                if (model) {
                    // Associer le modèle à la QTableView
                    tableView->setModel(model);
                } else {
                    QMessageBox::critical(nullptr, QObject::tr("Erreur de chargement"),
                                QObject::tr("Impossible de charger les données.\n"
                                            "Click Cancel to exit."), QMessageBox::Cancel);
                }

        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);



    return a.exec();
}

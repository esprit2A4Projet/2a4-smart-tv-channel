#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;
    bool test = c.createconnect(); // Etablir la connexion
    if (test)
    {
        w.show();
        w.updateTableWidgetM(); // Charger les données du tableau au démarrage
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                               QObject::tr("connection failed.\n"
                                           "Click Cancel to exit."), QMessageBox::Cancel);

    return a.exec();
}


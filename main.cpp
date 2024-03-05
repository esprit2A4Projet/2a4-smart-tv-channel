#include "mainwindow.h"
<<<<<<< HEAD

#include <QApplication>
#include <QMovie>
#include <QThread>
=======
#include <QApplication>
#include <QMessageBox>
#include "connexion.h"



>>>>>>> 73de357111212f80d2977013cd4c7cb2a45ec9c4

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
<<<<<<< HEAD
    w.show();
=======
    Connection c;
    bool test=c.createconnect(); //etablir la connexion
    if(test)
    {w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);



>>>>>>> 73de357111212f80d2977013cd4c7cb2a45ec9c4
    return a.exec();
}

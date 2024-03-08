#include "mainwindow.h"
#include <QApplication>
#include <QTableView>
#include <QMessageBox>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    connection c;

    w.show();
    bool test=c.createconnect();

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


            return a.exec();
}

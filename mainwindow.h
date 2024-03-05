#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
<<<<<<< HEAD
#include <QMovie>
#include <QThread>
=======
#include <QSqlQueryModel>
#include "transaction.h"
>>>>>>> 73de357111212f80d2977013cd4c7cb2a45ec9c4

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

<<<<<<< HEAD
private:
    Ui::MainWindow *ui;
};
=======
private slots:
    void on_pushButton_ajouter_clicked(); //boutton d'ajout
    void on_pushButton_supprimer_clicked(); // boutton de suppression
    void on_pushButton_modifier_clicked(); //boutton de modification
    void updateTableWidget(); // Rename the slot to match the implementation


    void on_pushButton_Ajout_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_Button_trierParDate_clicked();

    void on_pushButton_exportationFormatExcel_clicked();

signals:
    void dataUpdated(); // Declare the dataUpdated signal
private:
    Ui::MainWindow *ui;
    Transaction Etmp;
    QSqlQueryModel *model; // Declare a member variable for the model
};

>>>>>>> 73de357111212f80d2977013cd4c7cb2a45ec9c4
#endif // MAINWINDOW_H

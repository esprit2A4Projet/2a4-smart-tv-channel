#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "transaction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

    void on_Button_filtrer_2_clicked();

    void on_Button_chercherParID_clicked();

signals:
    void dataUpdated(); // Declare the dataUpdated signal
private:
    Ui::MainWindow *ui;
    Transaction Etmp;
    QSqlQueryModel *model; // Declare a member variable for the model
};

#endif // MAINWINDOW_H

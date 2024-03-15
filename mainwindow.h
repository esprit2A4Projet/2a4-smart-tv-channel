#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "connection.h"
#include "podcast.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QMovie>
#include <QThread>

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
    void on_pushButton_ajouterpod_clicked();

    void on_pushButton_annulerpod_clicked();

    void on_pushButton_supprimerpod_clicked();

    void on_pushButton_modifierpod_clicked();

    void on_pushButton_expo_clicked();

    void on_pushButton_historique_clicked();

    void on_pushButton_recherchep_clicked();

    void on_pushButton_trierpod_clicked();


private:
    Ui::MainWindow *ui;
    podcast p;
         QStandardItemModel *model; // Déclaration du modèle ici
};

#endif // MAINWINDOW_H

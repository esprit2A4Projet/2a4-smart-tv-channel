#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "sponsor.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QMovie>
#include <QThread>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_pushButton_ajouter_clicked();

    void on_pushButton_supprimer_clicked();

    void on_pushButton_modifier_clicked();

private:
    Ui::MainWindow *ui;
    Sponsor s;
    QStandardItemModel *model;
};

#endif // MAINWINDOW_H

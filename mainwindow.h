#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Materiaux.h"

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
    //void on_pushButton_clicked();
    void on_pushButton_ajout_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();





private:
    Ui::MainWindow *ui;
    Materiaux Etmp;
};
#endif
// MAINWINDOW_H

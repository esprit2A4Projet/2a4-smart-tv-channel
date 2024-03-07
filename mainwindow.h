#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "connection.h"
#include "employee.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QMovie>
#include <QThread>

namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_67_clicked();

    void on_pushButton_68_clicked();

    void on_pushButton_10_clicked();


    void on_pushButton_69_clicked();

    void on_pushButton_70_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_92_clicked();

    void on_pushButton_71_clicked();

private:
    Ui::MainWindow *ui;
    Employee e;
     QStandardItemModel *model; // Déclaration du modèle ici
};
#endif // MAINWINDOW_H


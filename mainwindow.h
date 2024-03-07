#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "sponsor.h"
#include <QMainWindow>
#include <QSqlQueryModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateTableWidget();
    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_annuler_clicked();
    void on_pushButton_rechercher_clicked();
    void on_pushButton_trier_clicked();
    void on_PDF_clicked();

signals:
    void dataUpdated();
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model;
};

#endif // MAINWINDOW_H

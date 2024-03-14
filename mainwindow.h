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
    void updateTableWidget_S();
    void on_pushButton_ajouterS_clicked();

    void on_pushButton_supprimerS_clicked();

    void on_pushButton_modifierS_clicked();

    void on_pushButton_annulerS_clicked();

    void on_pushButton_rechercherS_clicked();

    void on_pushButton_trierS_clicked();

    void on_PDF_clicked();

    void on_pushButton_reset_clicked();

    //void on_SMS_clicked();
    //control de saisie
    void validateNom(const QString &text);
    void validateBudget(const QString &text);
    //void validateDate(const QDate &startDate);
    void validateTelephone(const QString &text);

signals:
    void dataUpdated();
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model;
};

#endif // MAINWINDOW_H

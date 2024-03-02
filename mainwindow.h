#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QMessageBox>

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
    void on_pushButton_ajouter_clicked();
    void on_pushButton_delete_clicked(); // Add the slot for delete button
    void on_pushButton_update_clicked();
    void on_pushButton_annuler_clicked();
    void on_pushButton_rechercher_clicked();
    void on_pushButton_trier_clicked();
    void updateTableWidget(const QString &filter); // Rename the slot to match the implementation
    void showCustomMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon);
signals:
    void dataUpdated(); // Declare the dataUpdated signal
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model; // Declare a member variable for the model
};

#endif // MAINWINDOW_H

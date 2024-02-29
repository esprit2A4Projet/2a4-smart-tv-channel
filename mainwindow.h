#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>

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
    void on_pushButton_clicked();
    void updateTableWidget(); // Rename the slot to match the implementation
    void on_pushButton_delete_clicked(); // Add the slot for delete button
    void on_pushButton_update_clicked();
signals:
    void dataUpdated(); // Declare the dataUpdated signal
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model; // Declare a member variable for the model
};

#endif // MAINWINDOW_H

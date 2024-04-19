#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QCalendarWidget>
#include <QGraphicsSimpleTextItem>
#include <QPieSlice>


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
    void on_pushButton_ajouterInvite_clicked();
    void on_pushButton_deleteInvite_clicked(); // Add the slot for delete button
    void on_pushButton_updateInvite_clicked();
    void on_pushButton_annulerInvite_clicked();
    void on_pushButton_rechercherInvite_clicked();
    void on_pushButton_trierInvite_clicked();
    void on_pushButton_exporterInvite_clicked();
    void updateTableWidget(const QString &filter); // Rename the slot to match the implementation
    void showCustomMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon);
    void validateEmail(const QString &text);
    void validateNbAbonnes(const QString &text);
    void validateNbParticipation(const QString &text);
    void validateNom(const QString &text);
    void validatePrenom(const QString &text);
    void validateProfession(const QString &text);
    void validateEmailInvite(const QString &text);
    void on_pushButton_rafraichir_clicked();
    void on_pushButton_envoyer_clicked();
    bool isPodcastDate(const QDate &date);
    void mailStatus(const QString &status);
    void on_calendarWidget_clicked();
    void statistiquesNbAbonnes();
    void on_pushButton_actualiser_clicked();
    void on_pushButton_imprimer_clicked();
    QString getPodcastNameByDate(const QDate &date);
    //bool emailExists(const QString &email);
    void on_calendarWidget_clicked(const QDate &date);
    QString getPodcastInfoByDate(const QDate &date);



signals:
    void dataUpdated(); // Declare the dataUpdated signal
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model; // Declare a member variable for the model
    QHash<int, int> idToRowMap;
    QCalendarWidget *calendarWidget;
    bool dateSelected = false;
};

#endif // MAINWINDOW_H

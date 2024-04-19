#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QtCharts/QChart>
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



    //MISE A JOUR DE LA TABLE
    void update_table_de_transactions();


private slots:

    //BOUTTON D'AJOUT, AFFICHAGE ET SUPPRESSION
    void on_pushButton_Ajout_Transaction_clicked();
    void on_pushButton_Suppression_Transaction_clicked();
    void on_pushButton_Modification_Transaction_clicked();
    void on_pushButton_Annuler_Transaction_clicked();


    //METIERS STANDARDS : TRI PAR DATE/MONTANT, EXPORTATION EXCEL ET RECHERCHE PAR MONTANT
    void on_Button_trierParDate_Transaction_clicked();
    void on_pushButton_exportationFormatExcel_Transaction_clicked();
    void on_Button_TrierParMontant_Transaction_clicked();
    void searchTransactionsByAmount_Transaction(const QString &amount);


    //CONTROLE DE SAISIE AVANCE
    bool eventFilter(QObject *obj, QEvent *event);
    void preventCellEdit_Transactions();
    void showEditChoices_Transactions(int row, int column);
    void showChoicesUnderCell_Transactions(int row, int column, const QStringList &choices);
    void handleCellClicked_Transactions(int row, int column);
    void editMontantCell_Transactions(int row, int column);
    void updateCategoryOptions();

    //NOTIFICATIONS
    void envoyerNotificationTransaction(QString a, QString b);
    void envoyerNofiticatiionDepassementRevenuesEtDepenses();

    //STATS
    void AfficherEtmettreAJourPieChartTransaction();

    void on_faceIDButton_clicked();

signals:
    void signal_table_de_transaction_mise_a_jour(); // Declare the dataUpdated signal
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model; // Declare a member variable for the model
};

#endif // MAINWINDOW_H

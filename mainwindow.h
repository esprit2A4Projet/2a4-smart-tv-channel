#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "connection.h"
#include "employee.h"
#include "transaction.h"
#include "podcast.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QMovie>
#include <QThread>
#include <QLabel>
#include <QMessageBox>
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QStringListModel>
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //MISE A JOUR DE LA TABLE
    void update_table_de_transactions();

    //NOTIFICATIONS
    void afficherListeNotifications();
    void setupListView();


private slots:
void displaySalaryChart();
 void clearStatistics();
void validateCIN();
 void updateMessageLabel(const QString &text);
 void validateAndAddCIN();
// Louay :

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
    // Feriel
    void checkEmailFormat(const QString &text);
    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_67_clicked();

    void on_pushButton_68_clicked();

    void on_pushButton_69_clicked();

    void on_pushButton_70_clicked();

    void on_pushButton_92_clicked();

    void on_pushButton_71_clicked();
// Ibtissem :

    void on_pushButton_ajouterInvite_clicked();
    void on_pushButton_deleteInvite_clicked(); // Add the slot for delete button
    void on_pushButton_updateInvite_clicked();
    void on_pushButton_annulerInvite_clicked();
    void on_pushButton_rechercherInvite_clicked();
    void on_pushButton_trierInvite_clicked();
    void on_pushButton_exporterInvite_clicked();
    void updateTableWidgetInvite(const QString &filter); // Rename the slot to match the implementation
    void showCustomMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon);
    void validateEmail(const QString &text);
    void validateNbAbonnes(const QString &text);
    void validateNbParticipation(const QString &text);
    void validateNom(const QString &text);
    void validatePrenom(const QString &text);
    void validateProfession(const QString &text);

    // Mokh :
    void updateTableWidget_S();
    void on_pushButton_ajouterS_clicked();

    void on_pushButton_supprimerS_clicked();

    void on_pushButton_modifierS_clicked();

    void on_pushButton_annulerS_clicked();

    void on_pushButton_rechercherS_clicked();

    void on_pushButton_trierS_clicked();

    void on_PDF_clicked();

    void on_pushButton_reset_clicked();
// Zoh :
void updateTableWidgetM();
    //BOUTTON D'AJOUT, AFFICHAGE ET SUPPRESSION
    void on_pushButton_AjoutMateriaux_clicked();
    void on_pushButton_SuppressionMateriaux_clicked();
    void on_pushButton_ModificationMateriaux_clicked();
    void on_pushButton_AnnulerMateriaux_clicked();

    //METIERS STANDARDS : TRI PAR DATE EXPORTATION EXCEL ET RECHERCHE
    void on_Button_trierParDateMateriaux_clicked();
    void on_pushButton_exportationFormatExcelMateriel_clicked();
    void searchMateriauxByType(const QString &type);


    //CONTROLE DE SAISIE AVANCE
    bool eventFilterT(QObject *obj, QEvent *event);
    void preventCellEdit();
    void showEditChoices(int row, int column);
    void showChoicesUnderCell(int row, int column, const QStringList &choices);
    void handleCellClicked(int row, int column);
    void editnomCell(int row, int column);


// Ismail :
    void on_pushButton_ajouterpod_clicked();

    void on_pushButton_annulerpod_clicked();

    void on_pushButton_supprimerpod_clicked();

    void on_pushButton_modifierpod_clicked();

    void on_pushButton_expo_clicked();

    void on_pushButton_historique_clicked();

    void on_pushButton_recherchep_clicked();

    void on_pushButton_trierpod_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_QR_clicked();

    void on_clear_clicked();

    void on_EnregistrerQRCode_clicked();

    void on_pushButtonDisplaySalaryChart_clicked();
    void statistiquesEmploye();

signals:
     void employeeAdded();
    void dataUpdated(); // Declare the dataUpdated signal
    void dataUpdatedS();
    void dataUpdatedM(); // Declare the dataUpdated signal
 void signal_table_de_transaction_mise_a_jour(); // Declare the dataUpdated signal

private:
    QSqlQueryModel *modelI; // Declare a member variable for the model
    QHash<int, int> idToRowMap;
    Ui::MainWindow *ui;

    Employee e;

     QStandardItemModel *model; // Déclaration du modèle ici

    podcast p;
     Transaction Etmp;
 QList<int> cinList;
   //  SortedStringListModel *listeNotifications_Transactions;
};
#endif // MAINWINDOW_H


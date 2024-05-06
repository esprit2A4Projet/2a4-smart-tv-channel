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
#include "todolist.h"
#include <QCalendarWidget>
#include <QGraphicsSimpleTextItem>
#include <QVariant>
#include "arduino.h"
#include <QSerialPort>
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    //MISE A JOUR DES TABLES
    void update_table_de_transactions();
    void updateTableWidgetInvite(const QString &filter);
    void updateTableWidget_S();
    void updateTableWidgetM();
    void logToFile(const QString &type,const QString &message);



private slots:
    //-------------------------EMPLOYES--------------------//
    //void displaySalaryChart();
    //void clearStatistics();
    //void validateCIN();
    void updateMessageLabel(const QString &text);
    //void validateAndAddCIN();
    void checkEmailFormat(const QString &text);
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_67_clicked();
    void on_pushButton_68_clicked();
    void on_pushButton_69_clicked();
    void on_pushButton_70_clicked();
    void on_pushButton_92_clicked();
    void on_pushButton_71_clicked();
    void on_pushButton_3_clicked();
    //void on_QR_clicked();
    void on_clear_clicked();
    void on_EnregistrerQRCode_clicked();
    //void on_pushButtonDisplaySalaryChart_clicked();
    void statistiquesEmploye();
    void on_pushButton_clicked();
    //-----------------------------------------------------//


    //---------------------------------------------------------TRANSACTIONS------------------------------------------------------//
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
    //------------------------------------------------------------------------------------------------------------------------------//



    //------------------------------------------------------INVITES-----------------------------------------------------------------//
    void on_pushButton_ajouterInvite_clicked();
    void on_pushButton_deleteInvite_clicked(); // Add the slot for delete button
    void on_pushButton_updateInvite_clicked();
    void on_pushButton_annulerInvite_clicked();
    void on_pushButton_rechercherInvite_clicked();
    void on_pushButton_trierInvite_clicked();
    void on_pushButton_exporterInvite_clicked();
    void showCustomMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon);
    void validateEmail(const QString &text);
    void validateNbAbonnes(const QString &text);
    void validateNbParticipation(const QString &text);
    void validateNom(const QString &text);
    void validatePrenom(const QString &text);
    void validateProfession(const QString &text);
    void validateEmailInvite(const QString &text);
    void on_pushButton_rafraichir_clicked();
    void on_calendarWidget_clicked();
    void on_pushButton_envoyer_clicked();
    bool isPodcastDate(const QDate &date);
    QString getPodcastNameByDate(const QDate &date);
    void mailStatus(const QString &status);
    void statistiquesNbAbonnes();
    void on_pushButton_actualiser_clicked();
    void on_pushButton_imprimer_clicked();
    void on_calendarWidget_clicked(const QDate &date);
    QString getPodcastInfoByDate(const QDate &date);
    bool eventFilterI(QObject *obj, QEvent *event);
    //-------------------------------------------------------------------------------------------------------------------------------//


    //-----------------------------------------------------------SPONSORS------------------------------------------------------------//
    void on_pushButton_ajouterS_clicked();
    void on_pushButton_supprimerS_clicked();
    void on_pushButton_modifierS_clicked();
    void on_pushButton_annulerS_clicked();
    void on_pushButton_rechercherS_clicked();
    void on_pushButton_trierS_clicked();
    void on_pushButton_trierdateS_clicked();
    void on_PDF_clicked();
    void on_pushButton_reset_clicked();
    void on_pushButton_annulerS2_clicked();
    void on_SMS_clicked();
    void statistiquesS();
    void on_exportstat_clicked();

    // Control de saisie
    void validateNomS(const QString &text);
    void validateBudget(const QString &text);
    void validateTelephone(const QString &text);
    void on_tableWidget_S_cellDoubleClicked(int row, int column);
    void on_tableWidget_S_itemChanged(QTableWidgetItem *item);


    // Slots for toolbar actions
    void onAddToDo();
    void onRemoveToDo();
    void saveToDoListToFile(const QString& filename);
    void loadToDoListFromFile(const QString& filename);
    void closeEvent(QCloseEvent *event);
    void onTabChanged(int index);
    //arduino
    //void readFromSerialPort();
    void checkForRFIDTag();
    void handleRFIDScanned(const QString &RFIDTag);
    bool getUserInfoFromRFID(const QString &RFIDTag, QString *CIN, QString *mdp);


    //-------------------------------------------------MATERIAUX----------------------------------------------------------------------//
        //BOUTTON D'AJOUT, AFFICHAGE ET SUPPRESSION
        void on_pushButton_AjoutMateriaux_clicked();
        void on_pushButton_SuppressionMateriaux_clicked();
        void on_pushButton_ModificationMateriaux_clicked();
        void on_pushButton_AnnulerMateriaux_clicked();
        //METIERS STANDARDS : TRI PAR DATE EXPORTATION EXCEL ET RECHERCHE
        void on_Button_trierParDateMateriaux_clicked();
        void on_pushButton_exportationFormatExcelMateriel_clicked();
        void searchMateriauxByType(const QString &type);
        void selectMaterialAndInsertIntoContenir();
        void AfficherEtmettreAJourPieChartMateriaux();
        void handleConfirmationButtonClick();
        //void showMaterialStats();
        //BILAN FINANCIER
        void bilanFinancier();
        void afficherResultatsBilanFinancier(QTextEdit *textEdit, int totalTransactions, const QString& modesDePaiement, double totalRevenus, double totalDepenses, double beneficeOuPerte);
        //CONTROLE DE SAISIE
        bool eventFilterM(QObject *obj, QEvent *event);
        void preventCellEdit();
        void showEditChoices(int row, int column);
        void showChoicesUnderCell(int row, int column, const QStringList &choices);
        void handleCellClicked(int row, int column);
        void editnomCell(int row, int column);
        //void on_tabWidget_currentChanged(int index);


    //---------------------------------------------------------------------------------------------------------------------------//


    //-------------------------------------------------------PODCASTS-----------------------------------------------------------//
        void on_pushButton_ajouterpod_clicked();

        void on_pushButton_annulerpod_clicked();

        void on_pushButton_supprimerpod_clicked();

        void on_pushButton_modifierpod_clicked();

        void on_pushButton_expo_clicked();

        void on_pushButton_historique_clicked();

        void on_pushButton_recherchep_clicked();

        void on_pushButton_trierpod_clicked();

        void statistiquesP();

        void on_pod_chatbox_2_clicked();

        void on_spons_chatbox_2_clicked();

        void on_tres_chatbox_2_clicked();

        void on_invite_chatbox_2_clicked();

        void on_logist_chatbox_3_clicked();

        void on_emp_chatbox_2_clicked();
    //----------------------------------------------------------------------------------------------------------------------------//


    void on_connexionAvecFaceIDPushButton_clicked();


signals:
    void employeeAdded();
    void dataUpdated();
    void dataUpdatedS();
    void dataUpdatedM();
    void signal_table_de_transaction_mise_a_jour();
    void statisticsTabSelected();
    void setCenter(QVariant, QVariant);
    void addMarker(QVariant, QVariant);


private:
    QSqlQueryModel *modelI; // Declare a member variable for the model
    QHash<int, int> idToRowMap;
    Ui::MainWindow *ui;
    Employee e;
    QStandardItemModel *model; // Déclaration du modèle ici
    podcast p;
    QList<int> cinList;
    QSqlQueryModel *modelS;
    CToDoList *toDoListWidget;
    QToolBar* mainToolBar;
    QAction* addToDoAction;
    QAction* removeToDoAction;
    QCalendarWidget *calendarWidget;
    bool dateSelected = false;
    QByteArray data;
    QSerialPort *serial;
    Arduino A;
};
#endif // MAINWINDOW_H


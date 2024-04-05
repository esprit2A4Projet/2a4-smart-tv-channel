//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QComboBox>
#include "Materiaux.h"
#include <QTextEdit>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    int getCurrentPodcastId();
public slots:
    //MISE A JOUR DE LA TABLE
    void updateTableWidgetM();
    void selectMaterialAndInsertIntoContenir();

private slots:

    //BOUTTON D'AJOUT, AFFICHAGE ET SUPPRESSION
    void on_pushButton_AjoutMateriaux_clicked();
    void on_pushButton_SuppressionMateriaux_clicked();
    void on_pushButton_ModificationMateriaux_clicked();
    void on_pushButton_AnnulerMateriaux_clicked();

    //METIERS STANDARDS : TRI PAR DATE EXPORTATION EXCEL ET RECHERCHE
    void on_Button_trierParDateMateriaux_clicked();
    void on_pushButton_exportationFormatExcelMateriel_clicked();
    void searchMateriauxByType(const QString &type);
    //BILAN FINANCIER
    void bilanFinancier();
    void afficherResultatsBilanFinancier(QTextEdit *textEdit, double totalMateriaux, double totalTransactions, double totalRevenus, double totalDepenses, const QString& modesDePaiement);
    void analyserTransactionsParCategorie(QTableWidget *tableWidget);
    QString filtrerModesPaiement(const QString& modesDePaiement, const QString& typeTransaction);
    QString getTypePaiement(const QString& mode);
    QString getIdTransaction(const QString& mode);
    void afficherMateriauxParTransaction(int idTransaction, QTextEdit *textEdit);
    void exporterBilan(QTextEdit *textEditResultats);
    //void exporterBilanCSV(QTextEdit *textEditResultats);


    //CONTROLE DE SAISIE
    bool eventFilter(QObject *obj, QEvent *event);
    void preventCellEdit();
    void showEditChoices(int row, int column);
    void showChoicesUnderCell(int row, int column, const QStringList &choices);
    void handleCellClicked(int row, int column);
    void editnomCell(int row, int column);

    void on_tabWidget_currentChanged(int index);
    void showMaterialStats();


signals:
    void dataUpdated(); // Declare the dataUpdated signal
    void statisticsTabSelected();
private:
    Ui::MainWindow *ui;
    Materiaux Etmp;
    QSqlQueryModel *model; // Declare a member variable for the model
    QTextEdit *textEditResultats;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QComboBox>
#include "Materiaux.h"

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
    void updateTableWidgetM();

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
    void selectMaterialsForPodcast();


    //CONTROLE DE SAISIE
    bool eventFilter(QObject *obj, QEvent *event);
    void preventCellEdit();
    void showEditChoices(int row, int column);
    void showChoicesUnderCell(int row, int column, const QStringList &choices);
    void handleCellClicked(int row, int column);
    void editnomCell(int row, int column);



signals:
    void dataUpdated(); // Declare the dataUpdated signal
private:
    Ui::MainWindow *ui;
    Materiaux Etmp;
    QSqlQueryModel *model; // Declare a member variable for the model
};

#endif // MAINWINDOW_H

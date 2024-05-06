#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "employee.h"
#include "login.h"
#include "invite.h"
#include "sponsor.h"
#include "materiaux.h"
#include "connection.h"
#include <QInputDialog>
#include <QTextStream>
#include "qrcodegen.hpp"
#include <QDesktopServices>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextDocument>
#include<QUrl>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QTableWidget>

#include <QKeyEvent>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>

#include <fstream>
#include <QtCharts>
#include <QChartView>

#include <QFont>
#include "qrcode.hpp"

#include "xlsxdocument.h"
#include "xlsxformat.h"
#include <QImage>
#include <QBuffer>
#include <QFile>

#include <QProcess>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include "connection.h"
#include "todolist.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDialog>
#include <QLabel>
#include <QPrinter>
#include <QFileDialog>
#include <QPdfWriter>
#include <QTextDocument>
#include <QComboBox>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QEvent>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

#include <QtCharts/QPieSlice>
#include <QGeoCodingManager>
#include <QGeoCoordinate>
#include <QGeoAddress>
#include <QGeoServiceProvider>
#include <QQuickItem>
#include <QQuickWidget>

#include <QTcpSocket>
#include <QtCore>
#include "smtp.h"
#include <QtNetwork>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QSqlRecord>
#include <QPrinter>
#include <QPrintDialog>
#include "arduino.h"
#include <QSerialPort>
using namespace qrcodegen;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    serial(new QSerialPort(this))
{
    //******************************************************************************************************************************************************************//
    //******************************************************************************************************************************************************************//
    //******************************************************************************************************************************************************************//
    //******************************************************************************************************************************************************************//
    //---------------------------------------------------------------LE CONSTRUCTEUR DU MAINWINDOW----------------------------------------------------------------------//
    //******************************************************************************************************************************************************************//
    //******************************************************************************************************************************************************************//
    //******************************************************************************************************************************************************************//
       ui->setupUi(this);

       int ret = A.connect_arduino();
       switch (ret) {
           case 0:
               qDebug() << "Arduino is available and connected to: " << A.getarduino_port_name();
               //QMessageBox::information(this, "Arduino connecté", "Arduino connecté avec succès.");
               break;
           case 1:
               qDebug() << "Arduino is available but not connected to: " << A.getarduino_port_name();
               break;
           case -1:
               qDebug() << "Arduino is not available";
               //QMessageBox::critical(this, "Erreur de connexion Arduino", "Échec de la connexion à Arduino. Veuillez vous assurer qu'il est correctement connecté et réessayer.");
               break;
       }

               connect(A.getserial(), SIGNAL(readyRead()), this, SLOT(checkForRFIDTag()));
//------------------------------------------------------------------------------------EMPLOYES--------------------------------------------------------------------//
       Employee E;
       statistiquesEmploye();
       ui->lineEdit_nom->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
       ui->lineEdit_prenom->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
       ui->lineEdit_salaire->setValidator(new QRegExpValidator(QRegExp("[0-9]*\\.?([0-9]+)?"), this));
       ui->lineEdit_cin->setValidator(new QIntValidator(0, 999999, this));
       ui->lineEdit_id_login->setValidator(new QIntValidator(0, 999999, this));
       ui->lineEdit_codeQR->setValidator(new QIntValidator(0, 999999, this)); 
       connect(ui->lineEdit_cin, &QLineEdit::textChanged, this, &MainWindow::updateMessageLabel);
       ui->lineEdit_3->setValidator(new QIntValidator(0,  999999, this));
       ui->lineEdit_8->setValidator(new QIntValidator(0,  999999, this));
       ui->comboBox->addItem("Ressources Humaines");
       ui->comboBox->addItem("R.podcast");
       ui->comboBox->addItem("R. sponsoring");
       ui->comboBox->addItem("tresorier");
       ui->comboBox->addItem("C.Invites");
       ui->comboBox->addItem("R.logistique");
       ui->comboBox->addItem("Autre");
       // Connectez le signal textChanged à une fonction de vérification personnalisée
       connect(ui->lineEdit_email, &QLineEdit::textChanged, this, &MainWindow::checkEmailFormat);
       model = new QStandardItemModel(this);
       ui->tableView->setModel(model);
       model->setColumnCount(6); // Nous avons 5 colonnes dans notre TableView
       model->setHorizontalHeaderLabels({ "Nom", "Prénom","email", "Date Embauche", "Salaire", "Poste","cin"});
       ui->tableView->setModel(model);
       // NEW LOGIN :
       //ui->label_gif_animation->setVisible(true);
       ui->label_gif_animation_2->setVisible(true);
       // Spécifiez le chemin absolu complet du fichier GIF
       QString gifPath = "C:/Users/user/Desktop/projet_integration/projet/animation logo version finale.gif";
       // Créez le QMovie en utilisant le chemin absolu complet
       QMovie *gifanimation = new QMovie(gifPath);
       // Définissez le QMovie sur le QLabel
       ui->label_gif_animation_2->setMovie(gifanimation);
       // Démarrez l'animation
       gifanimation->start();
       ui->tabWidget->setCurrentIndex(7);
      int expectedIndex = 7;  // Define the expected index value
       if (ui->tabWidget->currentIndex() == expectedIndex) {
           // Disable specific tabs when the current index matches the expected index
           ui->tabWidget->setTabEnabled(0, false);
           ui->tabWidget->setTabEnabled(1, false);
           ui->tabWidget->setTabEnabled(2, false);
           ui->tabWidget->setTabEnabled(3, false);
           ui->tabWidget->setTabEnabled(4, false);
           ui->tabWidget->setTabEnabled(5, false);  // This line is redundant if index 5 is already disabled
           ui->tabWidget->setTabEnabled(6, false);
           ui->tabWidget->setTabEnabled(7, true);
       }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------INVITES-----------------------------------------------------------------------------------//
       connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget())); // connecter le signal envers le slot
        ui->lineEdit_nomI->installEventFilter(this);
        ui->lineEdit_prenomI->installEventFilter(this);
       //initialiser la table
       ui->tableWidget->setColumnCount(7);
       QStringList labels{"ID", "Nom", "Prenom", "Profession", "Email", "Nombre d'abonnes", "Nombre de Participation"};
       ui->tableWidget->setHorizontalHeaderLabels(labels);
       Connection c;
       if (c.createconnect())
       {
           QString searchTerm =ui->lineEdit_rechercherInvite->text();
           updateTableWidgetInvite(searchTerm);
       }
       connect(ui->pushButton_annulerInvite, &QPushButton::clicked, this, &MainWindow::on_pushButton_annulerInvite_clicked);
       //controle de saisie

       ui->label_emailError->setVisible(false);
       ui->label_nbAbonnesError->setVisible(false);
       ui->label_nbParticipationError->setVisible(false);
       ui->label_nomError->setVisible(false);
       ui->label_prenomError->setVisible(false);
       ui->label_professionError->setVisible(false);
       ui->label_emailInviteError->setVisible(false);

       connect(ui->lineEdit_email, &QLineEdit::textChanged, this, &MainWindow::validateEmail);
       connect(ui->lineEdit_nbAbonnesI, &QLineEdit::textChanged, this, &MainWindow::validateNbAbonnes);
       connect(ui->lineEdit_nbParticipationI, &QLineEdit::textChanged, this, &MainWindow::validateNbParticipation);
       connect(ui->lineEdit_nom, &QLineEdit::textChanged, this, &MainWindow::validateNom);
       connect(ui->lineEdit_prenom, &QLineEdit::textChanged, this, &MainWindow::validatePrenom);
       connect(ui->lineEdit_professionI, &QLineEdit::textChanged, this, &MainWindow::validateProfession);
       connect(ui->lineEdit_emailInvite, &QLineEdit::textChanged, this, &MainWindow::validateEmailInvite);
       /****************FONCTIONNALITES******************/
       Invite i;
       i.afficherDatePodcastsCalendrier(ui->calendarWidget);

       ui->tableWidget->hideColumn(0); // Cette ligne masque la première colonne
       connect(ui->calendarWidget, SIGNAL(clicked(const QDate&)), this, SLOT(on_calendarWidget_clicked(const QDate&)));

       statistiquesNbAbonnes();
       // Dans le constructeur ou l'initialisation de votre classe MainWindow
       disconnect(ui->calendarWidget, SIGNAL(clicked(const QDate&)), this, SLOT(on_calendarWidget_clicked(const QDate&)));
       connect(ui->calendarWidget, SIGNAL(clicked(const QDate&)), this, SLOT(on_calendarWidget_clicked(const QDate&)));
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------//



//----------------------------------------------------------------------------SPONSORS-----------------------------------------------------------------------------//
       //*****************************dashboard*****************************//
       toDoListWidget = new CToDoList(this);
       //readFromSerialPort();

           // Create a tab widget if it doesn't exist already
           if (!ui->dashboard) {
               ui->dashboard = new QTabWidget(this);
               setCentralWidget(ui->dashboard);
           }

           // Add the instance of CToDoList to the tab widget
           ui->dashboard->addTab(toDoListWidget, "To Do List");
           toDoListWidget->setMinimumSize(1000, 1000);

           connect(ui->dashboard, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
           mainToolBar = addToolBar("Main Toolbar");

               // Create actions for the toolbar

               addToDoAction = new QAction(QIcon(":/new/prefix2/add.png"), "Add To Do", this);
               connect(addToDoAction, &QAction::triggered, this, &MainWindow::onAddToDo);
               mainToolBar->addAction(addToDoAction);

               removeToDoAction = new QAction(QIcon(":/new/prefix2/remove.png"), "Remove To Do", this);
               connect(removeToDoAction, &QAction::triggered, this, &MainWindow::onRemoveToDo);
               mainToolBar->addAction(removeToDoAction);

               QTimer::singleShot(0, this, [=]() {
                                  mainToolBar->setVisible(false);
                              });

               connect(this, &MainWindow::saveToDoListToFile, toDoListWidget, &CToDoList::saveToDoListToFile);
               connect(this, &MainWindow::loadToDoListFromFile, toDoListWidget, &CToDoList::loadToDoListFromFile);

               //***************************************maps*********************************************************//
               //ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/new/prefix4/map.qml")));
               ui->quickWidget->show();

               auto obj = ui->quickWidget->rootObject();
               connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
               connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));
               emit setCenter(36.898488, 10.189342);
               emit addMarker(36.898488, 10.189342);


       connect(this, SIGNAL(dataUpdatedS()), this, SLOT(updateTableWidget_S())); // Connect the signal to the slot
       ui->tableWidget_S->setColumnCount(7);
       ui->tableWidget_S->hideColumn(0);
       QStringList labelsS{"ID","Nom", "Budget", "Pack", "Date debut", "Date fin", "Telephone"};
       ui->tableWidget_S->setHorizontalHeaderLabels(labelsS);

       ui->label_NomErrorS->setVisible(false);
       ui->label_BudgetError->setVisible(false);
       ui->label_DateError->setVisible(false);
       ui->label_Date2Error->setVisible(false);
       ui->label_TelError->setVisible(false);

       // Connect the cellDoubleClicked signal to the on_tableWidget_S_cellDoubleClicked slot
       connect(ui->tableWidget_S, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_tableWidget_S_cellDoubleClicked(int,int)));
       // Connect the itemChanged signal to the on_tableWidget_S_itemChanged slot
       connect(ui->tableWidget_S, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(on_tableWidget_S_itemChanged(QTableWidgetItem*)));
       //connect(ui->tableWidget_S, &QTableWidget::cellDoubleClicked, this, &MainWindow::preventCellEditS);
       connect(ui->lineEdit_nomS, &QLineEdit::textChanged, this, &MainWindow::validateNomS);
       connect(ui->lineEdit_budget, &QLineEdit::textChanged, this, &MainWindow::validateBudget);
       connect(ui->lineEdit_tel, &QLineEdit::textChanged, this, &MainWindow::validateTelephone);
       statistiquesS();
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------//




//--------------------------------------------------------------------------------MATERIAUX------------------------------------------------------------------------//
       //-----------------------------------installation des reflexes selon les signaux--------------------------------//
       ui->lineEdit_nomM->installEventFilter(this);
       installEventFilter(this);
       connect(ui->tableWidgetM, &QTableWidget::cellDoubleClicked, this, &MainWindow::preventCellEdit);
       connect(ui->tableWidgetM, &QTableWidget::cellClicked, this, &MainWindow::showEditChoices);
       connect(ui->tableWidgetM, &QTableWidget::cellClicked, this, &MainWindow::handleCellClicked);
       connect(ui->lineEdit_RechercheParType, &QLineEdit::textChanged, this, &MainWindow::searchMateriauxByType);
       connect(ui->tableWidgetM, &QTableWidget::cellClicked, this, &MainWindow::editnomCell);
       connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidgetM()));
       connect(ui->pushButton_selectMaterialsForPodcast, &QPushButton::clicked, this, &MainWindow::selectMaterialAndInsertIntoContenir);
       //connect(this, &MainWindow::statisticsTabSelected, this, &MainWindow::showMaterialStats);
       AfficherEtmettreAJourPieChartMateriaux();
       connect(ui->pushButton_calculerBilan, &QPushButton::clicked, this, &MainWindow::bilanFinancier);
       QPushButton *exportButton = new QPushButton("Exporter Bilan", ui->tabWidget);
       //exportButton->setStyleSheet("background-color: #E8E8FD;");
       ui->tabWidget->setCornerWidget(exportButton, Qt::TopRightCorner); // Positionnez le bouton dans le coin supérieur droit du QTabWidget
       connect(exportButton, &QPushButton::clicked, [=]() {
                           });
       ui->tableWidgetM->setColumnCount(7);
       QStringList labelsM{"ID","Nom", "Type", "Etat", "Quantite", "Date", "idTransaction"};
       ui->tableWidgetM->setHorizontalHeaderLabels(labelsM);
       ui->tableWidgetM->hideColumn(0); // Indice de la colonne ID
       updateTableWidgetM();
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//




//-------------------------------------------------------------------------TRANSACTIONS-----------------------------------------------------------------------------//
       //-----------------------------------installation des reflexes selon les signaux--------------------------------//
       updateCategoryOptions();
       envoyerNofiticatiionDepassementRevenuesEtDepenses();
       AfficherEtmettreAJourPieChartTransaction();
       ui->lineEdit_Montant_Transaction->installEventFilter(this);
       ui->lineEdit_RechercheParMONTANT_Transaction->installEventFilter(this);
       installEventFilter(this);
       connect(ui->table_de_transactions, &QTableWidget::cellDoubleClicked, this, &MainWindow::preventCellEdit_Transactions);
       connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::showEditChoices_Transactions);
       connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::handleCellClicked_Transactions);
       connect(ui->lineEdit_Type_Transaction, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCategoryOptions);
       connect(ui->lineEdit_RechercheParMONTANT_Transaction, &QLineEdit::textChanged, this, &MainWindow::searchTransactionsByAmount_Transaction);
       connect(ui->table_de_transactions, &QTableWidget::cellClicked, this, &MainWindow::editMontantCell_Transactions);
       connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget()));
        //--------------------------------Initialisation du tableau et ses champs----------------------------------------------//
       ui->table_de_transactions->setColumnCount(6);
       QStringList labelsT{"Mode de paiement", "Type", "Categorie", "Date de transaction", "Montant en dinar","ID"};
       ui->table_de_transactions->setHorizontalHeaderLabels(labelsT);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//



//---------------------------------------------------------------------------PODCASTS-------------------------------------------------------------------------------//
       ui->lineEdit_nomp->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
       ui->lineEdit_dureep->setValidator(new QIntValidator(0, 999, this));
       ui->lineEdit_lieup->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
       ui->comboBoxp->currentText();
       ui->dateEditp->text();
       ui->lineEdit_7p->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));
       ui->lineEdit_8p->setValidator(new QRegExpValidator(QRegExp("[A-Za-z ]+"), this));


       ui->comboBoxp->addItem("Education");
       ui->comboBoxp->addItem("Sports");
       ui->comboBoxp->addItem("Finance");
       ui->comboBoxp->addItem("Politiques");

       statistiquesP();

       podcast p;

              model = new QStandardItemModel(this);
               ui->tableViewp->setModel(model);

                 model->setColumnCount(5); // Nous avons 5 colonnes dans notre tableViewp
                    model->setHorizontalHeaderLabels({"ID", "Nom du Podcast", "Durée min", "Lieu", "Categorie", "Date Podcast"});
                    ui->tableViewp->setModel(model);
                    ui->tableViewp->hideColumn(0);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
}



//******************************************************************************************************************************************************************//
//******************************************************************************************************************************************************************//
//******************************************************************************************************************************************************************//
//******************************************************************************************************************************************************************//
//-------------------------------------------------------LE DEVELOPPEMENT DE TOUT LES FONCTIONS DU MAINWINDOW-------------------------------------------------------//
//******************************************************************************************************************************************************************//
//******************************************************************************************************************************************************************//
//******************************************************************************************************************************************************************//



//---------------------------------------------------------------------EMPLOYES feriel------------------------------------------------------------------------------//
void MainWindow::updateMessageLabel(const QString &text)
{
    if (text.length() == 6) {
        ui->label_message->setText("");
    } else {
        ui->label_message->setText("Veuillez saisir exactement 6 chiffres.");
    }
}
void MainWindow::statistiquesEmploye()
{
    Connection C;
    C.createconnect();
    QSqlQueryModel *model = new QSqlQueryModel();
    // Retrieve the count of employees in different salary ranges
    model->setQuery("SELECT * FROM employes where SALAIRE < 1000");
    float exp1 = model->rowCount();
    model->setQuery("SELECT * FROM employes where SALAIRE >= 1000 AND SALAIRE < 1500");
    float exp2 = model->rowCount();
    model->setQuery("SELECT * FROM employes WHERE SALAIRE >= 1500 AND SALAIRE < 2000 ");
    float exp3 = model->rowCount();
    model->setQuery("SELECT * FROM employes WHERE SALAIRE >= 2000");
    float exp4 = model->rowCount();
    float total = exp1 + exp2 + exp3 + exp4;
    // Calculate percentages and create labels
    QString a = QString("  salaire &lt; 1000 ") + QString::number((exp1 * 100) / total, 'f', 2) + "%";
    QString b = QString(">1000DT & Salaire  &lt; 1500 ") + QString::number((exp2 * 100) / total, 'f', 2) + "%";
    QString c = QString(">1500DT & Salaire  &lt; 2000 ") + QString::number((exp3 * 100) / total, 'f', 2) + "%";
    QString d = QString("Salaire >2000DT ") + QString::number((exp4 * 100) / total, 'f', 2) + "%";
    // Create the pie series
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.3);
    // Set colors for each slice
    series->append(a, (exp1 * 100) / total)->setBrush(QColor("#CCCCCC"));
    series->append(b, (exp2 * 100) / total)->setBrush(QColor("#4682B4"));
    series->append(c, (exp3 * 100) / total)->setBrush(QColor("#D1D0FB"));
    series->append(d, (exp4 * 100) / total)->setBrush(QColor("#041041"));
    // Set label visibility and exploded state for slices
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible();
        if (slice->value() == 0) {
            slice->setExploded();
        }
    }
    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    //chart->setTitle("Statistiques sur les packs des employes");
    // Set the chart theme to a light theme to avoid conflicts with custom colors
    chart->setTheme(QChart::ChartThemeLight);
    // Create the chart view and set its properties
    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setParent(ui->horizontalFrame);
    chartview->setGeometry(10, 20 - 50, 600, 400);  // Adjust position if needed
}

void MainWindow::checkEmailFormat(const QString &text)
{
    if (!text.contains('@'))
    {
        ui->lineEdit_email->setStyleSheet("background-color: red"); // Mettre en évidence en rouge pour indiquer une erreur

    }
    else
    {
        ui->lineEdit_email->setStyleSheet("background-color: white"); // Rétablir le style par défaut
    }
}
void MainWindow::on_pushButton_23_clicked() // Bouton Valider
{
    QString RFIDTag = ui->lineEdit_RFID->text();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString date = ui->dateEdit->text();
    QString email = ui->lineEdit_email->text();
    int cin = ui->lineEdit_cin->text().toInt();
    int salaire = ui->lineEdit_salaire->text().toInt();
    QString poste = ui->comboBox->currentText();
    int id = ui->lineEdit_3->text().toInt(); // Assuming this is the employee ID
    Employee e(nom, prenom, email, date, salaire, poste, cin, RFIDTag );
    bool success = false;
        if (id == 0 && Employee::isCinUnique(cin) )
        {
            success = e.ajouter(poste);
        }
        else if (id == 0 && !(Employee::isCinUnique(cin) ))
        {
             QMessageBox::critical(this, "Erreur", "CIN existe déjà. Veuillez saisir un CIN unique");
        }
        else
        {
            success = e.update(id);
            ui->lineEdit_3->clear();
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Modification effectué\n"
                                                 "Click Cancel to exit."), QMessageBox::Cancel);
        }
        QMessageBox msgBox;
        if (success)
        {
            ui->lineEdit_email->setStyleSheet("background-color: white");
            statistiquesEmploye();
            ui->tableView->setModel(e.afficher());
            ui->lineEdit_nom->clear();
            ui->lineEdit_prenom->clear();
            ui->lineEdit_email->clear();
            ui->dateEdit->clear();
            ui->lineEdit_salaire->clear();
            ui->lineEdit_cin->clear();
            ui->label_message->clear();
        }
        else
        {
            msgBox.setText("Operation failed.");
            msgBox.exec();
        }
}

void MainWindow::on_pushButton_22_clicked() // Button annuler
{
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->dateEdit->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_cin->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_RFID->clear();
}

void MainWindow::on_pushButton_71_clicked() // Afficher
{
      Employee E;
      ui->tableView->setModel(E.afficher());
}

void MainWindow::on_pushButton_67_clicked()  // bouton supprimer
{
       int id = ui->lineEdit_3->text().toInt();
       Employee E;
       QSqlQuery query;
       query.prepare("SELECT nom, prenom,email, date_embauche, salaire, poste,cin, RFIDTag FROM employes WHERE cin = ?");
       query.addBindValue(id);
       if (query.exec() && query.next())
       {
               QString nom = query.value(0).toString();
               QString prenom = query.value(1).toString();
               QString email = query.value(2).toString();
               QString date_embauche = query.value(3).toString();
               int salaire = query.value(4).toInt();
               QString poste = query.value(5).toString();
               int cin = query.value(6).toInt();
               QString RFIDTag = query.value(7).toString();
               Employee(nom,prenom,email,date_embauche,salaire,poste,cin, RFIDTag);
               if (E.supprimer(id))
               {
                   ui->tableView->setModel(E.afficher());                 
                   QMessageBox::information(nullptr, QObject::tr("OK"),
                               QObject::tr("Suppression effectuée\n"
                                           "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
               }
               else
               {
                   // Afficher un message indiquant que la suppression a échoué
                   QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                       QObject::tr("La suppression a échoué.\n"
                                                   "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
               }
       }
       else
       {
               // Afficher un message indiquant que l'employé est introuvable
               QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                   QObject::tr("Employé introuvable.\n"
                                               "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
       }
           ui->lineEdit_3->clear();
}

void MainWindow::on_pushButton_69_clicked() // Tri
{
      Employee E;
      ui->tableView->setModel(E.tri());
      bool test=E.tri();
      if(test)
      {
          QMessageBox::information(nullptr,QObject::tr("OK"),
                                   QObject::tr("tri effectué. \n"
                                               "appuyez sur annuler pour quitter."),QMessageBox::Cancel);
      }
      else
      {
          QMessageBox::critical(nullptr, QObject::tr("not OK"),
                      QObject::tr("tri non effectué.\n"
                                  "appuyez sur annuler pour quitter."), QMessageBox::Cancel);
      }
}

void MainWindow::on_pushButton_68_clicked() //Bouton  MODIFIER
{
    int id = ui->lineEdit_3->text().toInt();
    Employee E;
    QSqlQuery query;
    query.prepare("SELECT nom, prenom,email, date_embauche, salaire, poste ,cin , rfidtag FROM employes WHERE cin = ?");
    query.addBindValue(id);
    if (query.exec() && query.next())
    {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        QString email = query.value(2).toString();
        QString date_embauche = query.value(3).toString();
        int salaire = query.value(4).toInt();
        QString poste = query.value(5).toString();
        int cin = query.value(6).toInt();
        QString RFIDTag = query.value(7).toString();
        ui->lineEdit_nom->setText(nom);
        ui->lineEdit_prenom->setText(prenom);
        ui->lineEdit_email->setText(email);
        ui->dateEdit->setDate(QDate::fromString(date_embauche, "yyyy-MM-dd"));
        ui->lineEdit_salaire->setText(QString::number(salaire));
        ui->comboBox->setCurrentText(poste);
        ui->lineEdit_cin->setText(QString::number(cin));
        ui->lineEdit_RFID->setText(RFIDTag);
        Employee(nom,prenom,email,date_embauche,salaire,poste,cin,RFIDTag);
     }
     else
     {
                // Afficher un message indiquant que l'employé est introuvable
                QMessageBox::critical(nullptr, QObject::tr("Error"),
                                      QObject::tr("Employé inexistant (68)!!!.\n"
                                                  "Click Cancel to exit."), QMessageBox::Cancel);
                qDebug() << "Query: " << query.lastQuery();
                qDebug() << "Error: " << query.lastError().text();
     }
}

void MainWindow::on_pushButton_70_clicked() // chercher
{
      Employee E;
      int id = ui->lineEdit_8->text().toInt();
      QSqlQuery query;
      query.prepare("SELECT nom, prenom,email, date_embauche, salaire, poste,cin,RFIDTag FROM employes WHERE cin = ?");
      query.addBindValue(id);
      if (query.exec() && query.next())
      {
          QString nom = query.value(0).toString();
          QString prenom = query.value(1).toString();
          QString email = query.value(2).toString();
          QString date_embauche = query.value(3).toString();
          int salaire = query.value(4).toInt();
          QString poste = query.value(5).toString();
          int cin = query.value(6).toInt();
          QString RFIDTag = query.value(7).toString();
          Employee(nom,prenom,email,date_embauche,salaire,poste,cin,RFIDTag);
          if (E.Rechercher(id))
          {
               ui->tableView->setModel(E.Rechercher(id));
              QMessageBox::information(nullptr, QObject::tr("OK"),
                            QObject::tr("Employé  Trouvé\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
          }
          else
          {
                  // Afficher un message indiquant que l'employé est introuvable
                  QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                        QObject::tr("Employé inexistant! (70)\n"
                                                    "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
          }
      }
      else
      {
              // Afficher un message indiquant que l'employé est introuvable
              QMessageBox::critical(nullptr, QObject::tr("Error"),
                                    QObject::tr("Employee inexistant.\n"
                                                "Click sur Cancel pour quitter."), QMessageBox::Cancel);
       }
       ui->lineEdit_8->clear();
}
void MainWindow::on_pushButton_92_clicked()  // Exportation PDF :
{
    QString strStream;
    strStream = QFileDialog::getSaveFileName((QWidget*)0, "Export PDF", QString(), "*.pdf");
    if (QFileInfo(strStream).suffix().isEmpty())
    {
        strStream.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(strStream);

    QTextStream out(&strStream);

    const int rowCount = ui->tableView->model()->rowCount();
    const int columnCount = ui->tableView->model()->columnCount();
    QString TT = QDateTime::currentDateTime().toString();

    // Adjusted HTML to place the image in the top right corner
    out << "<html>\n"
           "<head>\n"
          // "<meta Content=\"Text/html; charset=Windows-1251\">\n"
          // "<title>ERP - COMmANDE LIST</title>\n "
           "<style>"
           "body { margin: 0; padding: 0; }"
           "table { width: 100%; }"
           ".logo { position: absolute; top: 10px; right: 10px; width: 100px; }"  // Adjusted top and right values
                    "</style>"
                    "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<h1 style=\"text-align: center;\"><strong> " + TT +  "</strong></h1>"
           "<h1 style=\"text-align: center;\"><strong> "  "</strong></h1>"
           "<img src=\"C:/Users/user/Desktop/projet_integration (4)/projet_integration/projet/logo_new.png\" class=\"logo\">" // Update the path and class

          // "<p  >           <img src=\"C:/Users/MSI/OneDrive - ESPRIT/Bureau/Nouveau dossier/projet/logo\" class=\"logo\"></p>" // Update the path and class

          //  "<p >Faire la kamelia avant l'image                 &emsp;         image  oo  <img src=\"C:\Users\user\Desktop\projet_integration (4)\projet_integration\projet\logo_new.png"\" class=\"logo\"></p>"

           "<h1 style=\"text-align: center;\"><strong> ****LISTE DES EMPLOYES **** </strong></h1>"
           "<h1 style=\"text-align: center;\"><strong>  </strong></h1>"

           // Table style adjusted to span the entire page width
           "<table style=\"text-align: center; font-size: 10px;\" border=1>\n "
           "</br> </br>";

    // headers
    out << "<thead><tr bgcolor=#d6e5ff>";
    for (int column = 0; column < columnCount; column++)
    {
        if (!ui->tableView->isColumnHidden(column))
        {
            out << QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++)
        {
            if (!ui->tableView->isColumnHidden(column))
            {
                QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, column)).toString().simplified();
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out << "</table>\n"
           "</body>\n"
           "</html>\n";
    QTextDocument document;
    document.setHtml(strStream);
    document.print(&printer);
}
void MainWindow::on_pushButton_3_clicked()  // bouton login
{
    int enteredUsername = ui->lineEdit_id_login->text().toInt();
    QString enteredPassword = ui->lineEdit_mdp_login->text();
    Login loginObject;
    int userId = loginObject.validateLogin(enteredUsername, enteredPassword);
    if (userId != -1)
    {
        switch (userId)
        {
            case 1: // Employé RH
                ui->tabWidget->setCurrentIndex(1);
                ui->tabWidget->setTabEnabled(1, true);
                ui->tabWidget->setTabEnabled(0, true);
                userId=-1;
                break;
            case 2:
                ui->tabWidget->setCurrentIndex(2);
                ui->tabWidget->setTabEnabled(2, true);
                ui->tabWidget->setTabEnabled(0, true);
                break;
            case 3:
                ui->tabWidget->setCurrentIndex(3);
                ui->tabWidget->setTabEnabled(3, true);
                ui->tabWidget->setTabEnabled(0, true);
                break;
            case 4:
                ui->tabWidget->setCurrentIndex(4);
                ui->tabWidget->setTabEnabled(4, true);
                ui->tabWidget->setTabEnabled(0, true);
                break;
            case 5:
                ui->tabWidget->setCurrentIndex(5);
                ui->tabWidget->setTabEnabled(5, true);
                ui->tabWidget->setTabEnabled(0, true);
                break;
            case 6:
                ui->tabWidget->setCurrentIndex(6);
                ui->tabWidget->setTabEnabled(6, true);
                ui->tabWidget->setTabEnabled(0, true);
                break;
            case 0:
                ui->tabWidget->setCurrentIndex(0);
                ui->tabWidget->setTabEnabled(0, true);
                ui->tabWidget->setTabEnabled(0, true);
                break;
            default:
                break;
        }
        // Désactivez les onglets inutiles
        for (int i = 1; i < 7; i++) {
            if ( ui->tabWidget->currentIndex()==7)
            {
                  ui->tabWidget->setTabEnabled(i, false);

            }
            if (i != ui->tabWidget->currentIndex()) {
                ui->tabWidget->setTabEnabled(i, false);
            }
        }

    } else {
        // Connexion échouée, affichez un message d'erreur
        QMessageBox::warning(this, "Erreur de connexion", "Identifiants incorrects !");
    }


    // Effacez les champs de connexion après traitement
    ui->lineEdit_id_login->clear();
    ui->lineEdit_mdp_login->clear();
}


void MainWindow::on_pushButton_clicked() // QR CODE
{
    QString cin = ui->lineEdit_codeQR->text(); // Obtenir la valeur de CIN à partir du line edit

    if (!cin.isEmpty()) {
        QSqlQuery qry;
        qry.prepare("SELECT nom, prenom, email, date_embauche, salaire, poste, cin, mot_de_passe FROM employes WHERE CIN=:cin");
        qry.bindValue(":cin", cin);
        qry.exec();

        if (qry.next()) {
            QString nom = qry.value(0).toString();
            QString prenom = qry.value(1).toString();
            QString email = qry.value(2).toString();
            QString date_embauche = qry.value(3).toString();
            int salaire = qry.value(4).toInt();
            QString poste = qry.value(5).toString();
            QString mot_de_passe = qry.value(7).toString();

            // Création de la chaîne d'informations de l'employé
            QString ide = "Nom: " + nom + "\nPrénom: " + prenom +
                          "\nEmail: " + email + "\nDate d'embauche: " + date_embauche +
                          "\nSalaire: " + QString::number(salaire) + "\nPoste: " + poste +
                          "\nCIN: " + cin + "\nMot de passe: " + mot_de_passe;

            // Encoder les informations de l'employé dans un code QR
            QrCode qr = QrCode::encodeText(ide.toUtf8().constData(), QrCode::Ecc::HIGH);

            // Convertir le code QR en une image avec une marge autour
            int margin = 40; // Taille de la marge (multipliée par 2 pour une taille deux fois plus grande)
            QImage im(qr.getSize() + 2 * margin, qr.getSize() + 2 * margin, QImage::Format_RGB888);
            im.fill(Qt::white); // Remplir l'image avec du blanc
            for (int y = 0; y < qr.getSize(); y++) {
                for (int x = 0; x < qr.getSize(); x++) {
                    int color = qr.getModule(x, y); // 0 pour blanc, 1 pour noir
                    im.setPixelColor(x + margin, y + margin, (color == 0) ? Qt::white : Qt::black);
                }
            }

            // Afficher l'image du code QR avec une taille agrandie dans l'interface utilisateur
            ui->qrCode->setPixmap(QPixmap::fromImage(im).scaled(300, 300)); // Taille de l'affichage : largeur 300, hauteur 300

        } else {
            QMessageBox::warning(this, "Erreur", "CIN non trouvé dans la base de données.");
        }
    }
}
void MainWindow::on_clear_clicked()
{
    ui->qrCode->clear();
    ui->labelMessage->clear();
    ui->lineEdit_codeQR->clear();
}

void MainWindow::on_EnregistrerQRCode_clicked()
{
    // Vérifier si une image est affichée dans qrCode
    if (ui->qrCode->pixmap() != nullptr && !ui->qrCode->pixmap()->isNull()) {
        // Récupérer l'image affichée dans l'interface
        QPixmap pixmap = ui->qrCode->pixmap()->scaled(400, 400); // Taille de l'image : 400x400 pixels

        // Enregistrer l'image dans le répertoire spécifié
        QString cin = ui->lineEdit_codeQR->text();
        if (!cin.isEmpty()) {
            QString filePath = "C:/Users/user/Desktop/projet_integration (4)/projet_integration/projet/" + cin + ".png";
            if (pixmap.save(filePath, "PNG")) {
                QString message = "Image enregistrée avec succès sous le chemin:\n" + filePath;
                ui->labelMessage->setText(message); // Mettre à jour le texte du QLabel
                ui->labelMessage->setStyleSheet("color: green;"); // Changer la couleur du texte en vert (facultatif)
            } else {
                ui->labelMessage->setText("Erreur lors de l'enregistrement de l'image.");
                ui->labelMessage->setStyleSheet("color: red;"); // Changer la couleur du texte en rouge (facultatif)
            }
        } else {
            ui->labelMessage->setText("Le champ CIN est vide.");
            ui->labelMessage->setStyleSheet("color: red;"); // Changer la couleur du texte en rouge (facultatif)
        }
    } else {
        ui->labelMessage->setText("Aucune image de code QR à enregistrer.");
        ui->labelMessage->setStyleSheet("color: red;"); // Changer la couleur du texte en rouge (facultatif)
    }
    ui->lineEdit_codeQR->clear();
}
//---------------------------------------------------------------------EMPLOYES feriel------------------------------------------------------------------------------//




//---------------------------------------------------------------------INVITES ibtissem-----------------------------------------------------------------------------//
/************************************************************************CRUD***********************************************************************************/
/*-----------------------------------------------------------------------Ajouter Un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_ajouterInvite_clicked() {
    // Valider tous les champs
    validateEmail(ui->lineEdit_emailI->text());
    validateNbAbonnes(ui->lineEdit_nbAbonnesI->text());
    validateNbParticipation(ui->lineEdit_nbParticipationI->text());
    validateNom(ui->lineEdit_nomI->text());
    validatePrenom(ui->lineEdit_prenomI->text());
    validateProfession(ui->lineEdit_professionI->text());
    // Vérifier si toutes les validations sont réussies et si tous les champs sont remplis
    if (ui->label_emailError->isVisible() ||
        ui->label_nbAbonnesError->isVisible() ||
        ui->label_nbParticipationError->isVisible() ||
        ui->label_nomError->isVisible() ||
        ui->label_prenomError->isVisible() ||
        ui->label_professionError->isVisible() ||
        ui->lineEdit_emailI->text().isEmpty() ||
        ui->lineEdit_nbAbonnesI->text().isEmpty() ||
        ui->lineEdit_nbParticipationI->text().isEmpty() ||
        ui->lineEdit_nomI->text().isEmpty() ||
        ui->lineEdit_prenomI->text().isEmpty() ||
        ui->lineEdit_professionI->text().isEmpty()) {
        showCustomMessageBox("Erreur", "Veuillez vérifier les données saisies.", QMessageBox::Critical);
        return;
    }
    // Si toutes les validations réussissent et tous les champs sont remplis, ajouter l'invite à la base de données
    Invite i;
    i.setnom(ui->lineEdit_nomI->text());
    i.setprenom(ui->lineEdit_prenomI->text());
    i.setprofession(ui->lineEdit_professionI->text());
    i.setemail(ui->lineEdit_emailI->text());
    i.setnbAbonnes(ui->lineEdit_nbAbonnesI->text().toInt());
    i.setnbParticipation(ui->lineEdit_nbParticipationI->text().toInt());
    Connection c;
    if (c.createconnect())
    {
        if (i.ajouterInvite())
        {
            updateTableWidgetInvite(ui->lineEdit_rechercherInvite->text());
            showCustomMessageBox("Succès", "Succès d'ajout dans la base de données", QMessageBox::Information);
        } else
        {
            showCustomMessageBox("Erreur", "Erreur de l'insertion dans la base de données", QMessageBox::Critical);
        }
    } else
    {
        showCustomMessageBox("Erreur", "Connexion impossible à la base de données.", QMessageBox::Critical);
    }
}
/*---------------------------------------------------------Mettre à jour les informations d'un Invite--------------------------------------------------------------*/
void MainWindow::on_pushButton_updateInvite_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidget->item(row, 0)->text().toInt();

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidget->item(row, 1)->text();
        QString prenom = ui->tableWidget->item(row, 2)->text();
        QString profession = ui->tableWidget->item(row, 3)->text();
        QString email = ui->tableWidget->item(row, 4)->text();
        QString nbAbonnesStr = ui->tableWidget->item(row, 5)->text();
        QString nbParticipationStr = ui->tableWidget->item(row, 6)->text();

        // Contrôle de saisie pour le nom et le prénom (max 10 caractères)
        bool isNomValid = !nom.isEmpty() && nom.length() <= 10;
        bool isPrenomValid = !prenom.isEmpty() && prenom.length() <= 10;

        // Contrôle de saisie pour la profession (commencer par une majuscule)
        bool isProfessionValid = !profession.isEmpty() && profession.at(0).isUpper();

        // Contrôle de saisie pour l'email (format valide)
        QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
        bool isEmailValid = emailRegex.exactMatch(email);

        // Vérifiez si toutes les validations ont réussi
        QString errorMessage;
        if (!isNomValid) {
            errorMessage += "Le nom ne doit pas dépasser 10 lettres\n";
        }
        if (!isPrenomValid) {
            errorMessage += "Le prenom ne doit pas dépasser 10 lettres\n";
        }
        if (!isProfessionValid) {
            errorMessage += "La profession doit commencer par une lettre majuscule \n";
        }
        if (!isEmailValid) {
            errorMessage += "Verifier le format de l'email\n";
        }

        // Vérifiez si nbAbonnes et nbParticipation sont des entiers
        bool isNbAbonnesValid, isNbParticipationValid;
        int nbAbonnes = nbAbonnesStr.toInt(&isNbAbonnesValid);
        int nbParticipation = nbParticipationStr.toInt(&isNbParticipationValid);
        if (!isNbAbonnesValid) {
            errorMessage += "Nombre d'abonnés doit être un entier.\n";
        }
        if (!isNbParticipationValid) {
            errorMessage += "Nombre de participations doit être un entier.\n";
        }

        if (!errorMessage.isEmpty()) {
            showCustomMessageBox("Erreur", errorMessage, QMessageBox::Critical);
            return;
        }

        // Appelez la fonction modifierInvite pour mettre à jour la base de données
        Invite i;
        if (i.modifierInvite(idToModify, nom, prenom, profession, email, nbAbonnes, nbParticipation)) {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(prenom));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(profession));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(email));

            ui->tableWidget->item(row, 5)->setFlags(ui->tableWidget->item(row, 5)->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->item(row, 6)->setFlags(ui->tableWidget->item(row, 6)->flags() & ~Qt::ItemIsEditable);

            emit dataUpdated(); // Mettre à jour la vue après la modification
            showCustomMessageBox("Succès", "Succès de la modification dans la base de données", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Erreur", "Connexion impossible à la base de données.", QMessageBox::Critical);
        }
    }
    else
    {
        showCustomMessageBox("Attention", "Sélectionnez une ligne à modifier", QMessageBox::Warning);
    }
}

/*-----------------------------------------------------------------------Supprimer Un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_deleteInvite_clicked()
{
   QItemSelectionModel *select = ui->tableWidget->selectionModel();
   QModelIndexList selectedIndexes = select->selectedIndexes();
   if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
       QTableWidgetItem *item = ui->tableWidget->item(row, 0);
       int idToDelete = item->text().toInt();
       Invite i;
       if (i.supprimerInvite(idToDelete))
       {
           ui->tableWidget->removeRow(row);
           emit dataUpdated(); // Mettre à jour la vue après la suppression
           showCustomMessageBox("Success", "Data deleted from the database.", QMessageBox::Information);
       }
       else
       {
           showCustomMessageBox("Error", "Failed to delete data from the database.", QMessageBox::Critical);
       }
   }
   else
   {
       showCustomMessageBox("Warning", "Please select a row to delete.", QMessageBox::Warning);
   }
}
/***************************************************************************FONCTIONNALITES**************************************************************************************/
/*-----------------------------------------------------------------------Rechercher Un Invite----------------------------------------------------------------------------------*/
void MainWindow::on_pushButton_rechercherInvite_clicked()
{
   QString searchTerm = ui->lineEdit_rechercherInvite->text();
   updateTableWidgetInvite(searchTerm);
}


/*-----------------------------------------------------------------------Trier les Invites-----------------------------------------------------------------------------------*/
void MainWindow::on_pushButton_trierInvite_clicked()
{
    // Vérifier quelle option de tri est sélectionnée dans le comboBox
    QString triSelectionne = ui->comboBox_I->currentText();

    Invite i;
    if (triSelectionne == "Nombre d'abonnés")
    {
        if (i.trierParNbAbonnes(ui->tableWidget))
        {
            emit dataUpdated();
            showCustomMessageBox("Succès", "Données triées par nombre d'abonnés", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Erreur", "Echec de tri de données", QMessageBox::Critical);
        }
    }
    else if (triSelectionne == "Nombre de participation")
    {
        if (i.trierParNbParticipations(ui->tableWidget))
        {
            emit dataUpdated();
            showCustomMessageBox("Succès", "Données triées par nombre de participations", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Erreur", "Echec de tri de données", QMessageBox::Critical);
        }
    }

}
/*-----------------------------------------------------------------------Exporter les informations d'un Invite-------------------------------------------------------------------*/
void MainWindow::on_pushButton_exporterInvite_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Sauvegarder en PDF", "", "Fichiers PDF (*.pdf)");

    if (!filePath.isEmpty())
    {
        QString strStream;
        QTextStream out(&strStream);

        const int rowCount = ui->tableWidget->rowCount();
        const int columnCount = ui->tableWidget->columnCount();

        out << "<html>\n"
               "<head>\n"
               "<meta Content=\"Text/html; charset=Windows-1251\">\n"
               << QString("<title>%1</title>\n").arg("Liste des invites")
               << "<style>"
               "   body { font-family: 'Space Grotesk'; background-color: #ffffff; }"
               "   table { border-collapse: collapse; width: 100%; }"
               "   th, td { border: 1px solid #dddddd; text-align: left; padding: 8px; }"
               "   th { background-color: #E8E8FD; }"
               "</style>\n"
               "</head>\n"
               "<body>\n";

        // Add logo and application name
        out << "<div style=\"text-align: right;\"><img src=\"C:/Users/user/Desktop/projet_integration (4)/projet_integration/projet/tsawer/logoo.png\" width=\"250\" height=\"100\"><br>"
               "<h1 style=\"color: #E8E8FD; text-align: center;\">Liste des invites</h1></div>\n";



        // Add table
        out << "<table>\n"
               "<thead><tr> <th>Numéro</th>";
        for (int column = 0; column < columnCount; column++)
            out << QString("<th>%1</th>").arg(ui->tableWidget->horizontalHeaderItem(column)->text());
        out << "</tr></thead>\n";

        // Add data rows
        for (int row = 0; row < rowCount; row++)
        {
            out << "<tr> <td>" << row + 1 << "</td>";
            for (int column = 0; column < columnCount; column++)
            {
                QTableWidgetItem *item = ui->tableWidget->item(row, column);
                QString data = (item) ? item->text().simplified() : "&nbsp;";
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : "&nbsp;");
            }
            out << "</tr>\n";
        }
        out << "</table>\n";

        // Add copyright and team information
        out << "<br><br><div style=\"text-align: center;\">"
               " 2024 Insightify. Tout droits reservees.<br>"
               "Developpee par Insightify Team</div>\n";

        out << "</body>\n"
               "</html>\n";

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(filePath);

        QTextDocument doc;
        doc.setHtml(strStream);
        doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);

        showCustomMessageBox("Succès", "Tableau exporté en PDF.", QMessageBox::Information);
    }
    else
    {
        showCustomMessageBox("Erreur", "Échec de l'exportation du tableau en PDF.", QMessageBox::Critical);
    }
}

/*-----------------------------------------------------------------------Effacer le formulaire-------------------------------------------------------------------*/
void MainWindow::on_pushButton_annulerInvite_clicked()
{
   ui->lineEdit_nomI->clear();
   ui->lineEdit_prenomI->clear();
   ui->lineEdit_professionI->clear();
   ui->lineEdit_emailI->clear();
   ui->lineEdit_nbAbonnesI->clear();
   ui->lineEdit_nbParticipationI->clear();
}

/*-----------------------------------------------------Rafraichir la table apres chaque modification------------------------------------------------------------*/
void MainWindow::updateTableWidgetInvite(const QString &filter)
{
   Connection c;
   QSqlQueryModel *model = new QSqlQueryModel();
   if (filter.isEmpty()) {
       // Si le filtre de recherche est vide, récupérer toutes les lignes
       model->setQuery("SELECT * FROM INVITE");
   } else {
       // Si un filtre de recherche est spécifié, utiliser la requête filtrée
       QString query = QString("SELECT * FROM INVITE WHERE Nom LIKE '%1%'").arg(filter);
       model->setQuery(query);
   }

   int RowNumber = 0;
   ui->tableWidget->setRowCount(0);

   while (model->canFetchMore()) {
       model->fetchMore();
   }

   for (int row = 0; row < model->rowCount(); ++row) {
           // Ajouter la ligne uniquement si le filtre est vide ou si c'est la première ligne de la recherche
           ui->tableWidget->insertRow(RowNumber);
           for (int col = 0; col < 7; ++col) {
               QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
               ui->tableWidget->setItem(RowNumber, col, item);
           }
           RowNumber++;
       }

   delete model;
}

/*-----------------------------------------------------------------------Modifier le style du Message Box-------------------------------------------------------------------*/
void MainWindow::showCustomMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon)
{
   // Créer la boîte de message avec le texte et l'icône fournis
   QMessageBox messageBox(icon, title, message, QMessageBox::Ok, this);
   // Obtenir le bouton Ok
   QAbstractButton *okButton = messageBox.button(QMessageBox::Ok);
   // Styles pour le bouton Ok
   QString okButtonStyle = "QPushButton {"
                           "background-color: #D1D0FB;"
                           "color: white;"
                           "border: 1px solid #C4C4C4;"
                           "border-radius: 5px;"
                           "padding: 5px;"
                           "margin: 4px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: #D3D3D3;"
                           "}";
   okButton->setStyleSheet(okButtonStyle);
   // Appliquer les styles directement au QLabel (texte)
   QLabel *label = messageBox.findChild<QLabel*>("qt_msgbox_label");
   if (label) {
       label->setStyleSheet("min-width: 300px; text-align: center;");
   }
   // Styles pour l'ensemble de la QMessageBox
   QString messageBoxStyle = "QMessageBox {"
                             "background-color: #E8E8FD;"
                             "color: white;"
                             "font-size: 16px;"
                             "font-family: 'Space Grotesk';"
                             "}"
                             "QPushButton:pressed {"
                             "background-color: #A5A5A5;"
                             "}";
   messageBox.setStyleSheet(messageBoxStyle);
   // Connecter l'événement hover personnalisé pour le bouton Ok
   connect(okButton, &QAbstractButton::pressed, [=]() {
       okButton->setStyleSheet("background-color: #A5A5A5;");
   });
   connect(okButton, &QAbstractButton::released, [=]() {
       okButton->setStyleSheet("background-color: #D3D3D3;");
   });

   // Ajuster la taille de la boîte de message
   messageBox.setMinimumSize(300, 100);

   // Afficher la boîte de message
   messageBox.exec();
}
/*---------------------------------------------------------------------------Controle de Saisie-----------------------------------------------------------------------------------*/
// Fonction de validation de l'e-mail
void MainWindow::validateEmail(const QString &text) {
    QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
    bool isValid = emailRegex.exactMatch(text);
    ui->label_emailError->setVisible(!isValid);
    ui->label_emailError->setStyleSheet(isValid ? "" : "color: red;");
}
void MainWindow::validateEmailInvite(const QString &text) {
    QRegExp emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
    bool isValid = emailRegex.exactMatch(text);
    ui->label_emailInviteError->setVisible(!isValid);
    ui->label_emailInviteError->setStyleSheet(isValid ? "" : "color: red;");
}

// Fonction de validation du nombre d'abonnés
void MainWindow::validateNbAbonnes(const QString &text)
{
    if (!text.isEmpty() && !text.toInt()) {
        // Si le texte n'est pas vide mais ne peut pas être converti en entier, le supprimer
        ui->lineEdit_nbAbonnesI->setText("");
    }
}

// Fonction de validation du nombre de participations
void MainWindow::validateNbParticipation(const QString &text)
{
    if (!text.isEmpty() && !text.toInt()) {
        // Si le texte n'est pas vide mais ne peut pas être converti en entier, le supprimer
        ui->lineEdit_nbParticipationI->setText("");
    }
}
void MainWindow::validateNom(const QString &text) {
    bool isValid = !text.isEmpty() && text.length() <= 10; // Le nom ne doit pas être vide et ne doit pas dépasser 10 caractères
    ui->label_nomError->setVisible(!isValid);
    ui->label_nomError->setStyleSheet(isValid ? "" : "color: red;");
}

void MainWindow::validatePrenom(const QString &text) {
    bool isValid = !text.isEmpty() && text.length() <= 10; // Le prénom ne doit pas être vide et ne doit pas dépasser 10 caractères
    ui->label_prenomError->setVisible(!isValid);
    ui->label_prenomError->setStyleSheet(isValid ? "" : "color: red;");
}

void MainWindow::validateProfession(const QString &text) {
    bool isValid = !text.isEmpty() && text.at(0).isUpper(); // La profession ne doit pas être vide et doit commencer par une lettre majuscule
    ui->label_professionError->setVisible(!isValid);
    ui->label_professionError->setStyleSheet(isValid ? "" : "color: red;");
}

/*---------------------------------------------------------------------------CALENDRIER-----------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************************************************************************************/

/*--------Rafraichir le calendrier-------------*/
void MainWindow::on_pushButton_rafraichir_clicked()
{
    Invite i;
    // Appeler la fonction pour afficher les dates des podcasts sur le calendrier
    i.afficherDatePodcastsCalendrier(ui->calendarWidget);
    // Effacer le contenu du lineEdit de l'email
    ui->lineEdit_emailInvite->clear();
    ui->calendarWidget->setMinimumDate(QDate(1900, 1, 1));
    ui->calendarWidget->setMaximumDate(QDate(3000, 1, 1));

       // Sélectionner une date qui ne sera jamais utilisée
       ui->calendarWidget->setSelectedDate(QDate(2024, 1, 1));

}
/*--------Verifier si le calendrier est selectionné-------------*/
void MainWindow::on_calendarWidget_clicked()
{
     dateSelected = true;

}

/*---------------------------------------------------------------------------EMAIL-----------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************************************************************************************/

/*--------Verifier si l'email existe dans la bdd-------------*/
bool emailExists(const QString &email)
{
    Connection c;
    if (!c.createconnect())
    {
        return false;
    }

    QSqlQuery query(c.db);
    query.prepare("SELECT COUNT(*) FROM INVITE WHERE Email = ?");
    query.addBindValue(email);

    if (!query.exec())
    {

        return false;
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        if (count > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
/*-------------Envoyer l'Email----------------*/

void MainWindow::on_pushButton_envoyer_clicked()
{
    QString emailAddress = ui->lineEdit_emailInvite->text();
    QDate selectedDate = ui->calendarWidget->selectedDate();

    // Validate the email address
    validateEmailInvite(emailAddress);

    // Check if a date is selected
    if (!emailAddress.isEmpty() && selectedDate.isValid())
    {
        // Check if the email exists in the database
        if (emailExists(emailAddress))
        {
            // Check if the selected date is a podcast date
            if (isPodcastDate(selectedDate))
            {
                // Fetch the name of the podcast based on the selected date
                QString podcastName = getPodcastNameByDate(selectedDate);

                // Create an instance of the Smtp class
                Smtp *smtp = new Smtp("ibtissembenamara14@gmail.com", "pgygpnhtzubfjruk", "smtp.gmail.com", 465);

                // Connect signals from the Smtp object to slots in the MainWindow class to capture status updates or errors
                connect(smtp, SIGNAL(status(QString)), this, SLOT(mailStatus(QString)));

                // Construct the email subject with the podcast date
                QString dateString = selectedDate.toString("dd/MM/yyyy");
                QString subject = "Invitation au podcast \"" + podcastName + "\" du " + dateString;

                // Construct the email message content
                QString messageContent = "Bonjour,\n\nVous êtes cordialement invité au podcast \"" + podcastName + "\" du " + dateString + ".\n\nCordialement,\nInsightify Team";

                // Attempt to send the email
                if (smtp->sendMail("ibtissembenamara14@gmail.com", emailAddress, subject, messageContent))
                {
                    QMessageBox::information(this, "Succès", "L'email a été envoyé avec succès!");
                }
                else
                {

                    QMessageBox::critical(this, "Erreur", "Échec de l'envoi de l'email.");
                }

            }
            else
            {

                QMessageBox::warning(this, "Date invalide", "La date sélectionnée n'est pas une date de podcast valide.");
            }
        }
        else
        {

            QMessageBox::critical(this, "Erreur", "L'e-mail n'existe pas dans la base de données. Impossible d'envoyer l'e-mail.");
        }
    }
    else
    {

        QMessageBox::warning(this, "Champs vides", "Veuillez saisir une adresse e-mail et sélectionner une date avant d'envoyer l'e-mail.");
    }
}

/*--------Verifier si la date selectionnée est une date de podcast-------------*/

bool MainWindow::isPodcastDate(const QDate &date)
{
    Connection c;
    if (!c.createconnect()) return false;

    QSqlQuery query(c.db);

    if (!query.exec("SELECT DATE_POD FROM PODCASTS")) {

        c.db.close();
        return false;
    }

    QList<QDate> podcastDates;

    while (query.next()) {
        QString dateString = query.value(0).toString();
        QDate podcastDate = QDate::fromString(dateString, "dd/MM/yyyy");
        if (podcastDate.isValid()) {
            podcastDates.append(podcastDate);
        } else {

        }
    }

    return podcastDates.contains(date);
}

/*--------obtenir le nom du podcast suivant la date-------------*/

QString MainWindow::getPodcastNameByDate(const QDate &date)
{
    Connection c;
    if (!c.createconnect())
    {

        return QString();
    }

    QSqlQuery query(c.db);

    query.prepare("SELECT NOM FROM PODCASTS WHERE TO_CHAR(TO_DATE(DATE_POD, 'DD/MM/YYYY'), 'DD/MM/YYYY') = :date");
    query.bindValue(":date", date.toString("dd/MM/yyyy"));

    if (!query.exec())
    {

        c.db.close();
        return QString();
    }

    if (query.next())
    {
        QString podcastName = query.value(0).toString();
        c.db.close();
        return podcastName;
    }
    else
    {

        c.db.close();
        return QString();
    }
}



/*--------le status de l'email-------------*/
void MainWindow::mailStatus(const QString &status)
{
    qDebug() << "Mail status:" << status;
}


/*-----------------------------------------------------------------STATISTIQUES------------------------------------------------------------------------*/
/*******************************************************************************************************************************************************************************************/

void MainWindow::statistiquesNbAbonnes()
{
    Connection C;
        C.createconnect();
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM INVITE");

        int totalCount = model->rowCount();

        int count0_1000 = 0;
        int count1000_10000 = 0;
        int count10000_100000 = 0;
        int count100000_plus = 0;

        for (int i = 0; i < model->rowCount(); ++i) {
            int nbAbonnes = model->record(i).value("nbAbonnes").toInt();
            if (nbAbonnes >= 0 && nbAbonnes <= 1000) {
                count0_1000++;
            } else if (nbAbonnes > 1000 && nbAbonnes <= 10000) {
                count1000_10000++;
            } else if (nbAbonnes > 10000 && nbAbonnes <= 100000) {
                count10000_100000++;
            } else if (nbAbonnes > 100000) {
                count100000_plus++;
            }
        }

        // Calculate percentages
        qreal percent0_1000 = static_cast<qreal>(count0_1000) / totalCount * 100;
           qreal percent1000_10000 = static_cast<qreal>(count1000_10000) / totalCount * 100;
           qreal percent10000_100000 = static_cast<qreal>(count10000_100000) / totalCount * 100;
           qreal percent100000_plus = static_cast<qreal>(count100000_plus) / totalCount * 100;

        QString a = QString("0-1k: %1%").arg(percent0_1000, 0, 'f', 2);
        QString b = QString("1k-10k: %1%").arg(percent1000_10000, 0, 'f', 2);
        QString c = QString("10k-100kk: %1%").arg(percent10000_100000, 0, 'f', 2);
        QString d = QString("100k+: %1%").arg(percent100000_plus, 0, 'f', 2);




    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.2);
    series->append(a, count0_1000);
    series->append(b, count1000_10000);
    series->append(c, count10000_100000);
    series->append(d, count100000_plus);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QFont titleFont("Space Grotesk", 11, QFont::Bold);
    chart->setTitleFont(titleFont);
    chart->setTitle("Statistiques des nombres d'invités selon nombre d'abonnés");
    chart->setBackgroundBrush(QBrush(QColor("#E8E8FD")));

    QList<QColor> sliceColors = {
        QColor("#354c7c"),
        QColor("#0b1338"),
        QColor("#a69fca"),
        QColor("#d9d9d9")
    };

    int colorIndex = 0;
    for (QAbstractSeries *series : chart->series()) {
        QPieSeries *pieSeries = qobject_cast<QPieSeries *>(series);
        if (pieSeries) {
            for (QPieSlice *slice : pieSeries->slices()) {
                slice->setColor(sliceColors.at(colorIndex % sliceColors.size()));
                slice->setLabelVisible();
                colorIndex++;
            }
        }
    }

    QFont font("Space Grotesk");
    font.setPointSize(9);
    chart->setTitleFont(font);

    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setMouseTracking(true);

    ui->stat_I->setVisible(true);
    ui->stat_I->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QLayout *layout = ui->stat_I->layout();
    if (layout)
        delete layout;

    layout = new QVBoxLayout(ui->stat_I);
    layout->addWidget(chartview);
    layout->setAlignment(Qt::AlignCenter);
    ui->stat_I->setLayout(layout);
}

/*-----------------Actualiser le piechart--------------------*/

void MainWindow::on_pushButton_actualiser_clicked()
{
    statistiquesNbAbonnes();
}

/*-----------------Imprimer le piechart--------------------*/

void MainWindow::on_pushButton_imprimer_clicked()
{
    // Créer un objet printer
    QPrinter printer(QPrinter::HighResolution);

    // Configurer le dialogue d'impression
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() != QDialog::Accepted)
        return; // Sortir si l'utilisateur annule l'impression

    // Créer un painter pour dessiner sur le printer
    QPainter painter(&printer);

    // Récupérer la taille du chartview
    QSize size = ui->stat_I->size();

    // Créer un facteur d'échelle pour adapter le graphique à la taille de la page A4
    double margin = 50; // in millimeters
    double width = printer.pageRect().width() - 2 * margin;
    double height = printer.pageRect().height() - 2 * margin;
    QSize chartSize(qRound(width), qRound(height));

    // Définir la zone de visualisation du painter pour correspondre à la taille du graphique
    painter.setViewport(0, 0, chartSize.width(), chartSize.height());

    // Centrer et mettre à l'échelle le graphique dans le painter
    double scaleFactor = qMin(chartSize.width() / size.width(), chartSize.height() / size.height());
    painter.translate((printer.pageRect().width() - size.width() * scaleFactor) / 2,
                      (printer.pageRect().height() - size.height() * scaleFactor) / 2);
    painter.scale(scaleFactor, scaleFactor);

    // Dessiner le chartview sur le painter du printer
    ui->stat_I->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
}


void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    // Récupérez les informations du podcast pour la date sélectionnée
    QString podcastInfo = getPodcastInfoByDate(date);

    // Vérifiez si des informations ont été trouvées
    if (!podcastInfo.isEmpty()) {
        // Créez une fenêtre de dialogue pour afficher les informations du podcast
        QMessageBox::information(this, "Informations du podcast", podcastInfo);
    } else {
        // Si aucune information n'a été trouvée, affichez un message d'erreur
        QMessageBox::warning(this, "Aucun podcast trouvé", "Aucun podcast n'est prévu pour la date sélectionnée.");
    }
}

// Méthode pour récupérer les informations du podcast à partir de la base de données
QString MainWindow::getPodcastInfoByDate(const QDate &date)
{
    Connection c;
    if (!c.createconnect()) {
        return QString();
    }

    QSqlQuery query(c.db);
    query.prepare("SELECT NOM, DUREE, LIEU, CATEGORIE FROM PODCASTS WHERE DATE_POD = :date");
    query.bindValue(":date", date.toString("dd/MM/yyyy"));

    if (!query.exec()) {
        c.db.close();
        return QString();
    }

    if (query.next()) {
        QString podcastName = query.value(0).toString();
        QString podcastDuration = query.value(1).toString();
        QString podcastLocation = query.value(2).toString();
        QString podcastCategory = query.value(3).toString();

        // Construisez une chaîne de texte avec les informations récupérées
        QString podcastInfo = "Nom : " + podcastName + "\n"
                            + "Durée : " + podcastDuration + "\n"
                            + "Lieu : " + podcastLocation + "\n"
                            + "Catégorie : " + podcastCategory;

        c.db.close();
        return podcastInfo;
    } else {
        c.db.close();
        return QString();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------//



//---------------------------------------------------------------------SPONSORS mokhtar---------------------------------------------------------------------------//
void MainWindow::validateNomS(const QString &text)
{
    QRegExp rx("^[a-zA-Z _]+$");
    bool isValid = rx.exactMatch(text) && text.length() <= 20;
    ui->label_NomErrorS->setVisible(!isValid);
    ui->label_NomErrorS->setStyleSheet(isValid ? "" : "color: red;");
    ui->lineEdit_nomS->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z _]+$"), this));

    // Disable focus out if not valid
    if (!isValid) {
        ui->lineEdit_nomS->setFocus();
    }
}

void MainWindow::validateBudget(const QString &text)
{

    bool budgetConversionOk;
    double budgetValue = text.toDouble(&budgetConversionOk);
    bool isValid = budgetConversionOk && text.length() <= 20;
    ui->label_BudgetError->setVisible(!isValid);
    ui->label_BudgetError->setStyleSheet(isValid ? "" : "color: red;");
    QRegExp regExp("\\d{1,20}"); // This regex allows for numbers with 1 to 20 digits
    QRegExpValidator* budgetValidator = new QRegExpValidator(regExp, this);
    ui->lineEdit_budget->setValidator(budgetValidator);

    // Disable focus out if not valid
    if (!isValid) {
        ui->lineEdit_budget->setFocus();
    }


}

void MainWindow::validateTelephone(const QString &text)
{

    bool isValid = text.length() == 8 && text.toInt(); // Check for 8 digits and numeric characters
    ui->label_TelError->setVisible(!isValid);
    ui->label_TelError->setStyleSheet(isValid ? "" : "color: red;");
    QRegExp regExp("\\d{8}");
    QRegExpValidator* telValidator = new QRegExpValidator(regExp, this);
    ui->lineEdit_tel->setValidator(telValidator);

    // Disable focus out if not valid
    if (!isValid) {
        ui->lineEdit_tel->setFocus();
    }

}

//*******************arduino*******************************************************//
void MainWindow::checkForRFIDTag()
{
    bool isopen=false;

    // Automatically trigger RFID scanning process when the application starts
    if(isopen==false)
    {
    QString tag = A.read_RFID_card();
    if (!tag.isEmpty()) {
        // If a tag is scanned, handle it
        handleRFIDScanned(tag);
        //QMessageBox::information(nullptr, "RFID Card Read", "RFID Card Successfully Read: " + tag);
    }
    }
}


void MainWindow::handleRFIDScanned(const QString &RFIDTag)
{
    QString CIN;
    QString mdp;
    //Connection c;
    // Print debug message indicating that an RFID tag is being scanned
    qDebug() << "RFID Tag Scanned: " << RFIDTag;
    qDebug() << "handleRFIDScanned function called with RFID tag: " << RFIDTag;

    if (ui->tabWidget->currentIndex() == 1)
                {
                    // Set RFID tag only when the second tab is selected
                    ui->lineEdit_RFID->setText(RFIDTag);
                }
    // Retrieve user information from the database based on the scanned RFID tag
    if (ui->tabWidget->currentIndex() == 7)
    {
    if (!RFIDTag.isEmpty()) {
        if (getUserInfoFromRFID(RFIDTag, &CIN, &mdp)) {
            // Automatically fill the CIN field
            ui->lineEdit_id_login->setText(CIN);
            // Automatically fill the Password field (optional)
            ui->lineEdit_mdp_login->setText(mdp);
            QSqlQuery query;
            QString nom;
            query.prepare("SELECT nom FROM employes WHERE cin = :CIN");
            query.bindValue(":CIN", CIN);
            if (query.exec() && query.next())
            {
                nom = query.value(0).toString();
                qDebug() << "le nom est "<< nom;
            }

            // Combine the name and role with a delimiter (space) between them
            QString userData = CIN ;
            QByteArray userDataBytes = userData.toUtf8();

            // Send the user's name and role to Arduino
            A.write_to_arduino(userDataBytes);
            QMessageBox::warning(this, "Welcome", "Bonjour et bienvenue : " + nom);

            // Automatically trigger the sign-in process
            on_pushButton_3_clicked();

        }
    }
         else {
        // Print debug message indicating failure to read RFID tag
        qDebug() << "Failed to read RFID tag.";
    }
    }


}


bool MainWindow::getUserInfoFromRFID(const QString &RFIDTag, QString *CIN, QString *mdp)
{
    qDebug() << "RFID tag received:" << RFIDTag;
    QSqlQuery query;


    query.prepare("SELECT CIN, MOT_DE_PASSE,RFIDTag FROM EMPLOYES WHERE RFIDTag= :RFIDTag");
    query.bindValue(":RFIDTag",RFIDTag);
    //query.addBindValue(RFIDTag);


    if (!query.exec())
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête: " + query.lastError().text());
       // c.closeConnection(); // Close connection on query execution failure
        return false;
    }

    else
    {
        qDebug() << "Query executed successfully";
    }

    if (query.next())
    {
        *CIN = query.value(0).toString();
        *mdp = query.value(1).toString();
        QString ref = query.value(2).toString();
        qDebug() << "Cin=" << *CIN;
        qDebug() << "mdp=" << *mdp;
        qDebug() << "ref=" << ref;
        return true;
    }
    else
    {
        qDebug() << "No record found in the database for RFID tag: " << RFIDTag;
        QMessageBox::warning(this, "Warning", "Employe non existant : " + RFIDTag);
        return false;
    }
}

//*****************************************arduino****************************************************//

void MainWindow::on_pushButton_ajouterS_clicked()
{
    Sponsor s;
    s.Setnom(ui->lineEdit_nomS->text());
    s.Setbudget(ui->lineEdit_budget->text());
    s.Setpack(ui->comboBox_pack->currentText());
    s.Setdate_deb(ui->dateEdit_SD->text());
    s.Setdate_fin(ui->dateEdit_2->text());
    s.Settelephone(ui->lineEdit_tel->text());


    QDate dateDeb = ui->dateEdit_SD->date();
    QDate dateFin = ui->dateEdit_2->date();
    if (dateFin < dateDeb) {
        QMessageBox::critical(this, "Erreur", "Plage de dates non valide. Date_fin ne devrait pas l’être avant date_deb.");
        return;
    }

    if (ui->label_NomErrorS->isVisible() ||
        ui->label_BudgetError->isVisible() ||
        ui->label_TelError->isVisible() || s.Getnom().isEmpty() || s.Getbudget().isEmpty() || s.Getpack().isEmpty() ||
            s.Getdate_deb().isEmpty() || s.Getdate_fin().isEmpty() || s.Gettelephone().isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez vérifier les données saisies.");
        return;
    }

        Connection c;
        if (c.createconnect())
        {
            if (c.insertDataS(s.Getnom(), s.Getbudget(), s.Getpack(), s.Getdate_deb(), s.Getdate_fin(), s.Gettelephone() ))
            {
                emit dataUpdated();
                updateTableWidget_S();
                QMessageBox::information(this, "Success", "Données insérées dans la base de données.");
            }
            else
            {
                QMessageBox::critical(this, "Erreur", "Échec de l'insertion des données dans la base de données.");
            }
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec de la connexion à la base de données.");
        }
        statistiquesS();
}


void MainWindow::on_pushButton_annulerS_clicked()
{

       // Update the UI or clear the input fields if needed
       ui->lineEdit_nomS->clear();
       ui->lineEdit_budget->clear();
       ui->dateEdit_SD->clear();
       ui->dateEdit_2->clear();
       ui->lineEdit_tel->clear();

}


void MainWindow::updateTableWidget_S()
{
    Connection c;
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SPONSOR");
    ui->tableWidget_S->clearContents();
    int RowNumber = 0;
    ui->tableWidget_S->setRowCount(0);

    while (model->canFetchMore())
        model->fetchMore();

    for (int row = 0; row < model->rowCount(); ++row)
    {
        ui->tableWidget_S->insertRow(row);
        for (int col = 0; col < 7; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget_S->setItem(row, col, item);
        }
        RowNumber++;
    }

    delete model;
}


void MainWindow::on_pushButton_supprimerS_clicked()
{
    QItemSelectionModel *select = ui->tableWidget_S->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
         int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->tableWidget_S->item(row, 0);
        int idToDelete = item->text().toInt();

        Sponsor s;
        if (s.supprimer(idToDelete))
        {
            // Supprimer la ligne de l'affichage
            ui->tableWidget_S->removeRow(row);
            emit dataUpdated(); // Mettre à jour la vue après la suppression
            QMessageBox::information(this, "Succes", "Données supprimées de la base de données.");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression des données de la base de données.");
        }
    } else {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner une ligne à supprimer.");
    }
    statistiquesS();
}

void MainWindow::on_pushButton_modifierS_clicked()
{
    QItemSelectionModel *select = ui->tableWidget_S->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidget_S->item(row, 0)->text().toInt();

        // Obtain the new values from the selected cells in the table
        QString nom = ui->tableWidget_S->item(row, 1)->text();
        QString budget = ui->tableWidget_S->item(row, 2)->text();
        QString date_deb = ui->tableWidget_S->item(row, 4)->text();
        QString date_fin = ui->tableWidget_S->item(row, 5)->text();
        QString telephone = ui->tableWidget_S->item(row, 6)->text();

        // Input validation and control
        if (nom.isEmpty() || budget.isEmpty() || date_deb.isEmpty() || date_fin.isEmpty() || telephone.isEmpty()) {
            QMessageBox::critical(this, "Erreur", "Les champs ne doivent pas être vides.");
            return;
        }

        // Validation des saisies
        // Validation du nom
        QRegExp rx("^[a-zA-Z _]+$");
        if (nom.length() > 20 || !rx.exactMatch(nom)) {
            QMessageBox::critical(this, "Erreur", "Le nom ne doit pas dépasser 20 caractères et doit être alphabétique.");
            return;
        }

        // Validation du budget
        bool budgetConversionOk;
        double budgetValue = budget.toDouble(&budgetConversionOk);
        if (!budgetConversionOk) {
            QMessageBox::critical(this, "Erreur", "Le budget doit être un nombre valide.");
            return;
        }

        // Validation de la date
        QDate startDate = QDate::fromString(date_deb, "dd/MM/yyyy");
        QDate endDate = QDate::fromString(date_fin, "dd/MM/yyyy");
        if (!startDate.isValid() || !endDate.isValid() || endDate < startDate) {
            QMessageBox::critical(this, "Erreur", "Plage de dates non valide. Date_fin ne devrait pas être avant date_deb.");
            return;
        }

        // Validation du téléphone
        if (telephone.length() != 8 || !telephone.toInt()) {
            QMessageBox::critical(this, "Erreur", "Le téléphone doit être composé de 8 chiffres.");
            return;
        }

        // Remove the combobox from the "Pack" field if it's currently in combobox mode
        QComboBox* packComboBox = qobject_cast<QComboBox*>(ui->tableWidget_S->cellWidget(row, 3));
        if (packComboBox) {
            QString selectedPack = packComboBox->currentText();
            ui->tableWidget_S->removeCellWidget(row, 3); // Remove the combobox
            ui->tableWidget_S->setItem(row, 3, new QTableWidgetItem(selectedPack)); // Set the selected pack as QTableWidgetItem
        }

        // Call the modifier function to update the database
        Sponsor s;
        if (s.modifier(idToModify, nom, budget, ui->tableWidget_S->item(row, 3)->text(), date_deb, date_fin, telephone))
        {
            // Update the row in the table with the new values
            ui->tableWidget_S->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidget_S->setItem(row, 2, new QTableWidgetItem(budget));
            ui->tableWidget_S->setItem(row, 4, new QTableWidgetItem(date_deb));
            ui->tableWidget_S->setItem(row, 5, new QTableWidgetItem(date_fin));
            ui->tableWidget_S->setItem(row, 6, new QTableWidgetItem(telephone));

            emit dataUpdated(); // Update the view after modification
            QMessageBox::information(this, "Succes", "Données modifiées dans la base de données.");
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec de la modification des données dans la base de données.");
        }
    } else {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner une ligne à modifier.");
    }
    statistiquesS();
}


void MainWindow::on_tableWidget_S_cellDoubleClicked(int row, int column)
{
    if (column == 3) { // Check if the double-clicked cell is in the "Pack" column
        QTableWidgetItem* item = ui->tableWidget_S->item(row, column);
        if (item) {
            QComboBox* packComboBox = new QComboBox();
            packComboBox->addItem("Bronze");
            packComboBox->addItem("Silver");
            packComboBox->addItem("Gold");
            QString currentPack = item->text();
            int currentPackIndex = packComboBox->findText(currentPack);
            packComboBox->setCurrentIndex(currentPackIndex);
            ui->tableWidget_S->setCellWidget(row, column, packComboBox); // Set the combobox as cell widget
        }
    }
}

void MainWindow::on_tableWidget_S_itemChanged(QTableWidgetItem *item)
{
    int column = item->column();
    if (column == 3) { // Check if the modified item is in the "Pack" column
        QComboBox* packComboBox = qobject_cast<QComboBox*>(ui->tableWidget_S->cellWidget(item->row(), column));
        if (packComboBox) {
            QString selectedPack = packComboBox->currentText();
            ui->tableWidget_S->removeCellWidget(item->row(), column); // Remove the combobox
            ui->tableWidget_S->setItem(item->row(), column, new QTableWidgetItem(selectedPack)); // Set the selected pack as QTableWidgetItem
        }
    }
}


void MainWindow::on_pushButton_reset_clicked()
{
    Sponsor s;
    s.afficher(ui->tableWidget_S);
}


void MainWindow::on_pushButton_rechercherS_clicked()
{
    QString nomSponsor = ui->lineEdit_rechercherS->text();

    if (!nomSponsor.isEmpty())
    {
        // Perform the search by sponsor name
        Sponsor s;
        updateTableWidget_S();
        s.rechercherS(nomSponsor, ui->tableWidget_S);

    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom de sponsor pour la recherche.");
    }
}



void MainWindow::on_pushButton_trierS_clicked()
{
    Sponsor s;
    if (s.trierParPack(ui->tableWidget_S)) {
        QMessageBox::information(this,"Success", "Tableau trie par packs.");
    } else {
        QMessageBox::critical(this,"Erreur", "Impossible de trier le tableau.");
    }
}

void MainWindow::on_pushButton_trierdateS_clicked()
{
    Sponsor s;
    if (s.trierParDateDeb(ui->tableWidget_S)) {
        QMessageBox::information(this,"Success", "Tableau trie par date debut.");
    } else {
        QMessageBox::critical(this,"Erreur", "Impossible de trier le tableau.");
    }

}

void MainWindow::on_PDF_clicked()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Sauvegarder en PDF", QString(), ".pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    if (!fileName.isEmpty())
    {
        QString strStream;
        QTextStream out(&strStream);

        const int rowCount = ui->tableWidget_S->rowCount();
        const int columnCount = ui->tableWidget_S->columnCount();

        out << "<html>\n"
               "<head>\n"
               "<meta Content=\"Text/html; charset=Windows-1251\">\n"
               << QString("<title>%1</title>\n").arg("Liste des sponsors")
               << "<style>"
               "   body { font-family: 'Space Grotesk'; background-color: #ffffff; }"
               "   .table-container { margin: auto; }"
               "   table { border-collapse: collapse; width: 80%; }"
               "   th, td { border: 1px solid #3498db; text-align: left; padding: 8px; }"
               "   tr:nth-child(even) { background-color: #f2f2f2; }"
               "   th { background-color: #3498db; color: white; }"
               "</style>\n"
               "</head>\n"
               "<body>\n";

        // Add logo and application name
        out << "<div style=\"text-align: center;\"><img src=\":/new/prefix3/tsawer/logo 3.png\" alt=\"Logo\"></div>\n"
               "<h1 style=\"color: #3498db; text-align: center;\">Liste Des Sponsors</h1>\n";

        // Center the table
        out << "<div class=\"table-container\">\n";

        // Add table
        out << "<table>\n"
               "<thead><tr>";
        for (int column = 0; column < columnCount; column++)
            if (!ui->tableWidget_S->isColumnHidden(column))
                out << QString("<th>%1</th>").arg(ui->tableWidget_S->horizontalHeaderItem(column)->text());
        out << "</tr></thead>\n";

        // Add data rows
        for (int row = 0; row < rowCount; row++)
        {
            out << "<tr>";
            for (int column = 0; column < columnCount; column++)
            {
                if (!ui->tableWidget_S->isColumnHidden(column))
                {
                    QTableWidgetItem *item = ui->tableWidget_S->item(row, column);
                    QString data = (item) ? item->text().simplified() : QString("&nbsp;");
                    out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
            }
            out << "</tr>\n";
        }
        out << "</table>\n";

        // Close the table container div
        out << "</div>\n";

        // Add copyright and team information
        out << "<br><br><div style=\"text-align: center;\">"
               "2024 Insightify. Tous droits reserves.<br>"
               "Developpe par l'equipe Insightify</div>\n";

        out << "</body>\n"
               "</html>\n";

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(fileName);

        QTextDocument doc;
        doc.setHtml(strStream);
        doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);

        showCustomMessageBox("Succès", "Tableau exporté en PDF.", QMessageBox::Information);
    }
    else
    {
        showCustomMessageBox("Erreur", "Échec de l'exportation du tableau en PDF.", QMessageBox::Critical);
    }
}


void MainWindow::on_SMS_clicked()
{
    Sponsor s;
    //qInfo()<< QSslSocket::sslLibraryBuildVersionString();
    QString nomSponsor = ui->smsEdit->text();
    if (s.sponsorExists(nomSponsor)) {
            // Sponsor exists, proceed to construct and send SMS
            QString message = s.constructSMS(nomSponsor);
            QString phoneNumber = "+21656623537"; // Assuming this remains constant
            s.sendSMS(phoneNumber, message);
        } else {
            // Sponsor doesn't exist, show error message
            QMessageBox::critical(this, "Erreur", "Sponsor n'existe pas dans la base de données.");
        }
}

void MainWindow::on_pushButton_annulerS2_clicked()
{
    ui->smsEdit->clear();
}


void MainWindow::statistiquesS()
{

    Connection C;
    C.createconnect();
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SPONSOR WHERE PACK='Bronze' ");
    float exp1=model->rowCount();
    model->setQuery("SELECT * FROM SPONSOR WHERE PACK='Silver' ");
    float exp2=model->rowCount();
    model->setQuery("SELECT * FROM SPONSOR WHERE PACK='Gold' ");
    float exp3=model->rowCount();
    float total=exp1+exp2+exp3;
    QString a=QString("bronze "+ QString::number((exp1*100)/total,'f',2)+"%");
    QString b=QString("silver "+ QString::number((exp2*100)/total,'f',2)+"%");
    QString c=QString("gold "+ QString::number((exp3*100)/total,'f',2)+"%");
    QPieSeries *series = new QPieSeries();
        series->setHoleSize(0.3);
        series->append(a, exp1);
        series->append(b, exp2);
        series->append(c, exp3);
        if (exp1!=0)
        {
           QPieSlice *slice1 = series->slices().at(0);
           //slice1->setExploded();
           slice1->setLabelVisible();
        }
        if (exp2!=0)
        {
            QPieSlice *slice2 = series->slices().at(1);
            slice2->setLabelVisible();
        }
        if (exp3!=0)
        {
            QPieSlice *slice3 = series->slices().at(2);
            slice3->setExploded();
            slice3->setLabelVisible();
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->setTitle("Repartition des sponsor selon les packs");
        chart->setTheme(QChart::ChartThemeBlueCerulean);
        QChartView *chartview = new QChartView(chart);
        chartview->setGeometry(10, 10, 500, 380);
        chartview->setRenderHint(QPainter::Antialiasing);
        chartview->setParent(ui->statS);

}

void MainWindow::on_exportstat_clicked()
{
    // Find the child QChartView within ui->statS
    QChartView *chartView = ui->statS->findChild<QChartView *>();

    if (!chartView)
    {
        showCustomMessageBox("Erreur", "Aucun graphique trouvé.", QMessageBox::Critical);
        return;
    }

    // Render the chart view as an image and save it as a PNG file
    QImage image(chartView->width(), chartView->height(), QImage::Format_ARGB32);
    QPainter imagePainter(&image);
    chartView->render(&imagePainter);
    QString filePath = QFileDialog::getSaveFileName(this, "Sauvegarder en PNG", "", "Fichiers PNG (*.png)");

    if (!filePath.isEmpty())
    {
        if (image.save(filePath))
        {
            showCustomMessageBox("Succès", "Graphique exporté en PNG.", QMessageBox::Information);
        }
        else
        {
            showCustomMessageBox("Erreur", "Échec de l'exportation du graphique en PNG.", QMessageBox::Critical);
        }
    }
}

//**********************************dashbaord*******************************************************//

void MainWindow::onAddToDo() {
    // Get the text for the new to-do item from the user or generate it programmatically
    QString newItemText = " ";

    // Call the addToDoItem method of CToDoList to add the new to-do item
    toDoListWidget->onAdd(newItemText);
}

void MainWindow::onRemoveToDo() {
    // Get the text for the new to-do item from the user or generate it programmatically
    QString newItemText = " ";

    // Call the addToDoItem method of CToDoList to add the new to-do item
    toDoListWidget->onRemove(newItemText);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveToDoListToFile("C:/Users/amine/Desktop/projet_integration-20240419T075358Z-001/projet_integration/projet/todo_list.txt"); //
    event->accept();
}

void MainWindow::saveToDoListToFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    // Save Pending items
    out << "Pending Items:\n";
    QStringListModel* pendingModel = qobject_cast<QStringListModel*>(toDoListWidget->m_pwPending->model());
    if (pendingModel) {
        QStringList pendingItems = pendingModel->stringList();
        for (const QString& item : pendingItems)
            out << item << "\n";
    }

    // Save Completed items
    out << "\nCompleted Items:\n";
    QStringListModel* completedModel = qobject_cast<QStringListModel*>(toDoListWidget->m_pwCompleted->model());
    if (completedModel) {
        QStringList completedItems = completedModel->stringList();
        for (const QString& item : completedItems)
            out << item << "\n";
    }

    file.close();
}

// Implement the loadToDoListFromFile function
void MainWindow::loadToDoListFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    QStringList pendingItems;
    QStringList completedItems;
    QString line;
    enum { Pending, Completed, None } section = None;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();

        if (line == "Pending Items:") {
            section = Pending;
            continue;
        }
        else if (line == "Completed Items:") {
            section = Completed;
            continue;
        }

        switch (section) {
        case Pending:
            if (!line.isEmpty())
                pendingItems.append(line);
            break;
        case Completed:
            if (!line.isEmpty())
                completedItems.append(line);
            break;
        default:
            break;
        }
    }

    file.close();

    // Populate Pending list view
    QStringListModel* pendingModel = qobject_cast<QStringListModel*>(toDoListWidget->m_pwPending->model());
    if (pendingModel) {
        pendingModel->setStringList(pendingItems);
    }

    // Populate Completed list view
    QStringListModel* completedModel = qobject_cast<QStringListModel*>(toDoListWidget->m_pwCompleted->model());
    if (completedModel) {
        completedModel->setStringList(completedItems);
    }
}


void MainWindow::onTabChanged(int index)
{
    // Check if the index corresponds to the tab containing the to-do list widget

    if (index == ui->dashboard->indexOf(toDoListWidget)) {
        mainToolBar->setVisible(true); // Show the toolbar
    } else {
        mainToolBar->setVisible(false); // Hide the toolbar
    }
}



//---------------------------------------------------------------------MATERIAUX Zoh------------------------------------------------------------------------------//

//-----------------------------Fonction pour mettre le tableau a jour apres les operations------------------------//
void MainWindow::updateTableWidgetM()
{
    Connection c;
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM MATERIAUX");

        // Efface le contenu actuel du tableau
        ui->tableWidgetM->clearContents();
        ui->tableWidgetM->setRowCount(0); // Réinitialise les lignes du tableau

        // Met à jour les données triées dans le tableau
        for (int row = 0; row < model->rowCount(); ++row)
        {
            // Insertion d'une nouvelle ligne dans le tableau
            ui->tableWidgetM->insertRow(row);

            // Ajout des données dans les cellules de la ligne
            for (int col = 0; col < 6; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
                ui->tableWidgetM->setItem(row, col, item);
            }
        }

        // Libère la mémoire occupée par le modèle
        delete model;
}
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//



//-----------------------------------FONCTION D'AJOUT--------------------------------------------------------------//
void MainWindow::on_pushButton_AjoutMateriaux_clicked()
{
    // Récupérer les valeurs saisies
    QString nom = ui->lineEdit_nomM->text();
    QString type = ui->lineEdit_typeM->text();
    QString etat = ui->lineEdit_etatM->currentText();
    QString quantite = ui->lineEdit_quantiteM->text();
    QString date = ui->lineEdit_dateM->text();
    int idTransaction = ui->lineEdit_idTransactionM->text().toInt();
    // Convertir la chaîne de caractères en entier

    // Vérifier si tous les champs obligatoires sont remplis
    if (nom.isEmpty() || type.isEmpty() || etat.isEmpty() || quantite.isEmpty() || date.isEmpty() || idTransaction <= 0) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Validation du champ "quantite" (chiffres uniquement)
    QRegularExpression regex("[0-9]+");
    if (!regex.match(quantite).hasMatch()) {
        QMessageBox::warning(this, "Quantité invalide", "La quantité doit contenir uniquement des chiffres.");
        return;
    }

    // Si tous les champs sont valides, créer un matériel et l'ajouter à la base de données
    Materiaux i;
    i.set_nom(nom);
    i.set_type(type);
    i.set_etat(etat);
    i.set_quantite(quantite);
    i.set_date(date);
    i.set_idTransaction(idTransaction);

    Connection c;
    if (c.createconnect()) {
        if (i.ajouterMateriaux()) {
            updateTableWidgetM(); // Émettre le signal après l'insertion réussie
            AfficherEtmettreAJourPieChartMateriaux();
            QMessageBox::information(this, "Succès", "Données insérées dans la base de données.");
            //showMaterialStats();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'insertion des données dans la base de données.");
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la connexion à la base de données.");
    }
}

//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//



//-------------------------------FONCTION DE MODIFICATION--------------------------------------------------------//
void MainWindow::on_pushButton_ModificationMateriaux_clicked()
{
    QItemSelectionModel *select = ui->tableWidgetM->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidgetM->item(row, 0)->text().toInt(); // Récupérer l'ID du materiel après le tri

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidgetM->item(row, 1)->text();
        QString type = ui->tableWidgetM->item(row, 2)->text();
        QString etat = ui->tableWidgetM->item(row, 3)->text();
        QString quantite = ui->tableWidgetM->item(row, 4)->text();
        QString date = ui->tableWidgetM->item(row, 5)->text();
        QString idTransaction = ui->tableWidgetM->item(row, 6)->text();

        // Appelez la fonction modifierMateriaux pour mettre à jour la base de données
        Materiaux i;
        if (i.modifierMateriaux(idToModify, nom, type, etat, quantite, date, idTransaction.toInt()))
        {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidgetM->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidgetM->setItem(row, 2, new QTableWidgetItem(type));
            ui->tableWidgetM->setItem(row, 3, new QTableWidgetItem(etat));
            ui->tableWidgetM->setItem(row, 4, new QTableWidgetItem(quantite));
            ui->tableWidgetM->setItem(row, 5, new QTableWidgetItem(date));
            ui->tableWidgetM->setItem(row, 6, new QTableWidgetItem(idTransaction));

            emit dataUpdated(); // Mettre à jour la vue après la modification
            AfficherEtmettreAJourPieChartMateriaux();
            ui->label_10->setText("Données modifiées avec succés.");
        } else {
            ui->label_10->setText("Echec de la modification !");
        }
    } else {
        ui->label_10->setText("Veuillez selectionner une case à modifier !");
    }
    for (int r = 0; r < ui->tableWidgetM->rowCount(); ++r)
    {
        for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c)
        {
            QWidget *cellWidget = ui->tableWidgetM->cellWidget(r, c);
            if (cellWidget) {
                cellWidget->deleteLater();
            }
        }
    }
    updateTableWidgetM();
}

//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//


//----------------------------------------FONCTION DE SUPPRESSION----------------------------------------------//
void MainWindow::on_pushButton_SuppressionMateriaux_clicked()
{

    QItemSelectionModel *select = ui->tableWidgetM->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty())
    {
        int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->tableWidgetM->item(row, 0); // Vérifiez la colonne contenant l'ID après le tri
        if (item) // Vérifiez si l'élément existe
        {
            int idToDelete = item->text().toInt();

            // Vérifiez d'abord s'il existe des enregistrements correspondants dans la table CONTENIR
            QSqlQuery queryCheck;
            queryCheck.prepare("SELECT * FROM CONTENIR WHERE ID_MATERIEL = :idMateriel");
            queryCheck.bindValue(":idMateriel", idToDelete);
            if (queryCheck.exec() && queryCheck.next())
            {
                // S'il existe des enregistrements dans CONTENIR, informez l'utilisateur et demandez confirmation pour la suppression
                QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, "Supprimer le matériel", "Il existe des enregistrements dans la table CONTENIR pour ce matériel. Voulez-vous quand même le supprimer ?", QMessageBox::Yes | QMessageBox::No);
                if (confirmDelete == QMessageBox::Yes)
                {
                    // Supprimez d'abord les enregistrements correspondants dans la table CONTENIR
                    QSqlQuery deleteQuery;
                    deleteQuery.prepare("DELETE FROM CONTENIR WHERE ID_MATERIEL = :idMateriel");
                    deleteQuery.bindValue(":idMateriel", idToDelete);
                    if (deleteQuery.exec())
                    {
                        qDebug() << "Enregistrements dans CONTENIR supprimés avec succès !";

                        // Après avoir supprimé les enregistrements dans CONTENIR, vous pouvez maintenant supprimer la ligne dans MATERIAUX
                        Materiaux i;
                        if (i.supprimerMateriaux(idToDelete))
                        {
                            // Supprimer les widgets de cellule associés à cette ligne
                            for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c) {
                                QWidget *cellWidget = ui->tableWidgetM->cellWidget(row, c);
                                AfficherEtmettreAJourPieChartMateriaux();
                                if (cellWidget) {
                                    cellWidget->deleteLater();
                                }
                            }

                            // Supprimer la ligne de l'affichage
                            ui->tableWidgetM->removeRow(row);
                            emit dataUpdated(); // Mettre à jour la vue après la suppression
                            ui->label_10->setText("Donnée supprimée avec succès.");

                        }
                        else
                        {
                            ui->label_10->setText("Echec de la suppression !");
                        }
                    }
                    else
                    {
                        qDebug() << "Erreur lors de la suppression des enregistrements dans CONTENIR :" << deleteQuery.lastError().text();
                    }
                }
            }
            else
            {
                // S'il n'y a pas d'enregistrements dans CONTENIR, vous pouvez simplement supprimer la ligne dans MATERIAUX
                Materiaux i;
                if (i.supprimerMateriaux(idToDelete))
                {
                    // Supprimer les widgets de cellule associés à cette ligne
                    for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c) {
                        QWidget *cellWidget = ui->tableWidgetM->cellWidget(row, c);
                        if (cellWidget) {
                            cellWidget->deleteLater();
                        }
                    }

                    // Supprimer la ligne de l'affichage
                    ui->tableWidgetM->removeRow(row);
                    emit dataUpdated(); // Mettre à jour la vue après la suppression
                    ui->label_10->setText("Donnée supprimée avec succès.");
                }
                else
                {
                    ui->label_10->setText("Echec de la suppression !");
                }
            }
        }
        else
        {
            ui->label_10->setText("Selected row does not contain ID.");
        }
    }
    else
    {
        ui->label_10->setText("Veuillez sélectionner une ligne à supprimer !");
    }
}


//-////////////////////////////////////////////////////////////////////////////////////////////////////////////-//


//--------------------------------------FONCTION DE TRI PAR DATE-----------------------------------------------//
void MainWindow::on_Button_trierParDateMateriaux_clicked()
{
        ui->tableWidgetM->clearContents(); // Efface le contenu actuel du tableau
        ui->tableWidgetM->setRowCount(0); // Efface également les lignes existantes du tableau
        // Trier les materiaux par date
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("Source_Projet2A4");
        db.setUserName("louayzorai");
        db.setPassword("louayzorai");
        db.open();
        QSqlQuery query(db);
        QString str = ("SELECT * FROM MATERIAUX");
        if (query.exec(str))
        {
            ui->tableWidgetM->setColumnCount(7);
            QStringList labels;
            labels << "ID" << "Nom" << "Type" << "Etat" << "Quantite" << "Date" <<"idTransaction";
            ui->tableWidgetM->setHorizontalHeaderLabels(labels);
            int row = 0;
            QVector<QPair<QDate, QStringList>> sortedMateriaux; // Pour stocker les materiaux triées

            while (query.next())
            {
                QStringList materiauxData;
                materiauxData << query.value(0).toString() << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString() << query.value(6).toString();
                QDate date = QDate::fromString(query.value(5).toString(), "dd/MM/yyyy"); // Convertir la chaîne de date en QDate
                sortedMateriaux.append(qMakePair(date, materiauxData));
            }

            // Tri des materiaux par date
            std::sort(sortedMateriaux.begin(), sortedMateriaux.end(), [](const QPair<QDate, QStringList> &a, const QPair<QDate, QStringList> &b) {
                return a.first < b.first;
            });

            // Affichage des materiaux triées dans le tableau
            for (const auto &materiaux : sortedMateriaux)
            {
                ui->tableWidgetM->insertRow(row);
                for (int col = 0; col < 7; ++col)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(materiaux.second[col]);
                    ui->tableWidgetM->setItem(row, col, item);
                }
                row++;
            }
        }
}
//----------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------FONCTION D'EXPORTATION EXCEL-----------------------------------------------------------------------------//
void MainWindow::on_pushButton_exportationFormatExcelMateriel_clicked()
{
    // Créer un objet Workbook
        QXlsx::Document xlsx;

        // Obtenir le nombre de lignes et de colonnes du QTableWidget
        int rowCount = ui->tableWidgetM->rowCount();
        int columnCount = ui->tableWidgetM->columnCount();

        // Ajouter le titre "TABLE DES MATERIAUX" sur 4 lignes et 8 colonnes
        QColor couleurCellule(0, 32, 96);
        xlsx.mergeCells(QXlsx::CellRange(1, 1, 4, 24));
        QXlsx::Format titleFormat; // Déclaration de la variable titleFormat
        titleFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        titleFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
        titleFormat.setFontBold(true);
        titleFormat.setFontSize(36);
        titleFormat.setFontColor(Qt::white);
        titleFormat.setPatternBackgroundColor(couleurCellule);
        xlsx.write(1, 1, "TABLE DES TRANSACTIONS", titleFormat);

        // Sauter 2 lignes après le titre
        xlsx.write(6, 1, ""); // Écrire une cellule vide à la ligne 6

        // Ajouter les noms de colonnes
        QXlsx::Format nomDeColonneFormat; // Déclaration de la variable titleFormat
        nomDeColonneFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        nomDeColonneFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
        nomDeColonneFormat.setFontBold(true);
        nomDeColonneFormat.setFontSize(24);
        nomDeColonneFormat.setFontColor(Qt::white);
        QColor couleurCelluleNomColonne(221, 171, 213);
        nomDeColonneFormat.setPatternBackgroundColor(couleurCelluleNomColonne);

        for (int column = 0; column < columnCount; ++column) {
            QTableWidgetItem *headerItem = ui->tableWidgetM->horizontalHeaderItem(column);
            if (headerItem) {
                xlsx.write(8, column * 4 + 1, headerItem->text(),nomDeColonneFormat);
                xlsx.mergeCells(QXlsx::CellRange(8, column * 4 + 1, 11, column * 4 + 4)); // Étaler sur 4 colonnes et 4 lignes

                // Appliquer le format pour les noms de colonnes
                QXlsx::Format columnNameFormat;
                columnNameFormat.setFontBold(true);
                columnNameFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
                columnNameFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
                columnNameFormat.setFontSize(24);
                xlsx.cellAt(8, column * 4 + 1)->format().setFont(columnNameFormat.font());
                xlsx.cellAt(8, column * 4 + 1)->format().setHorizontalAlignment(columnNameFormat.horizontalAlignment());
                xlsx.cellAt(8, column * 4 + 1)->format().setVerticalAlignment(columnNameFormat.verticalAlignment());
                xlsx.cellAt(8, column * 4 + 1)->format().setFontSize(columnNameFormat.fontSize());
            }
        }

        // Parcourir le QTableWidget pour copier les données dans le document Excel
        QXlsx::Format formatCelluleNormale; // Déclaration de la variable titleFormat
        formatCelluleNormale.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        formatCelluleNormale.setVerticalAlignment(QXlsx::Format::AlignVCenter);
        formatCelluleNormale.setFontSize(16);
        formatCelluleNormale.setFontColor(Qt::white);
        formatCelluleNormale.setPatternBackgroundColor(couleurCellule);
        for (int row = 0; row < rowCount; ++row) {
            for (int column = 0; column < columnCount; ++column) {
                // Obtenir le texte de la cellule dans le QTableWidget
                QTableWidgetItem *item = ui->tableWidgetM->item(row, column);
                if (item) {
                    // Insérer le texte de la cellule dans le document Excel
                    xlsx.write(row + 12, column * 4 + 1, item->text(),formatCelluleNormale);
                    xlsx.mergeCells(QXlsx::CellRange(row + 12, column * 4 + 1, row + 13, column * 4 + 4)); // Étaler sur 4 colonnes et 2 lignes
                }
            }
        }
        // Chemin vers votre image
        QString imagePath = "/tsawer/logo pour excel.png";

        // Charger l'image à partir du chemin
        QImage image(imagePath);

        // Insérer l'image dans le fichier Excel
        xlsx.insertImage(0, 0, image);
        // Sauvegarder le document Excel
        QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter en Excel"), "", tr("Fichiers Excel (*.xlsx)"));
        if (!filePath.isEmpty()) {
            xlsx.saveAs(filePath);
        }
}
//--------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------FONCTION DE RECHERCHE PAR TYPE---------------------------------------------------------//
void MainWindow::searchMateriauxByType(const QString &type)
{
    // Nettoyer et convertir en minuscules la chaîne de recherche
    QString filterType = type.trimmed().toLower();

    // Parcourir toutes les lignes du tableau
    for (int row = 0; row < ui->tableWidgetM->rowCount(); ++row) {
        // Récupérer le type du matériau dans la troisième colonne (indice 2)
        QString currentType = ui->tableWidgetM->item(row, 2)->text().trimmed().toLower();

        // Vérifier si le type actuel contient la chaîne de recherche
        bool showRow = currentType.contains(filterType, Qt::CaseInsensitive);

        // Afficher ou masquer la ligne en fonction de la correspondance du type
        ui->tableWidgetM->setRowHidden(row, !showRow);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------FONCTION BOUTON ANNULER--------------------------------------------------------------------//
void MainWindow::on_pushButton_AnnulerMateriaux_clicked()
{
    ui->lineEdit_nomM->clear();
    ui->lineEdit_typeM->clear();
    ui->lineEdit_etatM->clear();
    ui->lineEdit_quantiteM->clear();
    ui->lineEdit_dateM->clear();
}
//----------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------LES FONCTIONS DE CONTROLE DE SAISIE + GERER LES EVENEMENTS-------------------------------------------------------//
bool MainWindow::eventFilterM(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEdit_quantiteM && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease))
    {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            QString text = ui->lineEdit_quantiteM->text();

            // Seuls les chiffres, un point et la touche "Backspace" sont autorisés
            if (!(keyEvent->text().isEmpty() && keyEvent->key() != Qt::Key_Backspace) &&
                !(keyEvent->text().at(0).isDigit() || keyEvent->text() == "." || keyEvent->key() == Qt::Key_Backspace)) {
                ui->label_5->setText("La quantite doit contenir uniquement\ndes chiffres et un point !");
                return true;
            }

            // On autorise un seul point décimal
            if (keyEvent->text() == "." && text.contains(".")) {
                ui->label_5->setText("Vous ne pouvez entrer qu'un\nseul point décimal !");
                return true;
            }

            // Si la saisie est valide, effacez le contenu du QLabel
            if (text.isEmpty() || (text.toDouble() != 0.0 && text.toDouble() != 0.0)) {
                ui->label_5->clear(); // Efface le contenu du QLabel
            }
        }
    if (event->type() == QEvent::MouseButtonPress) {
            // Si l'événement est un clic de souris, effacez le contenu du champ label_10
            ui->label_10->clear();
        }
        return QObject::eventFilter(obj, event);
}

void MainWindow::preventCellEdit()
{
    ui->tableWidgetM->setEditTriggers(QAbstractItemView::NoEditTriggers);
}



void MainWindow::showEditChoices(int row, int column)
{
    if (column == 3) {
            QStringList choices = {"bonne etat", "en reparation", "en panne"};
            showChoicesUnderCell(row, column, choices);
        }
}



void MainWindow::showChoicesUnderCell(int row, int column, const QStringList &choices) {
    // Sauvegarder la modification actuelle dans le tableau avant de supprimer les combobox précédents
    QWidget *previousWidget = ui->tableWidgetM->cellWidget(row, column);
    if (previousWidget) {
        QComboBox *previousComboBox = previousWidget->findChild<QComboBox*>();
        if (previousComboBox) {
            QString newText = previousComboBox->currentText();
            ui->tableWidgetM->item(row, column)->setText(newText);
        } else {
            QLineEdit *previousLineEdit = previousWidget->findChild<QLineEdit*>();
            if (previousLineEdit) {
                QString newText = previousLineEdit->text();
                ui->tableWidgetM->item(row, column)->setText(newText);
            }
        }
    }

    // Supprimer tous les QComboBox et les lignes de saisie de date présents dans le tableau
    for (int r = 0; r < ui->tableWidgetM->rowCount(); ++r)
    {
        for (int c = 0; c < ui->tableWidgetM->columnCount(); ++c)
        {
            QWidget *cellWidget = ui->tableWidgetM->cellWidget(r, c);
            if (cellWidget) {
                cellWidget->deleteLater();
            }
        }
    }

    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    if (column == 5) { // Si c'est la colonne de la date
        QDateEdit *dateEdit = new QDateEdit();
        dateEdit->setDisplayFormat("dd/MM/yyyy"); // Format de la date
        layout->addWidget(dateEdit);
        layout->setAlignment(Qt::AlignTop);

        ui->tableWidgetM->setCellWidget(row, column, widget);

        connect(dateEdit, &QDateEdit::dateChanged, [=](const QDate &date) {
            // Mettre à jour la valeur de la cellule dans le QTableWidget avec la valeur de la date formatée
            QString formattedDate = date.toString("dd/MM/yyyy");
            ui->tableWidgetM->item(row, column)->setText(formattedDate);
        });
    } else { // Pour toutes les autres colonnes
        QComboBox *newComboBox = new QComboBox();
        newComboBox->addItems(choices);
        newComboBox->setCurrentIndex(-1); // Définir la sélection initiale sur -1

        layout->addWidget(newComboBox);
        layout->setAlignment(Qt::AlignTop);

        ui->tableWidgetM->setCellWidget(row, column, widget);

        //connect(newComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        connect(newComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=]() {
            // Mettre à jour la valeur de la cellule dans le QTableWidget avec la valeur sélectionnée dans le QComboBox
            QString newText = newComboBox->currentText();
            ui->tableWidgetM->item(row, column)->setText(newText);
        });
    }
}


void MainWindow::handleCellClicked(int row, int column) {
    if (column == 1 || column == 2 || column == 4) { // Colonnes nom, type et quantité
        ui->tableWidgetM->editItem(ui->tableWidgetM->item(row, column)); // Activer l'édition de la cellule
    } else if (column == 3) { // Colonnes état
        showEditChoices(row, column); // Afficher les choix du combo box
    } else if (column == 5) { // Colonne date
        QDateEdit *dateEdit = new QDateEdit(); // Créer un QDateEdit pour éditer la date
        dateEdit->setDate(QDate::fromString(ui->tableWidgetM->item(row, column)->text(), "dd-MM-yyyy")); // Récupérer et afficher la date actuelle dans le QDateEdit
        ui->tableWidgetM->setCellWidget(row, column, dateEdit); // Afficher le QDateEdit dans la cellule

        connect(dateEdit, &QDateEdit::dateChanged, this, [=](const QDate &date) {
            ui->tableWidgetM->item(row, column)->setText(date.toString("dd-MM-yyyy")); // Mettre à jour le texte de la cellule avec la nouvelle date
            ui->tableWidgetM->removeCellWidget(row, column); // Supprimer le QDateEdit après l'édition
        });
    }
}
void MainWindow::editnomCell(int row, int column)
{
    if (column == 0) { // Vérifier si la colonne est la colonne "nom"
        QLineEdit *lineEdit = new QLineEdit(ui->tableWidgetM);
        QString text = ui->tableWidgetM->item(row, column)->text();
        lineEdit->setText(text);
        ui->tableWidgetM->setCellWidget(row, column, lineEdit);
        lineEdit->setFocus();
        connect(lineEdit, &QLineEdit::textChanged, [=](const QString &newText) {
            QRegExp regExp("^\\d*\\.?\\d*$"); // Expression régulière pour vérifier la validité du nom
            if (!regExp.exactMatch(newText)) {
                // Si la saisie est invalide, effacer le contenu du line edit
                lineEdit->setText(text);
            }
        });
        connect(lineEdit, &QLineEdit::editingFinished, [=]() {
            // Mettre à jour la cellule du tableau avec le nouveau montant
            QString newText = lineEdit->text();
            QTableWidgetItem *item = new QTableWidgetItem(newText);
            ui->tableWidgetM->setItem(row, column, item);
            // Supprimer le line edit après l'édition
            ui->tableWidgetM->removeCellWidget(row, column);
        });
    }
}



//---------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------STATISTIQUE----------------------------------------------------------------------------//

void MainWindow::AfficherEtmettreAJourPieChartMateriaux()
{
    Connection c;
    c.createconnect();
    float percentBonneEtat = 0.0;
    float percentEnReparation = 0.0;
    float percentEnPanne = 0.0;
    QSqlQuery countQuery;
    int totalCount = 0;
    if (countQuery.exec("SELECT COUNT(*) FROM MATERIAUX"))
    {
        if (countQuery.next())
        {
            totalCount = countQuery.value(0).toInt();
        }
    }
    if (totalCount == 0)
    {
        QPieSeries *emptySeries = new QPieSeries();
        emptySeries->append("Aucune donnée", 0);
        QChart *chart = new QChart();
        chart->addSeries(emptySeries);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->setTitle("Statistiques de l'état des matériaux");
        QFont titleFont("Arial", 16); // Définir la police pour le titre
        chart->setTitleFont(titleFont);
        chart->setTheme(QChart::ChartThemeBlueCerulean);
        QChartView *chartview = new QChartView(chart);
        chartview->setRenderHint(QPainter::Antialiasing);
        chartview->setGeometry(chartview->x(), chartview->y(), 1000, 400); // Ajustez la largeur et la hauteur selon vos besoins
        return;
    }
    QSqlQuery query;
    if (query.exec("SELECT COUNT(*) FROM MATERIAUX WHERE ETAT = 'bonne etat'"))
    {
        query.next();
        int totalBonneEtat = query.value(0).toInt();
        percentBonneEtat = (totalBonneEtat / static_cast<float>(totalCount)) * 100.0;
    }
    if (query.exec("SELECT COUNT(*) FROM MATERIAUX WHERE ETAT = 'en reparation'"))
    {
        query.next();
        int totalEnReparation = query.value(0).toInt();
        percentEnReparation = (totalEnReparation / static_cast<float>(totalCount)) * 100.0;
    }
    if (query.exec("SELECT COUNT(*) FROM MATERIAUX WHERE ETAT = 'en panne'"))
    {
        query.next();
        int totalEnPanne = query.value(0).toInt();
        percentEnPanne = (totalEnPanne / static_cast<float>(totalCount)) * 100.0;
    }
    QString labelBonneEtat = QString("Bonne état %1%").arg(QString::number(percentBonneEtat, 'f', 2));
    QString labelEnReparation = QString("En réparation %1%").arg(QString::number(percentEnReparation, 'f', 2));
    QString labelEnPanne = QString("En panne %1%").arg(QString::number(percentEnPanne, 'f', 2));
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.3);
    series->setLabelsVisible(true); // Permettre l'affichage des étiquettes

    if (percentBonneEtat != 0.0)
        series->append(labelBonneEtat, percentBonneEtat);
    if (percentEnReparation != 0.0)
        series->append(labelEnReparation, percentEnReparation);
    if (percentEnPanne != 0.0)
        series->append(labelEnPanne, percentEnPanne);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Statistiques de l'état des matériaux");
    QFont titleFont("Arial", 16); // Définir la police pour le titre
    chart->setTitleFont(titleFont);
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->legend()->setVisible(false); // Masquer la légende
    chart->legend()->setAlignment(Qt::AlignRight); // Aligner la légende à droite
    QFont legendFont("Arial", 10); // Définir la police pour la légende
    chart->legend()->setFont(legendFont);

    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setParent(ui->statMateriaux);
    chartview->setGeometry(chartview->x(), chartview->y(), 1000, 400);
}


//---------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------Materiaux/Podcast------------------------------------------------------------------------------//
void MainWindow::selectMaterialAndInsertIntoContenir() {
    // Accéder à l'onglet "Materiaux_Selectionnes"
    ui->tabWidget_12->setCurrentIndex(2); // Supposons que l'onglet "Materiaux_Selectionnes" soit le troisième onglet, donc son index est 2

    // Effacer les éléments existants dans les listes
    ui->listWidget_podcasts->clear();
    ui->listWidget_materiaux->clear();

    // Récupérer les ID des matériaux associés à des podcasts
    QSqlQuery associatedMaterialsQuery("SELECT DISTINCT ID_MATERIEL FROM CONTENIR");
    QList<int> associatedMaterialIds;
    while (associatedMaterialsQuery.next()) {
        associatedMaterialIds << associatedMaterialsQuery.value(0).toInt();
    }

    // Afficher une liste de podcasts disponibles dans le QListWidget listWidget_podcasts
    QSqlQuery queryPodcasts("SELECT ID_PODCAST, NOM FROM PODCASTS");
    while (queryPodcasts.next()) {
        QString podcastName = queryPodcasts.value(1).toString();
        QIcon podcastIcon(":/icons/podcast_icon.png"); // Remplacez le chemin d'accès par celui de votre propre icône
        QListWidgetItem *item = new QListWidgetItem(podcastIcon, podcastName);
        ui->listWidget_podcasts->addItem(item); // Ajouter l'élément à la liste
    }

    // Afficher une liste de matériaux disponibles avec leur type dans le QListWidget listWidget_materiaux
    QSqlQuery queryMaterials("SELECT ID_MATERIEL, NOM, TYPE FROM MATERIAUX");
    while (queryMaterials.next()) {
        int materialId = queryMaterials.value(0).toInt();
        QString materialNameWithType = queryMaterials.value(1).toString() + " - " + queryMaterials.value(2).toString();

        // Vérifier si le matériau est déjà associé à un podcast
        if (!associatedMaterialIds.contains(materialId)) {
            QIcon materialIcon(":/icons/material_icon.png"); // Remplacez le chemin d'accès par celui de votre propre icône
            QListWidgetItem *item = new QListWidgetItem(materialIcon, materialNameWithType);
            ui->listWidget_materiaux->addItem(item); // Ajouter l'élément à la liste
        }
    }

    // Mettre en surbrillance l'élément sélectionné dans la liste des podcasts
    connect(ui->listWidget_podcasts, &QListWidget::itemClicked, [this](QListWidgetItem *item){
        ui->listWidget_podcasts->setCurrentItem(item);
        item->setBackgroundColor(QColor("#e0e0e0")); // Changer la couleur de fond de l'élément sélectionné
    });

    // Mettre en surbrillance l'élément sélectionné dans la liste des matériaux
    connect(ui->listWidget_materiaux, &QListWidget::itemClicked, [this](QListWidgetItem *item){
        ui->listWidget_materiaux->setCurrentItem(item);
        item->setBackgroundColor(QColor("#e0e0e0")); // Changer la couleur de fond de l'élément sélectionné
    });

    // Connecter le signal clicked du bouton de confirmation à la fonction handleConfirmationButtonClick
    connect(ui->Button_ConfirmationMateriauxPodcast, &QPushButton::clicked, this, &MainWindow::handleConfirmationButtonClick);
}


// Retirez les fonctions handlePodcastSelection et handleMaterialSelection

void MainWindow::handleConfirmationButtonClick() {
    // Récupérer le podcast sélectionné
    QListWidgetItem *podcastItem = ui->listWidget_podcasts->currentItem();
    QString selectedPodcast = podcastItem->text();

    // Récupérer le matériau sélectionné
    QListWidgetItem *materialItem = ui->listWidget_materiaux->currentItem();
    QString selectedMaterialWithType = materialItem->text();
    QString selectedMaterial = selectedMaterialWithType.split(" - ").at(0);

    // Récupérer l'ID du podcast sélectionné
    QSqlQuery podcastQuery;
    podcastQuery.prepare("SELECT ID_PODCAST FROM PODCASTS WHERE NOM = :nom");
    podcastQuery.bindValue(":nom", selectedPodcast);
    if (podcastQuery.exec() && podcastQuery.next()) {
        int podcastId = podcastQuery.value(0).toInt();

        // Récupérer l'ID du matériau sélectionné
        QSqlQuery materialQuery;
        materialQuery.prepare("SELECT ID_MATERIEL FROM MATERIAUX WHERE NOM = :nom");
        materialQuery.bindValue(":nom", selectedMaterial);
        if (materialQuery.exec() && materialQuery.next()) {
            int materialId = materialQuery.value(0).toInt();

            // Insérer un nouvel enregistrement dans la table CONTENIR pour le matériau et le podcast sélectionnés
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO CONTENIR (ID_MATERIEL, ID_PODCAST) VALUES (:idMateriel, :idPodcast)");
            insertQuery.bindValue(":idMateriel", materialId);
            insertQuery.bindValue(":idPodcast", podcastId);
            if (insertQuery.exec()) {
                qDebug() << "Matériau inséré avec succès dans la table CONTENIR !";

                // Actualiser la page après l'ajout
                selectMaterialAndInsertIntoContenir();
            } else {
                qDebug() << "Erreur lors de l'insertion du matériau dans la table CONTENIR :" << insertQuery.lastError().text();
            }
        } else {
            qDebug() << "Erreur lors de la récupération de l'ID du matériau :" << materialQuery.lastError().text();
        }
    } else {
        qDebug() << "Erreur lors de la récupération de l'ID du podcast :" << podcastQuery.lastError().text();
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------//
/*///////////////////////////////////////////BILAN FINANCIER//////////////////////////////////////////////////////////*/

void MainWindow::bilanFinancier() {
    // Accéder à l'onglet "Bilan Financier"
    ui->tabWidget_12->setCurrentIndex(3); // L'index de l'onglet "Bilan Financier" est 3

    QSqlQuery query;
    query.prepare("SELECT "
                  "COUNT(*) AS totalTransactions, "
                  "LISTAGG(MODEDEPAIEMENT, ', ') WITHIN GROUP (ORDER BY MODEDEPAIEMENT) AS modesDePaiement, "
                  "SUM(CASE WHEN UPPER(TYPE) = 'REVENUE' THEN CAST(MONTANT AS NUMBER) ELSE 0 END) AS totalRevenus, "
                  "SUM(CASE WHEN UPPER(TYPE) = 'DEPENSE' THEN CAST(MONTANT AS NUMBER) ELSE 0 END) AS totalDepenses "
                  "FROM TRANSACTIONS");


    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des données du bilan financier:" << query.lastError().text();
        return;
    }

    int totalTransactions = 0;
    QString modesDePaiement;
    double totalRevenus = 0;
    double totalDepenses = 0;

    if (query.next()) {
        totalTransactions = query.value(0).toInt();
        modesDePaiement = query.value(1).toString();
        totalRevenus = query.value(2).toDouble();
        totalDepenses = query.value(3).toDouble();
    }

    // Calculer le bénéfice ou la perte net(te)
    double beneficeOuPerte = totalRevenus - totalDepenses;

    // Afficher les résultats du bilan financier dans l'onglet "Bilan Financier"
    afficherResultatsBilanFinancier(ui->textEditBilanFinancier, totalTransactions, modesDePaiement, totalRevenus, totalDepenses, beneficeOuPerte);
}




void MainWindow::afficherResultatsBilanFinancier(QTextEdit *textEdit, int totalTransactions, const QString& modesDePaiement, double totalRevenus, double totalDepenses, double beneficeOuPerte) {
    QString rapportTexte;

    // En-tête du rapport
    rapportTexte += "<div style='background-color: #f5f5f5; border-bottom: 2px solid #ccc; padding: 20px;'>";
    rapportTexte += "<h2 style='color: #333; font-family: Arial, sans-serif; font-size: 24px; margin-bottom: 20px; text-align: center;'>Résultats financiers</h2>";

    // Section des totaux
    rapportTexte += "<div style='background-color: #fff; border: 1px solid #ddd; border-radius: 5px; padding: 20px;'>";
    rapportTexte += "<h3 style='color: #555; font-family: Arial, sans-serif; font-size: 20px; margin-bottom: 15px;'>Totaux</h3>";
    rapportTexte += "<ul style='list-style-type: none; padding: 0; margin: 0;'>";

    rapportTexte += "<li><strong style='color: #333;'>Total des transactions :</strong> " + QString::number(totalTransactions) + "</li>";
    rapportTexte += "<li><strong style='color: #333;'>Modes de paiement :</strong> " + modesDePaiement + "</li>";
    rapportTexte += "<li><strong style='color: #333;'>Revenus totaux :</strong> " + QString::number(totalRevenus, 'f', 2) + "</li>";
    rapportTexte += "<li><strong style='color: #333;'>Dépenses totales :</strong> " + QString::number(totalDepenses, 'f', 2) + "</li>";
    rapportTexte += "<li><strong style='color: #333;'>Bénéfice ou perte net(te) :</strong> " + QString::number(beneficeOuPerte, 'f', 2) + "</li>";
    rapportTexte += "</ul>";
    rapportTexte += "</div>"; // Fin de la section des totaux

    rapportTexte += "</div>"; // Fin de l'en-tête du rapport

    // Afficher le rapport en texte dans le QTextEdit
    textEdit->setHtml(rapportTexte);
}





//---------------------------------------------------------------TRANSACTIONS lou------------------------------------///------------------------------------------//
//-----------------------------Fonction pour mettre le tableau a jour apres les operations------------------------//
void MainWindow::update_table_de_transactions()
{
    Connection c;
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM TRANSACTIONS");
    ui->table_de_transactions->clearContents();
    ui->table_de_transactions->setRowCount(0); // Réinitialise les lignes du tableau
    for (int row = 0; row < model->rowCount(); ++row)
    {
        ui->table_de_transactions->insertRow(row);
        for (int col = 0; col < 6; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->table_de_transactions->setItem(row, col, item);
        }
    }
    envoyerNofiticatiionDepassementRevenuesEtDepenses();
    delete model;
}
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------FONCTION D'AJOUT--------------------------------------------------------------//
void MainWindow::on_pushButton_Ajout_Transaction_clicked()
{
    QString modeDePaiement = ui->lineEdit_ModeDePaiement_Transaction->currentText();
    QString type = ui->lineEdit_Type_Transaction->currentText();
    QString categorie = ui->LineEdit_Categorie_Transaction->currentText();
    QString dateTransaction = ui->LineEdit_Date_Transaction->text();
    QString montant = ui->lineEdit_Montant_Transaction->text();
    Transaction i;
    i.set_modeDePaiement(modeDePaiement);
    i.set_type(type);
    i.set_categorie(categorie);
    i.set_dateTransaction(dateTransaction);
    i.set_montant(montant);
    Connection c;
    if (c.createconnect())
    {
        if (i.ajouterTransaction())
        {
            ui->champ_resultat_CRUD_Transaction->setText("Ajout avec succées !");
            update_table_de_transactions();
            AfficherEtmettreAJourPieChartTransaction();
        }
        else
        {
            ui->champ_resultat_CRUD_Transaction->setText("Echec de l'ajout !");
        }
    }
}
//---------------------------------------------------------------------------------------------------------------//
//-------------------------------FONCTION DE MODIFICATION--------------------------------------------------------//
void MainWindow::on_pushButton_Modification_Transaction_clicked()
{
    QItemSelectionModel *select = ui->table_de_transactions->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty())
    {
        int row = selectedIndexes.first().row();
        int idToModify = ui->table_de_transactions->item(row, 5)->text().toInt();
        QString modeDePaiement = ui->table_de_transactions->item(row, 0)->text();
        QString type = ui->table_de_transactions->item(row, 1)->text();
        QString categorie = ui->table_de_transactions->item(row, 2)->text();
        QString dateTransaction = ui->table_de_transactions->item(row, 3)->text();
        QString montant = ui->table_de_transactions->item(row, 4)->text();
        Transaction i;
        if (i.modifierTransaction(idToModify, modeDePaiement, type, categorie, dateTransaction, montant))
        {
            ui->table_de_transactions->setItem(row, 0, new QTableWidgetItem(modeDePaiement));
            ui->table_de_transactions->setItem(row, 1, new QTableWidgetItem(type));
            ui->table_de_transactions->setItem(row, 2, new QTableWidgetItem(categorie));
            ui->table_de_transactions->setItem(row, 3, new QTableWidgetItem(dateTransaction));
            ui->table_de_transactions->setItem(row, 4, new QTableWidgetItem(montant));
            emit signal_table_de_transaction_mise_a_jour(); // Mettre à jour la vue après la modification
            ui->champ_resultat_CRUD_Transaction->setText("Données modifiées avec succés.");
            AfficherEtmettreAJourPieChartTransaction();
        }
        else
        {
            ui->champ_resultat_CRUD_Transaction->setText("Echec de la modification !");
        }
    }
    else
    {
        ui->champ_resultat_CRUD_Transaction->setText("Veuillez selectionner une case à modifier !");
    }
    for (int r = 0; r < ui->table_de_transactions->rowCount(); ++r)
    {
        for (int c = 0; c < ui->table_de_transactions->columnCount(); ++c)
        {
            QWidget *cellWidget = ui->table_de_transactions->cellWidget(r, c);
            if (cellWidget)
            {
                cellWidget->deleteLater();
            }
        }
    }
    update_table_de_transactions();
}
//-------------------------------------------------------------------------------------------------------------//
//----------------------------------------FONCTION DE SUPPRESSION----------------------------------------------//
void MainWindow::on_pushButton_Suppression_Transaction_clicked()
{
    QItemSelectionModel *select = ui->table_de_transactions->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty())
    {
        int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->table_de_transactions->item(row, 5); // Vérifiez la colonne contenant l'ID après le tri
        if (item) // Vérifiez si l'élément existe
        {
            int idToDelete = item->text().toInt();
            Transaction i;
            if (i.supprimerTransaction(idToDelete))
            {
                // Supprimer les widgets de cellule associés à cette ligne
                for (int c = 0; c < ui->table_de_transactions->columnCount(); ++c)
                {
                    QWidget *cellWidget = ui->table_de_transactions->cellWidget(row, c);
                    if (cellWidget)
                    {
                        cellWidget->deleteLater();
                    }
                }
                ui->table_de_transactions->removeRow(row);
                emit signal_table_de_transaction_mise_a_jour(); // Mettre à jour la vue après la suppression
                ui->champ_resultat_CRUD_Transaction->setText("Donnée supprimée avec succès.");
                AfficherEtmettreAJourPieChartTransaction();
            }
            else
            {
                ui->champ_resultat_CRUD_Transaction->setText("Echec de la suppression !");
            }
        }
        else
        {
            ui->champ_resultat_CRUD_Transaction->setText("Selected row does not contain ID.");
        }
    }
    else
    {
        ui->champ_resultat_CRUD_Transaction->setText("Veuillez sélectionner une ligne à supprimer !");
    }
}
//-------------------------------------------------------------------------------------------------------------//
//--------------------------------------FONCTION DE TRI PAR DATE-----------------------------------------------//
void MainWindow::on_Button_trierParDate_Transaction_clicked()
{
    ui->table_de_transactions->clearContents(); // Efface le contenu actuel du tableau
    ui->table_de_transactions->setRowCount(0); // Efface également les lignes existantes du tableau
    // Trier les transactions par date
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A4");
    db.setUserName("louayzorai");
    db.setPassword("louayzorai");
    db.open();
    QSqlQuery query(db);
    QString str = ("SELECT * FROM TRANSACTIONS");
    if (query.exec(str))
    {
        ui->table_de_transactions->setColumnCount(6);
        QStringList labels;
        labels << "Mode de paiement" << "Type" << "Categorie" << "Date de transaction" << "Montant" << "ID";
        ui->table_de_transactions->setHorizontalHeaderLabels(labels);
        int row = 0;
        QVector<QPair<QDate, QStringList>> sortedTransactions; // Pour stocker les transactions triées

        while (query.next())
        {
            QStringList transactionData;
            transactionData << query.value(0).toString() << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString();
            QDate date = QDate::fromString(query.value(3).toString(), "dd/MM/yyyy"); // Convertir la chaîne de date en QDate
            sortedTransactions.append(qMakePair(date, transactionData));
        }

        // Tri des transactions par date
        std::sort(sortedTransactions.begin(), sortedTransactions.end(), [](const QPair<QDate, QStringList> &a, const QPair<QDate, QStringList> &b) {
            return a.first < b.first;
        });

        // Affichage des transactions triées dans le tableau
        for (const auto &transaction : sortedTransactions)
        {
            ui->table_de_transactions->insertRow(row);
            for (int col = 0; col < 6; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(transaction.second[col]);
                ui->table_de_transactions->setItem(row, col, item);
            }
            row++;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------//


//-------------------------------------FONCTION D'EXPORTATION EXCEL-----------------------------------------------------------------------------//
void MainWindow::on_pushButton_exportationFormatExcel_Transaction_clicked()
{
    // Créer un objet Workbook
    QXlsx::Document xlsx;

    // Obtenir le nombre de lignes et de colonnes du QTableWidget
    int rowCount = ui->table_de_transactions->rowCount();
    int columnCount = ui->table_de_transactions->columnCount();

    // Ajouter le titre "TABLE DES TRANSACTIONS" sur 4 lignes et 8 colonnes
    QColor couleurCellule(0, 32, 96);
    xlsx.mergeCells(QXlsx::CellRange(1, 1, 4, 24));
    QXlsx::Format titleFormat; // Déclaration de la variable titleFormat
    titleFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    titleFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    titleFormat.setFontBold(true);
    titleFormat.setFontSize(36);
    titleFormat.setFontColor(Qt::white);
    titleFormat.setPatternBackgroundColor(couleurCellule);
    xlsx.write(1, 1, "TABLE DES TRANSACTIONS", titleFormat);

    // Sauter 2 lignes après le titre
    xlsx.write(6, 1, ""); // Écrire une cellule vide à la ligne 6

    // Ajouter les noms de colonnes
    QXlsx::Format nomDeColonneFormat; // Déclaration de la variable titleFormat
    nomDeColonneFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    nomDeColonneFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    nomDeColonneFormat.setFontBold(true);
    nomDeColonneFormat.setFontSize(24);
    nomDeColonneFormat.setFontColor(Qt::white);
    QColor couleurCelluleNomColonne(221, 171, 213);
    nomDeColonneFormat.setPatternBackgroundColor(couleurCelluleNomColonne);

    for (int column = 0; column < columnCount; ++column) {
        QTableWidgetItem *headerItem = ui->table_de_transactions->horizontalHeaderItem(column);
        if (headerItem) {
            xlsx.write(8, column * 4 + 1, headerItem->text(),nomDeColonneFormat);
            xlsx.mergeCells(QXlsx::CellRange(8, column * 4 + 1, 11, column * 4 + 4)); // Étaler sur 4 colonnes et 4 lignes

            // Appliquer le format pour les noms de colonnes
            QXlsx::Format columnNameFormat;
            columnNameFormat.setFontBold(true);
            columnNameFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            columnNameFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
            columnNameFormat.setFontSize(24);
            xlsx.cellAt(8, column * 4 + 1)->format().setFont(columnNameFormat.font());
            xlsx.cellAt(8, column * 4 + 1)->format().setHorizontalAlignment(columnNameFormat.horizontalAlignment());
            xlsx.cellAt(8, column * 4 + 1)->format().setVerticalAlignment(columnNameFormat.verticalAlignment());
            xlsx.cellAt(8, column * 4 + 1)->format().setFontSize(columnNameFormat.fontSize());
        }
    }

    // Parcourir le QTableWidget pour copier les données dans le document Excel
    QXlsx::Format formatCelluleNormale; // Déclaration de la variable titleFormat
    formatCelluleNormale.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    formatCelluleNormale.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    formatCelluleNormale.setFontSize(16);
    formatCelluleNormale.setFontColor(Qt::white);
    formatCelluleNormale.setPatternBackgroundColor(couleurCellule);
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            // Obtenir le texte de la cellule dans le QTableWidget
            QTableWidgetItem *item = ui->table_de_transactions->item(row, column);
            if (item) {
                // Insérer le texte de la cellule dans le document Excel
                xlsx.write(row + 12, column * 4 + 1, item->text(),formatCelluleNormale);
                xlsx.mergeCells(QXlsx::CellRange(row + 12, column * 4 + 1, row + 13, column * 4 + 4)); // Étaler sur 4 colonnes et 2 lignes
            }
        }
    }
    // Chemin vers votre image
    QString imagePath = "/tsawer/logo pour excel.png";

    // Charger l'image à partir du chemin
    QImage image(imagePath);

    // Insérer l'image dans le fichier Excel
    xlsx.insertImage(0, 0, image);
    // Sauvegarder le document Excel
    QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter en Excel"), "", tr("Fichiers Excel (*.xlsx)"));
    if (!filePath.isEmpty()) {
        xlsx.saveAs(filePath);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------//



//------------------------------------------FONCTION TRI PAR MONTANT--------------------------------------------------------------------//
void MainWindow::on_Button_TrierParMontant_Transaction_clicked() // tri par montant
{
    ui->table_de_transactions->clearContents(); // Efface le contenu actuel du tableau
    ui->table_de_transactions->setRowCount(0); // Efface également les lignes existantes du tableau
    // Trier les transactions par montant
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A4");
    db.setUserName("louayzorai");
    db.setPassword("louayzorai");
    db.open();
    QSqlQuery query(db);
    QString str = ("SELECT * FROM TRANSACTIONS");
    if (query.exec(str))
    {
        ui->table_de_transactions->setColumnCount(6);
        QStringList labels;
        labels << "Mode de paiement" << "Type" << "Categorie" << "Date de transaction" << "Montant" << "ID";
        ui->table_de_transactions->setHorizontalHeaderLabels(labels);
        int row = 0;
        QVector<QPair<double, QStringList>> sortedTransactions; // Pour stocker les transactions triées par montant

        while (query.next())
        {
            QStringList transactionData;
            transactionData << query.value(0).toString() << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString();
            double montant = query.value(4).toDouble(); // Obtenez le montant en double
            sortedTransactions.append(qMakePair(montant, transactionData));
        }

        // Tri des transactions par montant
        std::sort(sortedTransactions.begin(), sortedTransactions.end(), [](const QPair<double, QStringList> &a, const QPair<double, QStringList> &b) {
            return a.first < b.first;
        });

        // Affichage des transactions triées dans le tableau
        for (const auto &transaction : sortedTransactions)
        {
            ui->table_de_transactions->insertRow(row);
            for (int col = 0; col < 6; ++col)
            {
                QTableWidgetItem *item = new QTableWidgetItem(transaction.second[col]);
                ui->table_de_transactions->setItem(row, col, item);
            }
            row++;
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------//


//---------------------------------------------------FONCTION DE RECHERCHE PAR MONTANT---------------------------------------------------------//
void MainWindow::searchTransactionsByAmount_Transaction(const QString &amount)
{
    // Valider la saisie du montant
    bool amountIsValid = true;
    QString errorMessage;

    // Vérifier si la saisie est invalide
    QRegExp amountRegExp("^(\\d+\\.?\\d*|\\.\\d+)$");
    if (!amountRegExp.exactMatch(amount) && !amount.isEmpty()) {
        amountIsValid = false;
        errorMessage = "Montant invalide. Veuillez saisir uniquement des chiffres et un point.";
    }

    // Afficher un message d'erreur si le montant n'est pas valide
    if (!amountIsValid) {
        ui->champ_erreur_rechercheParMontant->setText(errorMessage);
        return;
    } else {
        ui->champ_erreur_rechercheParMontant->clear();
    }

    // Filtrer les transactions en fonction du montant valide
    QString filterAmount = amount.isEmpty() ? "" : amount;
    for (int row = 0; row < ui->table_de_transactions->rowCount(); ++row) {
        QString currentAmount = ui->table_de_transactions->item(row, 4)->text();
        bool showRow = currentAmount.startsWith(filterAmount, Qt::CaseInsensitive);

        // Ne pas appliquer le filtre si le montant n'est pas valide
        if (!amountIsValid) {
            showRow = true;
        }

        ui->table_de_transactions->setRowHidden(row, !showRow);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------FONCTION BOUTON ANNULER--------------------------------------------------------------------//
void MainWindow::on_pushButton_Annuler_Transaction_clicked()
{
       ui->lineEdit_Montant_Transaction->clear();
}
//----------------------------------------------------------------------------------------------------------------------------------------//



//-----------------------LES FONCTIONS DE CONTROLE DE SAISIE + GERER LES EVENEMENTS-------------------------------------------------------//
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
        if (obj == ui->lineEdit_Montant_Transaction && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease))
        {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                QString text = ui->lineEdit_Montant_Transaction->text();

                // Seuls les chiffres, un point et la touche "Backspace" sont autorisés
                if (!(keyEvent->text().isEmpty() && keyEvent->key() != Qt::Key_Backspace) && !(keyEvent->text().at(0).isDigit() || keyEvent->text() == "." || keyEvent->key() == Qt::Key_Backspace))
                {
                    ui->champs_erreur_ajout_Transaction->setText("Le montant doit contenir uniquement\ndes chiffres et un point !");
                    return true;
                }

                // On autorise un seul point décimal
                if (keyEvent->text() == "." && text.contains("."))
                {
                    ui->champs_erreur_ajout_Transaction->setText("Vous ne pouvez entrer qu'un\nseul point décimal !");
                    return true;
                }

                // Si la saisie est valide, effacez le contenu du QLabel
                if (text.isEmpty() || (text.toDouble() != 0.0 && text.toDouble() != 0.0))
                {
                    ui->champs_erreur_ajout_Transaction->clear(); // Efface le contenu du QLabel
                }
        }
        if (event->type() == QEvent::MouseButtonPress)
        {
                // Si l'événement est un clic de souris, effacez le contenu du champ label_10
                ui->champ_resultat_CRUD_Transaction->clear();
                // Si l'événement est un clic de souris, effacez les champs de modification du tableau
                for (int r = 0; r < ui->table_de_transactions->rowCount(); ++r)
                {
                    for (int c = 0; c < ui->table_de_transactions->columnCount(); ++c)
                    {
                        QWidget *cellWidget = ui->table_de_transactions->cellWidget(r, c);
                        if (cellWidget) {
                            cellWidget->deleteLater();
                        }
                    }
                }
        }
        if (obj == ui->lineEdit_RechercheParMONTANT_Transaction && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease))
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            QString text = ui->lineEdit_RechercheParMONTANT_Transaction->text();

            // Seuls les chiffres, un point et la touche "Backspace" sont autorisés
            if (!(keyEvent->text().isEmpty() && keyEvent->key() != Qt::Key_Backspace) && !(keyEvent->text().at(0).isDigit() || keyEvent->text() == "." || keyEvent->key() == Qt::Key_Backspace))
            {
                ui->champ_erreur_rechercheParMontant->setText("Le montant doit contenir uniquement des chiffres et un point !");
                return true;
            }

            // On autorise un seul point décimal
            if (keyEvent->text() == "." && text.contains("."))
            {
                ui->champ_erreur_rechercheParMontant->setText("Vous ne pouvez entrer qu'un seul point décimal !");
                return true;
            }

            // Si la saisie est valide, effacez le contenu du QLabel
            if (text.isEmpty() || (text.toDouble() != 0.0 && text.toDouble() != 0.0))
            {
                ui->champ_erreur_rechercheParMontant->clear(); // Efface le contenu du QLabel
            }
        }
        return QObject::eventFilter(obj, event);
}

void MainWindow::preventCellEdit_Transactions()
{
    ui->table_de_transactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::showEditChoices_Transactions(int row, int column)
{
        if (column == 0)
        {
                QStringList choices = {"Virement", "Cheques", "Especes"};
                showChoicesUnderCell_Transactions(row, column, choices);
        }

}

void MainWindow::showChoicesUnderCell_Transactions(int row, int column, const QStringList &choices)
{
    // Sauvegarder la modification actuelle dans le tableau avant de supprimer les combobox précédents
    QWidget *previousWidget = ui->table_de_transactions->cellWidget(row, column);
    if (previousWidget)
    {
        QComboBox *previousComboBox = previousWidget->findChild<QComboBox*>();
        if (previousComboBox)
        {
            QString newText = previousComboBox->currentText();
            ui->table_de_transactions->item(row, column)->setText(newText);
        } else
        {
            QLineEdit *previousLineEdit = previousWidget->findChild<QLineEdit*>();
            if (previousLineEdit) {
                QString newText = previousLineEdit->text();
                ui->table_de_transactions->item(row, column)->setText(newText);
            }
        }
    }

    // Supprimer tous les QComboBox et les lignes de saisie de date présents dans le tableau
    for (int r = 0; r < ui->table_de_transactions->rowCount(); ++r)
    {
        for (int c = 0; c < ui->table_de_transactions->columnCount(); ++c)
        {
            QWidget *cellWidget = ui->table_de_transactions->cellWidget(r, c);
            if (cellWidget) {
                cellWidget->deleteLater();
            }
        }
    }

    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    if (column == 3) { // Si c'est la colonne de la date
        QDateEdit *dateEdit = new QDateEdit();
        dateEdit->setDisplayFormat("dd/MM/yyyy"); // Format de la date
        layout->addWidget(dateEdit);
        layout->setAlignment(Qt::AlignTop);

        ui->table_de_transactions->setCellWidget(row, column, widget);

        connect(dateEdit, &QDateEdit::dateChanged, [=](const QDate &date) {
            // Mettre à jour la valeur de la cellule dans le QTableWidget avec la valeur de la date formatée
            QString formattedDate = date.toString("dd/MM/yyyy");
            ui->table_de_transactions->item(row, column)->setText(formattedDate);
        });
    } else { // Pour toutes les autres colonnes
        QComboBox *newComboBox = new QComboBox();
        newComboBox->addItems(choices);
        newComboBox->setCurrentIndex(-1); // Définir la sélection initiale sur -1

        layout->addWidget(newComboBox);
        layout->setAlignment(Qt::AlignTop);

        ui->table_de_transactions->setCellWidget(row, column, widget);

        connect(newComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=]() {
            // Mettre à jour la valeur de la cellule dans le QTableWidget avec la valeur sélectionnée dans le QComboBox
            QString newText = newComboBox->currentText();
            ui->table_de_transactions->item(row, column)->setText(newText);
        });
    }
}


void MainWindow::handleCellClicked_Transactions(int row, int column)
{
    if (column == 3) { // Si la colonne est la colonne de la date
        QStringList choices; // Vous pouvez remplir cette liste avec des choix de date si nécessaire
        showChoicesUnderCell_Transactions(row, column, choices);
    }
}
void MainWindow::editMontantCell_Transactions(int row, int column)
{
    if (column == 4)
    { // Vérifier si la colonne est la colonne "montant"
        QLineEdit *lineEdit = new QLineEdit(ui->table_de_transactions);
        QString text = ui->table_de_transactions->item(row, column)->text();
        lineEdit->setText(text);
        ui->table_de_transactions->setCellWidget(row, column, lineEdit);
        lineEdit->setFocus();
        connect(lineEdit, &QLineEdit::textChanged, [=](const QString &newText)
        {
            QRegExp regExp("^\\d*\\.?\\d*$"); // Expression régulière pour vérifier la validité du montant
            if (!regExp.exactMatch(newText)) {
                // Si la saisie est invalide, effacer le contenu du line edit
                lineEdit->setText(text);
            }
        });
        connect(lineEdit, &QLineEdit::editingFinished, [=]()
        {
            // Mettre à jour la cellule du tableau avec le nouveau montant
            QString newText = lineEdit->text();
            QTableWidgetItem *item = new QTableWidgetItem(newText);
            ui->table_de_transactions->setItem(row, column, item);
            // Supprimer le line edit après l'édition
            ui->table_de_transactions->removeCellWidget(row, column);
        });
    }
}
void MainWindow::updateCategoryOptions()
{
    QString transactionType = ui->lineEdit_Type_Transaction->currentText();
    ui->LineEdit_Categorie_Transaction->clear(); // Effacer les choix précédents

    if (transactionType == "Depense") {
        // Ajouter les options pour les dépenses
        ui->LineEdit_Categorie_Transaction->addItem("Payement employe");
        ui->LineEdit_Categorie_Transaction->addItem("Payement facture");
        ui->LineEdit_Categorie_Transaction->addItem("Location local");
        ui->LineEdit_Categorie_Transaction->addItem("Achat materiel");
        ui->LineEdit_Categorie_Transaction->addItem("Autres depenses");
    } else if (transactionType == "Revenue") {
        // Ajouter les options pour les revenus
        ui->LineEdit_Categorie_Transaction->addItem("Vente materiel");
        ui->LineEdit_Categorie_Transaction->addItem("Autres revenues");
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------//


//---------------------------------------------------------LES NOTIFICATIONS-------------------------------------------------------------------//
void MainWindow::envoyerNotificationTransaction(QString a, QString b)
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon("C:/Users/user/Desktop/ESPRIT/2A4/2eme periode/projet/Module_Finance/logo 2.png"));
    trayIcon->setToolTip("Tooltip de l'icône de plateau");
    trayIcon->show();
    trayIcon->showMessage(a, b, QSystemTrayIcon::Information, 3000);
}

void MainWindow::envoyerNofiticatiionDepassementRevenuesEtDepenses()
{
    QSqlQuery query1, query2;
    query1.prepare("SELECT SUM (MONTANT) AS total_depenses FROM TRANSACTIONS WHERE TYPE = :type");
    query1.bindValue(":type", "Depense");
    query1.exec();
    double diff = 0.0;
    if (query1.next())
    {
        double sommeDepenses = query1.value("total_depenses").toDouble();
        query2.prepare("SELECT SUM (MONTANT) AS total_revenues FROM TRANSACTIONS WHERE TYPE = :type");
        query2.bindValue(":type", "Revenue");
        query2.exec();
        if (query2.next())
        {
            double sommeRevenues = query2.value("total_revenues").toDouble();
            double difference = sommeRevenues - sommeDepenses;
            diff = difference;
        }
    }
    if (diff < 0)
    {
        envoyerNotificationTransaction("ATTENTION","Les dépenses ont depassé les revenues");
    }
}


//----------------------------------------STATS-------------------------------------------------------------//
void MainWindow::AfficherEtmettreAJourPieChartTransaction()
{
    Connection c;
    c.createconnect();
    float percentPayementEmploye = 0.0;
    float percentPayementFacture = 0.0;
    float percentLocationLocal = 0.0;
    float percentAchatMateriel = 0.0;
    float percentAutresDepenses = 0.0;
    QSqlQuery sumQuery;
    float totalAmount = 0.0;

    if (sumQuery.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE type = 'Depense'"))
    {
        if (sumQuery.next()) {
            totalAmount = sumQuery.value(0).toFloat();
        }
    }

    if (totalAmount == 0.0)
    {
        QPieSeries *emptySeries = new QPieSeries();
        emptySeries->append("Aucune dépense", 0);
        QChart *chart = new QChart();
        chart->addSeries(emptySeries);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->setTitle("Statistiques des dépenses selon la categorie");
        chart->setTitleFont(QFont("Arial", 200, QFont::Bold)); // Set font and size for the title
        chart->setTheme(QChart::ChartThemeBlueCerulean);
        QChartView *chartview = new QChartView(chart);
        chartview->setRenderHint(QPainter::Antialiasing);
        chartview->setParent(ui->statsTransactions);
        chartview->setGeometry((ui->statsTransactions->width() - 1000) / 2,
                               (ui->statsTransactions->height() - 400) / 2,
                               1000, 400);
        return;
    }

    QSqlQuery query;
    // Queries to calculate percentages for each category
    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Payement employe'"))
    {
        query.next();
        float totalPayementEmploye = query.value(0).toFloat();
        percentPayementEmploye = (totalPayementEmploye / totalAmount) * 100.0;
    }
    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Payement facture'"))
    {
        query.next();
        float totalPayementFacture = query.value(0).toFloat();
        percentPayementFacture = (totalPayementFacture / totalAmount) * 100.0;
    }
    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Location local'"))
    {
        query.next();
        float totalLocationLocal = query.value(0).toFloat();
        percentLocationLocal = (totalLocationLocal / totalAmount) * 100.0;
    }
    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Achat materiel'"))
    {
        query.next();
        float totalAchatMateriel = query.value(0).toFloat();
        percentAchatMateriel = (totalAchatMateriel / totalAmount) * 100.0;
    }
    if (query.exec("SELECT SUM(MONTANT) FROM TRANSACTIONS WHERE CATEGORIE = 'Autres depenses'"))
    {
        query.next();
        float totalAutresDepenses = query.value(0).toFloat();
        percentAutresDepenses = (totalAutresDepenses / totalAmount) * 100.0;
    }

    // Debugging the stylesheet and font size for the chart title
    qDebug() << "Setting stylesheet for QChart title font size";
    ui->statsTransactions->setStyleSheet("QChart { font-size: 14pt; }");

    // Create pie series and slices
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.3);

    // Append only if percentage is not zero and add legend
    if (percentPayementEmploye != 0.0) {
        series->append(QString("Payement employe %1%").arg(percentPayementEmploye, 0, 'f', 2), percentPayementEmploye);
        series->setLabelsVisible(true); // Show labels for non-zero percentages
    }
    if (percentPayementFacture != 0.0) {
        series->append(QString("Payement facture %1%").arg(percentPayementFacture, 0, 'f', 2), percentPayementFacture);
        series->setLabelsVisible(true); // Show labels for non-zero percentages
    }
    if (percentLocationLocal != 0.0) {
        series->append(QString("Location local %1%").arg(percentLocationLocal, 0, 'f', 2), percentLocationLocal);
        series->setLabelsVisible(true); // Show labels for non-zero percentages
    }
    if (percentAchatMateriel != 0.0) {
        series->append(QString("Achat materiel %1%").arg(percentAchatMateriel, 0, 'f', 2), percentAchatMateriel);
        series->setLabelsVisible(true); // Show labels for non-zero percentages
    }
    if (percentAutresDepenses != 0.0) {
        series->append(QString("Autres depenses %1%").arg(percentAutresDepenses, 0, 'f', 2), percentAutresDepenses);
        series->setLabelsVisible(true); // Show labels for non-zero percentages
    }

    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("Répartition des dépenses selon la categorie");
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold)); // Set font and size for the title
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->legend()->setVisible(true); // Show legend
    chart->legend()->setAlignment(Qt::AlignRight); // Align legend to right
    QFont legendFont("Arial", 14); // Set font for legend
    chart->legend()->setFont(legendFont);

    // Set up the chart view
    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setParent(ui->statsTransactions);
    chartview->setGeometry((ui->statsTransactions->width() - 1000) / 2,
                           (ui->statsTransactions->height() - 400) / 2,
                           1000, 400);
}










// Ismail :


void MainWindow::on_pushButton_ajouterpod_clicked()
{

    QString nom = ui->lineEdit_nomp->text();
    QString duree = ui->lineEdit_dureep->text();
    QString lieu = ui->lineEdit_lieup->text();
    QString categorie = ui->comboBoxp->currentText();
    QString date_pod = ui->dateEditp->text();
    //int id = ui->lineEdit_7p->text().toInt(); // Assuming this is the employee ID
    int id = ui->lineEdit_7p->text().toInt();
    // Vérifier si les champs obligatoires sont vides
    if (nom.isEmpty() || duree.isEmpty() || lieu.isEmpty() || categorie.isEmpty() || date_pod.isEmpty()) {
        QMessageBox::warning(this, "Warning", "veuillez remplir toutes les cases.");
        return;
    }

    // Ajouter ici les expressions régulières pour la validation des champs si nécessaire

    podcast p(nom, duree, lieu, categorie, date_pod);
    bool success = false;

    if (ui->lineEdit_7p->text() == "") {
         // Si l'ID est 0, cela signifie que nous ajoutons un nouvel enregistrement
         success = p.ajouterp();
         statistiquesP();

         logToFile("<b style='font-size: 14px; color: darkgreen;'>Historique d'Ajout :</b><br>", "\n<br>" +
                   QDateTime::currentDateTime().toString("[ddd MMM dd HH:mm:ss]-\n") +
                   "  Un nouveau podcast a été ajouté de la table PODCASTS avec le Nom:  <span style='color: red; font-weight: bold; font-style: italic;'> \n" +
                   QString("<b>") + nom + QString("</b><br>\n") +
                   "</span>\n");
    } else {
         // Sinon, nous mettons à jour un enregistrement existant

         success = p.update(id,nom, duree, lieu, categorie, date_pod);
        //success = p.update(nom, newNom, newDuree, newLieu, newCategorie, newDatePod);
         ui->lineEdit_7p->clear();
         QMessageBox::information(nullptr, QObject::tr("OK"),
                       QObject::tr("Modification  effectué\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);
         statistiquesP();
    }

    QMessageBox msgBox;
    if (success) {
        //  msgBox.setText("Operation successful.");
        ui->tableViewp->setModel(p.afficherp());
        ui->lineEdit_nomp->clear();
        ui->lineEdit_dureep->clear();
        ui->lineEdit_lieup->clear();
        //ui->lineEdit_categoriep->clear();
        ui->dateEditp->clear();
    } else {
        msgBox.setText("Operation failed.");
        msgBox.exec();
    }

    // Validation du champ "duree" (chiffres uniquement)
    QRegularExpression regex("[0-9]+");
    if (!regex.match(duree).hasMatch()) {
        QMessageBox::warning(this, "Warning", "duree format Invalide. veuillez entrez des numeros.");
        return;

    }

}


void MainWindow::on_pushButton_annulerpod_clicked()
{
    ui->lineEdit_nomp->clear();
    ui->lineEdit_dureep->clear();
    ui->lineEdit_lieup->clear();
    //ui->lineEdit_categoriep->clear();
    ui->dateEditp->clear();
}



void MainWindow::on_pushButton_supprimerpod_clicked()
{

    QString nom = ui->lineEdit_7p->text();
    // Obtenez le nom entré
    podcast p;
    QSqlQuery query;
    //query.prepare("SELECT id_podcast, nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE nom = ?");
    query.prepare("SELECT nom FROM PODCASTS WHERE nom = ?");
    query.addBindValue(nom);
    if (query.exec() && query.next()) {
        // Récupérez les valeurs de la requête
        //int id = query.value(0).toInt(); // Récupérez l'ID du podcast pour la suppression

        QString nom = query.value(0).toString();
        QString duree = query.value(1).toString();
        QString lieu = query.value(2).toString();
        QString categorie = query.value(2).toString();
        QString date_pod = query.value(4).toString();


        // Utilisez les valeurs récupérées pour instancier un objet podcast
        p = podcast(nom, duree, lieu, categorie, date_pod);

        // Supprimer le podcast
        if (p.supprimerp(nom)) {
            ui->tableViewp->setModel(p.afficherp());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                QObject::tr("Suppression effectuée\n"
                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            // Afficher un message indiquant que le podcast est introuvable
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                QObject::tr("Podcast inexistant.\n"
                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    } else {
        // Afficher un message indiquant que le podcast est introuvable
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
            QObject::tr("Podcast inexistant.\n"
            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    logToFile("<b style='font-size: 14px; color: #00008B;'>Historique de Suppression :</b><br>", "<br>" +
              QDateTime::currentDateTime().toString("[ ddd MMM dd HH:mm:ss ]- ") +
              "  Un nouveau podcast a été supprimé de la table PODCASTS avec le Nom:  <span style='color: red; font-weight: bold; font-style: italic;'> \n" +
              QString("<b>") + nom + QString("</b><br>") +
              "\n</span>\n");

}



void MainWindow::on_pushButton_modifierpod_clicked()
{
    QString nom = ui->lineEdit_7p->text(); // Obtenez le nom entré

    QSqlQuery query;
    query.prepare("SELECT nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE nom = ?");
    query.addBindValue(nom);

    if (query.exec() && query.next()) {
        // Récupérez les valeurs de la requête
        QString oldNom = query.value(0).toString();
        QString oldDuree = query.value(1).toString();
        QString oldLieu = query.value(2).toString();
        QString oldCategorie = query.value(3).toString();
        QString oldDatePod = query.value(4).toString();

        // Affichez les informations du podcast dans les champs de saisie pour permettre la modification
        ui->lineEdit_nomp->setText(oldNom);
        ui->lineEdit_dureep->setText(oldDuree);
        ui->lineEdit_lieup->setText(oldLieu);
        ui->comboBoxp->setCurrentText(oldCategorie);
        ui->dateEditp->setDate(QDate::fromString(oldDatePod, "yyyy-MM-dd"));

        // Demandez la confirmation de la modification
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Confirmation"),
                                                                  tr("Voulez-vous vraiment modifier le podcast sélectionné ?"),
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Mettre à jour le podcast avec les nouvelles valeurs
            QString newNom = ui->lineEdit_nomp->text();
            QString newDuree = ui->lineEdit_dureep->text();
            QString newLieu = ui->lineEdit_lieup->text();
            QString newCategorie = ui->comboBoxp->currentText();
            QString newDatePod = ui->dateEditp->text();

            // Mettre à jour la base de données avec les nouvelles valeurs
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE PODCASTS SET nom = ?, duree = ?, lieu = ?, categorie = ?, date_pod = ? WHERE nom = ?");
            updateQuery.addBindValue(newNom);
            updateQuery.addBindValue(newDuree);
            updateQuery.addBindValue(newLieu);
            updateQuery.addBindValue(newCategorie);
            updateQuery.addBindValue(newDatePod);
            updateQuery.addBindValue(oldNom); // Utilisez l'ancien nom pour la clause WHERE

            // Exécutez la mise à jour de la base de données
            if (updateQuery.exec()) {
                QString modifications; // Variable pour stocker les modifications

                // Vérifiez chaque champ pour détecter les modifications
                if (newNom != oldNom) {
                    modifications += "Nom: Ancien - " + oldNom + ", Nouveau - " + newNom + "\n";
                }
                if (newDuree != oldDuree) {
                    modifications += "Durée: Ancien - " + oldDuree + ", Nouveau - " + newDuree + "\n";
                }
                if (newLieu != oldLieu) {
                    modifications += "Lieu: Ancien - " + oldLieu + ", Nouveau - " + newLieu + "\n";
                }
                if (newCategorie != oldCategorie) {
                    modifications += "Catégorie: Ancien - " + oldCategorie + ", Nouveau - " + newCategorie + "\n";
                }
                if (newDatePod != oldDatePod) {
                    modifications += "Date du Podcast: Ancien - " + oldDatePod + ", Nouveau - " + newDatePod + "\n";
                }

                // Vérifiez si des modifications ont été détectées avant de les enregistrer dans le fichier journal
                if (!modifications.isEmpty()) {
                    // Enregistrez les modifications dans le fichier journal
                    logToFile("<b style='font-size: 14px; color: #800080;'>Historique de Modifications :</b><br>", "\n\n<br>\n" +
                              QDateTime::currentDateTime().toString("[ddd MMM dd HH:mm:ss]-") +
                              "  Un podcast a été modifié dans la table PODCASTS avec les modifications suivantes de: <span style='color: red; font-weight: bold; font-style: italic;'> \n" +
                              QString("<b>") + nom + QString("</b><br>") +
                              "\n</span>\n" +
                              "Modifications:\n" + modifications);
                }
            } else {
                // Afficher une erreur si la mise à jour de la base de données a échoué
                QMessageBox::critical(this, tr("Erreur"),
                                      tr("La mise à jour du podcast a échoué."), QMessageBox::Ok);
            }
        }
    } else {
        // Si le podcast n'a pas été trouvé
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Le podcast sélectionné est introuvable."), QMessageBox::Ok);
    }
}




void MainWindow::on_pushButton_expo_clicked()
{
    // Créer un objet Workbook
    QXlsx::Document xlsx;

    // Obtenir le modèle associé au QTableView
    QAbstractItemModel *model = ui->tableViewp->model();
    if (!model) {
        // Gérer l'erreur ici, le modèle n'est pas disponible
        return;
    }

    // Obtenir le nombre de lignes et de colonnes du modèle
    int rowCount = model->rowCount();
    int columnCount = model->columnCount();

    // Ajouter le titre "TABLE DES PODCASTS" sur 4 lignes et 8 colonnes
    QColor couleurCellule(0, 32, 96);
    xlsx.mergeCells(QXlsx::CellRange(1, 1, 4, 24));
    QXlsx::Format titleFormat; // Déclaration de la variable titleFormat
    titleFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    titleFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    titleFormat.setFontBold(true);
    titleFormat.setFontSize(36);
    titleFormat.setFontColor(Qt::white);
    titleFormat.setPatternBackgroundColor(couleurCellule);
    xlsx.write(1, 1, "TABLE DES PODCASTS", titleFormat);

    // Sauter 2 lignes après le titre
    xlsx.write(6, 1, ""); // Écrire une cellule vide à la ligne 6

    // Ajouter les noms de colonnes
    QXlsx::Format nomDeColonneFormat; // Déclaration de la variable titleFormat
    nomDeColonneFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    nomDeColonneFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    nomDeColonneFormat.setFontBold(true);
    nomDeColonneFormat.setFontSize(24);
    nomDeColonneFormat.setFontColor(Qt::white);
    QColor couleurCelluleNomColonne(221, 171, 213);
    nomDeColonneFormat.setPatternBackgroundColor(couleurCelluleNomColonne);

    for (int column = 0; column < columnCount; ++column) {
        // Insérer le nom de la colonne dans le document Excel
        QString columnName = ui->tableViewp->model()->headerData(column, Qt::Horizontal).toString();
        xlsx.write(8, column * 4 + 1, columnName, nomDeColonneFormat);
        xlsx.mergeCells(QXlsx::CellRange(8, column * 4 + 1, 11, column * 4 + 4)); // Étaler sur 4 colonnes et 4 lignes

        // Appliquer le format pour les noms de colonnes
        QXlsx::Format columnNameFormat;
        columnNameFormat.setFontBold(true);
        columnNameFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        columnNameFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
        columnNameFormat.setFontSize(24);
        xlsx.cellAt(8, column * 4 + 1)->format().setFont(columnNameFormat.font());
        xlsx.cellAt(8, column * 4 + 1)->format().setHorizontalAlignment(columnNameFormat.horizontalAlignment());
        xlsx.cellAt(8, column * 4 + 1)->format().setVerticalAlignment(columnNameFormat.verticalAlignment());
        xlsx.cellAt(8, column * 4 + 1)->format().setFontSize(columnNameFormat.fontSize());
    }

    // Parcourir le QTableView pour copier les données dans le document Excel
    QXlsx::Format formatCelluleNormale; // Déclaration de la variable titleFormat
    formatCelluleNormale.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    formatCelluleNormale.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    formatCelluleNormale.setFontSize(16);
    formatCelluleNormale.setFontColor(Qt::white);
    formatCelluleNormale.setPatternBackgroundColor(couleurCellule);
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            // Obtenir le texte de la cellule dans le QTableView
            QModelIndex index = ui->tableViewp->model()->index(row, column);
            QString cellData = ui->tableViewp->model()->data(index).toString();

            // Insérer le texte de la cellule dans le document Excel
            xlsx.write(row + 12, column * 4 + 1, cellData, formatCelluleNormale);
            xlsx.mergeCells(QXlsx::CellRange(row + 12, column * 4 + 1, row + 13, column * 4 + 4)); // Étaler sur 4 colonnes et 2 lignes
        }
    }

    // Chemin vers votre image
    QString imagePath = "C:/Users/user/Desktop/projet_integration (4)/projet_integration/projet/tsawer/logo pour excel.png";

    // Charger l'image à partir du chemin
    QImage image(imagePath);

    // Insérer l'image dans le fichier Excel
    xlsx.insertImage(0, 0, image);

    // Sauvegarder le document Excel
    QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter en Excel"), "", tr("Fichiers Excel (*.xlsx)"));
    if (!filePath.isEmpty()) {
        xlsx.saveAs(filePath);
    }



    QMessageBox::information(this, "Succès", "Les données ont été exportées vers EXCEL avec succès.");

    logToFile("Historique d'Exportation Excel :", QDateTime::currentDateTime().toString("ddd MMM dd HH:mm:ss yyyy") + " - Un nouveau artiste a été ajouté à la table PODCASTS \n");
}






void MainWindow::on_pushButton_historique_clicked()
{
    p.afficherHistorique();
}



bool MainWindow::eventFilterI(QObject *obj, QEvent *event)
{
        if ((obj == ui->lineEdit_nom || obj == ui->lineEdit_prenomI) && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease))
        {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                QString text = ui->lineEdit_Montant_Transaction->text();

                // Seuls les chiffres, un point et la touche "Backspace" sont autorisés
                // Gestion pour lineEdit_prenomI
                    if (obj == ui->lineEdit_prenomI)
                    {
                        QString text = ui->lineEdit_prenomI->text();
                        // Seuls les caractères alphabétiques et la touche "Backspace" sont autorisés
                        if (!(keyEvent->text().isEmpty() && keyEvent->key() != Qt::Key_Backspace) &&
                            !keyEvent->text().at(0).isLetter() && keyEvent->key() != Qt::Key_Backspace)
                        {
                            ui->label_prenomError->setText("Le prénom doit contenir uniquement des lettres !");
                            return true;
                        }

                        if (text.isEmpty())
                        {
                            ui->label_prenomError->clear();
                        }
                    }

        }
        return QObject::eventFilter(obj, event);
}

void MainWindow::on_pushButton_recherchep_clicked()
{
    QString nom = ui->lineEdit_8p->text(); // Obtenez le nom entré

    QSqlQuery query;
    query.prepare("SELECT nom, duree, lieu, categorie, date_pod FROM PODCASTS WHERE nom = ?");
    query.addBindValue(nom);

    if (query.exec() && query.next()) {
        // Récupérer les valeurs de la requête
        QString nomTrouve = query.value(0).toString();
        QString duree = query.value(1).toString();
        QString lieu = query.value(2).toString();
        QString categorie = query.value(3).toString();
        QString date_pod = query.value(4).toString();

        // Création d'un modèle pour stocker les résultats
        QStandardItemModel *model = new QStandardItemModel(1, 6, this);
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Nom")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Nom du Podcast")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Durée")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Lieu")));
        model->setHorizontalHeaderItem(4, new QStandardItem(QString("Catégorie")));
        model->setHorizontalHeaderItem(5, new QStandardItem(QString("Date du podcast")));

        // Ajouter les valeurs récupérées au modèle
        model->setItem(0, 0, new QStandardItem(nomTrouve));
        model->setItem(0, 1, new QStandardItem(nomTrouve));
        model->setItem(0, 2, new QStandardItem(duree));
        model->setItem(0, 3, new QStandardItem(lieu));
        model->setItem(0, 4, new QStandardItem(categorie));
        model->setItem(0, 5, new QStandardItem(date_pod));

        // Appliquer le modèle à votre TableView
        ui->tableViewp->setModel(model);
    } else {
        // Afficher un message indiquant que le podcast est introuvable
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                QObject::tr("Podcast introuvable.\n"
                                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    ui->lineEdit_8p->clear();
    QFile file("C:/Users/ISMAIL/Desktop/crud/Historique.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString();
        QString logMessage = timestamp + " - Recherche effectuée avec le Nom  : " + nom + "\n";
        out << logMessage;
        file.close();
    }
}


void MainWindow::on_pushButton_trierpod_clicked() //tri
{
    podcast p;

      ui->tableViewp->setModel(p.trip());

      bool test=p.trip();
      if(test){
          QMessageBox::information(nullptr,QObject::tr("OK"),
                                   QObject::tr("tri effectué. \n"
                                               "click Cancel to exist."),QMessageBox::Cancel);

      }
      else
      {
          QMessageBox::critical(nullptr, QObject::tr("not OK"),
                      QObject::tr("tri non effectué.\n"
                                  "Click Cancel to exit."), QMessageBox::Cancel);
      }
}



void MainWindow::logToFile(const QString &type, const QString &message) {
    // Create/Open the history file
    QFile file("C:/Users/user/Desktop/Historique.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open history file for writing.";
        return;
    }

    // Write the log message with timestamp
    QString logEntry =  message + "\n";

    // Write the log message
    QTextStream out(&file);

    // Store messages by type
    static QMap<QString, QStringList> messagesByType;

    // Add message to the list of messages for this type
    messagesByType[type].append(logEntry);

    // Close the file
    file.close();

    // Reopen the file to rewrite all messages with the new ones
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open history file for writing.";
        return;
    }

    // Write messages to file, separating each type with dashes
    for (auto it = messagesByType.constBegin(); it != messagesByType.constEnd(); ++it) {
        out << "----------------------***-----------------------<br>" << endl;
        out << "\n" << it.key() << " " << endl;
        for (const QString& msg : it.value()) {
            out << msg;

        }
    }
}


void MainWindow::statistiquesP()
{

    Connection C;
    C.createconnect();
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PODCASTS WHERE categorie='Education' ");
    float exp1=model->rowCount();
    model->setQuery("SELECT * FROM PODCASTS WHERE categorie='Sports' ");
    float exp2=model->rowCount();
    model->setQuery("SELECT * FROM PODCASTS WHERE categorie='Finance' ");
    float exp3=model->rowCount();
    model->setQuery("SELECT * FROM PODCASTS WHERE categorie='Politiques' ");
    float exp4=model->rowCount();
    float total=exp1+exp2+exp3+exp4;
    QString a=QString("Education "+ QString::number((exp1*100)/total,'f',2)+"%");
    QString b=QString("Sports "+ QString::number((exp2*100)/total,'f',2)+"%");
    QString c=QString("Finance "+ QString::number((exp3*100)/total,'f',2)+"%");
    QString d=QString("Politiques "+ QString::number((exp4*100)/total,'f',2)+"%");
    QPieSeries *series = new QPieSeries();
        series->setHoleSize(0.3);
        series->append(a, exp1);
        series->append(b, exp2);
        series->append(c, exp3);
        series->append(d, exp4);
        if (exp1!=0)
        {
           QPieSlice *slice1 = series->slices().at(0);
           //slice1->setExploded();
           slice1->setLabelVisible();
        }
        if (exp2!=0)
        {
            QPieSlice *slice2 = series->slices().at(1);
            slice2->setLabelVisible();
        }
        if (exp3!=0)
        {
            QPieSlice *slice3 = series->slices().at(2);
            slice3->setExploded();
            slice3->setLabelVisible();
        }
        if (exp4!=0)
        {
            QPieSlice *slice4 = series->slices().at(3);
            slice4->setLabelVisible();
        }


        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->setTitle("Répartition des podcasts selon les catégories");
        chart->setTheme(QChart::ChartThemeBlueCerulean);

        QChartView *chartview = new QChartView(chart);
        chartview->setRenderHint(QPainter::Antialiasing);

        // Set the geometry of the chartview to make it smaller
        chartview->setGeometry(10, 10, 500, 400);

        // Set up the chart view
        chartview = new QChartView(chart);
        chartview->setRenderHint(QPainter::Antialiasing);
        chartview->setParent(ui->statP);
        chartview->setGeometry((ui->statP->width() - 1000) / 2,
                               (ui->statP->height() - 400) / 2,
                               1000, 400);


}



void MainWindow::on_pod_chatbox_2_clicked()
{
    QString recipient = ui->pod_services_2->currentText();
    QString message = ui->pod_msg_3->text();
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString LocalMessage = "Vous: " + message;
    QString DestMessage = "Responsable POD: " + message;

    LocalMessage += QString("%1%2").arg(QString(110 - LocalMessage.length(), ' '), currentTime);
    DestMessage += QString("%1%2").arg(QString(90 - DestMessage.length(), ' '), currentTime);

    if(recipient == "Responsable Sponsoring")
    {
        ui->pod_chat_2->addItem(LocalMessage);
        ui->spons_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Trésorier")
    {
        ui->pod_chat_2->addItem(LocalMessage);
        ui->treso_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Invités")
    {
        ui->pod_chat_2->addItem(LocalMessage);
        ui->invite_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Logistique")
    {
        ui->pod_chat_2->addItem(LocalMessage);
        ui->logist_chat_3->addItem(DestMessage);
    }
    else if(recipient == "Responsable Employes")
    {
        ui->pod_chat_2->addItem(LocalMessage);
        ui->emp_chat_2->addItem(DestMessage);
    }

    ui->pod_msg_3->clear();
}

void MainWindow::on_spons_chatbox_2_clicked()
{
    QString recipient = ui->spons_services_2->currentText();
    QString message = ui->spons_msg_3->text();
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString LocalMessage = "Vous: " + message;
    QString DestMessage = "Responsable Sponsoring: " + message;

    LocalMessage += QString("%1%2").arg(QString(110 - LocalMessage.length(), ' '), currentTime);
    DestMessage += QString("%1%2").arg(QString(90 - DestMessage.length(), ' '), currentTime);

    if(recipient == "Responsable POD")
    {
        ui->spons_chat_2->addItem(DestMessage);
        ui->pod_chat_2->addItem(LocalMessage);
    }
    else if(recipient == "Responsable Trésorier")
    {
        ui->spons_chat_2->addItem(LocalMessage);
        ui->treso_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Invités")
    {
        ui->spons_chat_2->addItem(LocalMessage);
        ui->invite_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Logistique")
    {
        ui->spons_chat_2->addItem(LocalMessage);
        ui->logist_chat_3->addItem(DestMessage);
    }
    else if(recipient == "Responsable Employes")
    {
        ui->spons_chat_2->addItem(LocalMessage);
        ui->emp_chat_2->addItem(DestMessage);
    }

    ui->spons_msg_3->clear();
}


void MainWindow::on_tres_chatbox_2_clicked()
{
    QString recipient = ui->treso_services_2->currentText();
    QString message = ui->treso_msg_3->text();
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString LocalMessage = "Vous: " + message;
    QString DestMessage = "Responsable Trésorier: " + message;

    LocalMessage += QString("%1%2").arg(QString(110 - LocalMessage.length(), ' '), currentTime);
    DestMessage += QString("%1%2").arg(QString(90 - DestMessage.length(), ' '), currentTime);

    if(recipient == "Responsable POD")
    {
        ui->treso_chat_2->addItem(DestMessage);
        ui->pod_chat_2->addItem(LocalMessage);
    }
    if(recipient == "Responsable Sponsoring")
    {
        ui->treso_chat_2->addItem(LocalMessage);
        ui->spons_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Invités")
    {
        ui->treso_chat_2->addItem(LocalMessage);
        ui->invite_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Logistique")
    {
        ui->treso_chat_2->addItem(LocalMessage);
        ui->logist_chat_3->addItem(DestMessage);
    }
    else if(recipient == "Responsable Employes")
    {
        ui->treso_chat_2->addItem(LocalMessage);
        ui->emp_chat_2->addItem(DestMessage);
    }

    ui->treso_msg_3->clear();
}

void MainWindow::on_invite_chatbox_2_clicked()
{
    QString recipient = ui->invite_services_2->currentText();
    QString message = ui->invite_msg_3->text();
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString LocalMessage = "Vous: " + message;
    QString DestMessage = "Responsable Invités: " + message;

    LocalMessage += QString("%1%2").arg(QString(110 - LocalMessage.length(), ' '), currentTime);
    DestMessage += QString("%1%2").arg(QString(90 - DestMessage.length(), ' '), currentTime);

    if(recipient == "Responsable POD")
    {
        ui->invite_chat_2->addItem(DestMessage);
        ui->pod_chat_2->addItem(LocalMessage);
    }
    if(recipient == "Responsable Sponsoring")
    {
        ui->invite_chat_2->addItem(LocalMessage);
        ui->spons_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Trésorier")
    {
        ui->invite_chat_2->addItem(LocalMessage);
        ui->treso_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Logistique")
    {
        ui->invite_chat_2->addItem(LocalMessage);
        ui->logist_chat_3->addItem(DestMessage);
    }
    else if(recipient == "Responsable Employes")
    {
        ui->invite_chat_2->addItem(LocalMessage);
        ui->emp_chat_2->addItem(DestMessage);
    }


    ui->invite_msg_3->clear();
}

void MainWindow::on_logist_chatbox_3_clicked()
{
    QString recipient = ui->logist_services_3->currentText();
    QString message = ui->logist_msg_4->text();
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString LocalMessage = "Vous: " + message;
    QString DestMessage = "Responsable Logistique: " + message;

    LocalMessage += QString("%1%2").arg(QString(110 - LocalMessage.length(), ' '), currentTime);
    DestMessage += QString("%1%2").arg(QString(90 - DestMessage.length(), ' '), currentTime);

    if(recipient == "Responsable POD")
    {
        ui->logist_chat_3->addItem(DestMessage);
        ui->pod_chat_2->addItem(LocalMessage);
    }
    if(recipient == "Responsable Sponsoring")
    {
        ui->logist_chat_3->addItem(LocalMessage);
        ui->spons_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Trésorier")
    {
        ui->logist_chat_3->addItem(LocalMessage);
        ui->treso_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Invités")
    {
        ui->logist_chat_3->addItem(LocalMessage);
        ui->invite_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Employes")
    {
        ui->logist_chat_3->addItem(LocalMessage);
        ui->emp_chat_2->addItem(DestMessage);
    }

    ui->logist_msg_4->clear();
}

void MainWindow::on_emp_chatbox_2_clicked()
{
    QString recipient = ui->emp_services_2->currentText();
    QString message = ui->emp_msg_3->text();
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString LocalMessage = "Vous: " + message;
    QString DestMessage = "Responsable Employes: " + message;

    LocalMessage += QString("%1%2").arg(QString(110 - LocalMessage.length(), ' '), currentTime);
    DestMessage += QString("%1%2").arg(QString(90 - DestMessage.length(), ' '), currentTime);

    if(recipient == "Responsable POD")
    {
        ui->emp_chat_2->addItem(DestMessage);
        ui->pod_chat_2->addItem(LocalMessage);
    }
    else if(recipient == "Responsable Sponsoring")
    {
        ui->emp_chat_2->addItem(LocalMessage);
        ui->spons_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Trésorier")
    {
        ui->emp_chat_2->addItem(LocalMessage);
        ui->treso_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Invités")
    {
        ui->emp_chat_2->addItem(LocalMessage);
        ui->invite_chat_2->addItem(DestMessage);
    }
    else if(recipient == "Responsable Logistique")
    {
        ui->emp_chat_2->addItem(LocalMessage);
        ui->logist_chat_3->addItem(DestMessage);
    }

    ui->emp_msg_3->clear();
}



//-------------------------------------------------------DESCTRUCTEUR MAINWINDOW-----------------------------------------------------------------------------------//
MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------//

void MainWindow::on_connexionAvecFaceIDPushButton_clicked()
{
    QProcess process;
    QString pythonPath = "C:/Users/user/Desktop/FACE RECOGNITION/main.py"; // Chemin complet vers le script Python
    qDebug() << "Lancement du script Python depuis l'application Qt...";
    qDebug() << "Chemin du script Python :" << pythonPath;
    // Démarrer le script Python
    process.start("python", QStringList() << pythonPath);

    // Attendre que le processus se termine
    if (!process.waitForFinished()) {
        qDebug() << "Erreur : Le processus n'a pas pu être terminé correctement:" << process.errorString();
        return;
    }

    // Lecture de la sortie standard du processus Python
    QByteArray output = process.readAllStandardOutput();

    // Convertir la sortie en QString
    QString outputStr = QString::fromUtf8(output);

    // Trouver la ligne contenant la valeur de retour
    QStringList lines = outputStr.split("\n");
    int userId = 0; // Initialiser userId à 0 par défaut
    for (const QString& line : lines) {
        if (line.startsWith("Returning code:")) {
            userId = line.mid(15).trimmed().toInt(); // Extraire la partie après "Returning code:"
            break; // Sortir de la boucle dès que la ligne est trouvée
        }
    }
    switch (userId)
    {
        case 1: // Employé RH
            ui->tabWidget->setCurrentIndex(1);
            ui->tabWidget->setTabEnabled(1, true);
            userId=-1;
            break;
        case 2:
            ui->tabWidget->setCurrentIndex(2);
            ui->tabWidget->setTabEnabled(2, true);
            break;
        case 3:
            ui->tabWidget->setCurrentIndex(3);
            ui->tabWidget->setTabEnabled(3, true);
            break;
        case 4:
            ui->tabWidget->setCurrentIndex(4);
            ui->tabWidget->setTabEnabled(4, true);
            break;
        case 5:
            ui->tabWidget->setCurrentIndex(5);
            ui->tabWidget->setTabEnabled(5, true);
            break;
        case 6:
            ui->tabWidget->setCurrentIndex(6);
            ui->tabWidget->setTabEnabled(6, true);
            break;
        case 0:
            ui->tabWidget->setCurrentIndex(0);
            ui->tabWidget->setTabEnabled(0, true);
            break;
        default:
            break;
    }
    // Désactivez les onglets inutiles
    for (int i = 0; i < 7; i++) {
        if ( ui->tabWidget->currentIndex()==7)
        {
              ui->tabWidget->setTabEnabled(i, false);

        }
        if (i != ui->tabWidget->currentIndex()) {
            ui->tabWidget->setTabEnabled(i, false);
        }
    }

}





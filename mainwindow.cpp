#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_gif_animation->setVisible(true);

    // Spécifiez le chemin absolu complet du fichier GIF
    QString gifPath = "C:/Users/user/Desktop/Login/animation logo version finale.gif";

    // Créez le QMovie en utilisant le chemin absolu complet
    QMovie *gifanimation = new QMovie(gifPath);

    // Définissez le QMovie sur le QLabel
    ui->label_gif_animation->setMovie(gifanimation);

    // Démarrez l'animation
    gifanimation->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


#ifndef SPONSOR_H
#define SPONSOR_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>



class Sponsor
{

public:
    //constructours
    Sponsor();
    Sponsor(int,QString,QString,QString,QString,QString,QString);

    //setters
    void Setid_sponsor(int);
    void Setnom(QString );
    void Setbudget(QString );
    void Setpack(QString );
    void Setdate_deb(QString );
    void Setdate_fin(QString );
    void Settelephone(QString );

    //getters
    int Getid_sponsor();
    QString Getnom();
    QString Getbudget();
    QString Getpack();
    QString Getdate_deb();
    QString Getdate_fin();
    QString Gettelephone();

    //fonctionnet crud
    bool ajouter();
    void afficher(QTableWidget *tableWidget);
    bool supprimer(int);
    bool modifier(int id_sponsor, const QString &nom, const QString &budget, const QString &pack, const QString &date_deb, const QString &date_fin, const QString &telephone);


private:

    int id_sponsor;
    QString nom;
    QString budget;
    QString pack;
    QString date_deb;
    QString date_fin;
    QString telephone;
};
#endif // SPONSOR_H

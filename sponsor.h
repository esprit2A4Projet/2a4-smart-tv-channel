#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

using namespace std;


class Sponsor
{

public:
    //constructours
    Sponsor();
    Sponsor(QString,QString,QString,QString,QString);

    int Getid_sponsor();
    void Setid_sponsor(int);
    QString Getnom();
    void Setnom(QString );
    QString Getbudget();
    void Setbudget(QString );
    QString Getpack();
    void Setpack(QString );
    QString Getdate_deb();
    void Setdate_deb(QString );
    QString Getdate_fin();
    void Setdate_fin(QString );
    QString Gettelephone();
    void Settelephone(QString );

    //fonctionnet crud
    bool ajouter();
    QSqlQueryModel  * afficher();
    bool supprimer(int);



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

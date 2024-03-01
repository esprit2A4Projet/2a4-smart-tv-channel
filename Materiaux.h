#ifndef MATERIAUX_H
#define MATERIAUX_H
#include <QString>
#include<QSqlQuery>
#include<QSqlQueryModel>

class Materiaux
{
public:
    void setid(QString n);
    void setnom(QString n);
    void settype(QString n);
    void setetat(QString n);
    void setquantite(QString n);
    void setdate(QString n);

    QString get_nom();
    QString get_type();
    QString get_etat();
    QString get_quantite();
    QString get_date();
    //int get_id();
    Materiaux();
    Materiaux(QString nom, QString type, QString etat, QString quantite, QString date);

    //Fonctionnalitéq de Base relatives à l'entité Materiaux
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(const QString nom);
private:
    //int id;
    QString nom,type,etat,quantite,date;
};

#endif // MATERIAUX_H

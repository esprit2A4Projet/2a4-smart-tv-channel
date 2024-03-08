#ifndef MATERIAUX_H
#define MATERIAUX_H
#include <QString>
#include<QSqlQuery>
#include<QSqlQueryModel>

class Materiaux
{
public:
    void setid(int n);
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
    int get_id();
    Materiaux();
    Materiaux(int id,QString nom, QString type, QString etat, QString quantite, QString date);

    //Fonctionnalitéq de Base relatives à l'entité Materiaux
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int id);
    bool update(int id, const QString& nom, const QString& type, const QString& etat, const QString& quantite, const QString& date);
    //bool update(int id);
    //void rechercher(QTableView * table,QString );

private:
    int id;
    QString nom,type,etat,quantite,date;
};

#endif // MATERIAUX_H

//mateiraux.h
#ifndef MATERIAUX_H
#define MATERIAUX_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>

class Materiaux
{
public:
    void set_id(QString n);
    //constructeurs
    Materiaux();
    Materiaux(int,QString,QString,QString,QString,QString);

    //setters
    void set_id(int n);
    void set_nom(QString n);
    void set_type(QString n);
    void set_etat(QString n);
    void set_quantite(QString n);
    void set_date(QString n);
    bool ajouter();
    bool supprimer(int);
    QSqlQueryModel* afficher();


    //getters
    int get_id();
    QString get_nom();
    QString get_type();
    QString get_etat();
    QString get_quantite();
    QString get_date();
    Materiaux(QString,QString,QString,QString,QString);

    //CRUD
    bool ajouterMateriaux();
    void afficherMateriaux(QTableWidget *tableWidget);
    bool supprimerMateriaux(int);
    bool modifierMateriaux(int id, const QString &nom, const QString &type, const QString &etat, const QString &quantite, const QString &date);

    //controle de saisie
    bool montantIsNumeric();
    bool isValidModeDePaiement();
    bool isValidDateMateriaux();
    bool isValidCategorie();
    bool isValidType();

private:

    int id;
    QString nom,type,etat,quantite,date;


};
#endif // MATERIAUX_H

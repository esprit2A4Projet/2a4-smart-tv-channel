#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>

class Transaction
{
public:
    //constructeurs
    Transaction();
    Transaction(int,QString,QString,QString,QString,QString);

    //setters
    void set_id(int n);
    void set_modeDePaiement(QString n);
    void set_type(QString n);
    void set_categorie(QString n);
    void set_montant(QString n);
    void set_dateTransaction(QString n);

    //getters
    int get_id();
    QString get_modeDePaiement();
    QString get_type();
    QString get_categorie();
    QString get_montant();
    QString get_dateTransaction();

    //CRUD
    bool ajouterTransaction();
    void afficherTransaction(QTableWidget *tableWidget);
    bool supprimerTransaction(int);
    bool modifierTransaction(int id, const QString &modeDePaiement, const QString &type, const QString &categorie, const QString &dateTransaction, const QString &montant);

    //controle de saisie
    bool montantIsNumeric();
    bool isValidModeDePaiement();
    bool isValidDateTransaction();
    bool isValidCategorie();
    bool isValidType();

private:

    int id;
    QString modeDePaiement,type,categorie,dateTransaction,montant;


};

#endif // TRANSACTION_H

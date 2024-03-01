#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <QString>
#include <QSqlQueryModel>

class Transaction
{
public:
    void set_id(QString n);
    void set_modeDePaiement(QString n);
    void set_type(QString n);
    void set_categorie(QString n);
    void set_montant(QString n);
    void set_dateTransaction(QString n);
    bool ajouter();
    bool supprimer(int);
    QSqlQueryModel* afficher();

    QString get_id();
    QString get_modeDePaiement();
    QString get_type();
    QString get_categorie();
    QString get_montant();
    QString get_dateTransaction();
    Transaction();
    Transaction(QString,QString,QString,QString,QString);
private:
    QString id,modeDePaiement,type,categorie,montant,dateTransaction;


};

#endif // TRANSACTION_H

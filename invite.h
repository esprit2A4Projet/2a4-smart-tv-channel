#ifndef INVITE_H
#define INVITE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>

class Invite
{
public:
    //constructeurs
    Invite();
    Invite(int,QString,QString,QString,QString,QString,QString);

    //setters
    void setnom(QString n);
    void setprenom(QString n);
    void setprofession(QString n);
    void setemail(QString n);
    void setnbAbonnes(QString n);
    void setnbParticipation(QString n);

    //getters
    QString get_nom();
    QString get_prenom();
    QString get_profession();
    QString get_email();
    QString get_nbAbonnes();
    QString get_nbParticipation();

    //CRUD
    bool ajouterInvite();
    void afficherInvite(QTableWidget *tableWidget);
    bool supprimerInvite(int);
    bool modifierInvite(int id, const QString &nom, const QString &prenom, const QString &profession, const QString &email, const QString &nbAbonnes, const QString &nbParticipation);

private:
    int id;
    QString nom,prenom,profession,email,nbAbonnes,nbParticipation;
};

#endif // INVITE_H

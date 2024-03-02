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

    void setnom(QString n){nom=n;}
    void setprenom(QString n){prenom=n;}
    void setprofession(QString n){profession=n;}
    void setemail(QString n){email=n;}
    void setnbAbonnes(QString n){nbAbonnes=n;}
    void setnbParticipation(QString n){nbParticipation=n;}


    QString get_nom(){return nom;}
    QString get_prenom(){return prenom;}
    QString get_profession(){return profession;}
    QString get_email(){return email;}
    QString get_nbAbonnes(){return nbAbonnes;}
    QString get_nbParticipation(){return nbParticipation;}

    //CRUD
    bool ajouterInvite();
    void afficherInvite(QTableWidget *tableWidget);
    bool supprimerInvite(int);
    bool modifierInvite(int id, const QString &nom, const QString &prenom, const QString &profession, const QString &email, const QString &nbAbonnes, const QString &nbParticipation);

    //Fonctionnalites
    void rechercherInvite(const QString &searchTerm, QTableWidget *tableWidget);
private:
    int id;
    QString nom,prenom,profession,email,nbAbonnes,nbParticipation;
};

#endif // INVITE_H

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
    Invite(int,QString,QString,QString,QString,int,int);

    //setters
    void setnom(QString n){nom=n;}
    void setprenom(QString n){prenom=n;}
    void setprofession(QString n){profession=n;}
    void setemail(QString n){email=n;}
    void setnbAbonnes(int n){nbAbonnes=n;}
    void setnbParticipation(int n){nbParticipation=n;}

    //getters
    QString get_nom(){return nom;}
    QString get_prenom(){return prenom;}
    QString get_profession(){return profession;}
    QString get_email(){return email;}
    int get_nbAbonnes(){return nbAbonnes;}
    int get_nbParticipation(){return nbParticipation;}

    //CRUD
    bool ajouterInvite();
    void afficherInvite(QTableWidget *tableWidget);
    bool supprimerInvite(int);
    bool modifierInvite(int id, const QString &nom, const QString &prenom, const QString &profession, const QString &email, int &nbAbonnes, int &nbParticipation);

    //Fonctionnalites
    void rechercherInvite(const QString &searchTerm, QTableWidget *tableWidget);
    bool trierParNbAbonnes(QTableWidget *tableWidget);
    bool exportToPDF(const QString &filePath, QTableWidget *tableWidget);

private:
    int id,nbAbonnes,nbParticipation;
    QString nom,prenom,profession,email;
};

#endif // INVITE_H

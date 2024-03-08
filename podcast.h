#ifndef PODCAST_H
#define PODCAST_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class podcast
{
private:

 int id;
 QString nom,duree,lieu,categorie,date_pod;

public:
    podcast();
    podcast(QString,QString,QString,QString,QString);

    int get_id();
    QString get_nom();
    QString get_duree();
    QString get_lieu();
    QString get_categorie();
    QString get_date_pod();

    void setid(int n);
    void setnom(QString n);
    void setduree(QString n);
    void setlieu(QString n);
    void setcategorie(QString n);
    void setdate_pod(QString n);

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int );
    //bool update(int);
    bool update(int id, const QString& nom, const QString& duree, const QString& lieu, const QString& categorie, const QString& date_pod);
    QSqlQueryModel* Rechercher(int id);
    QSqlQueryModel* tri();

};

#endif // PODCAST_H

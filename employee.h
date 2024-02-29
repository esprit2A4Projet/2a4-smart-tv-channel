#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Employee
{
private:

 int id,salaire;
 QString nom,prenom,date_embauche,poste;

public:
 Employee();
 Employee(QString,QString,QString,int,QString);

int get_id();
QString get_nom();
QString get_prenom();
QString get_date_embauche();
int get_salaire();
QString get_poste();

void setnom(QString n);
void setprenom(QString n);
void setdate_embauche(QString n);
void setsalaire(int n);
void setposte(QString n);

bool ajouter();
QSqlQueryModel * afficher();
bool supprimer(int );
bool update(int);



};

#endif // EMPLOYEE_H

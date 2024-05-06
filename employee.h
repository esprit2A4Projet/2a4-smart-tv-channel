#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QChartView>
#include <QtCharts>
#include <QImage>
#include <QPixmap>
//#include <QZXing>
class Employee
{
private:

 int id,salaire,cin;
 QString nom,prenom,date_embauche,poste,email,mot_de_passe,RFIDTag;

public:
 Employee();
 Employee(QString a,QString b,QString c,QString d,int e,QString f,int g, QString ref);

int get_id();
QString get_nom();
QString get_prenom();
QString get_date_embauche();
int get_salaire();
QString get_poste();
int get_cin();
QString get_mot_de_passe();
QString get_email();
void setnom(QString n);
void setprenom(QString n);
void setdate_embauche(QString n);
void setsalaire(int n);
void setposte(QString n);
void setcin(int n);
void setemail(QString n);
void setmot_de_passe(QString n);
bool ajouter(QString);
QSqlQueryModel * afficher();
bool supprimer(int );
bool update(int id);
QSqlQueryModel* Rechercher(int );
QSqlQueryModel* tri();
void setRFIDTag(QString ref);
QString get_RFIDTag();

void genererPDFact();
QChartView * stat();
QPixmap generateQRCode() const;
static bool isCinUnique(int cin);
bool verifierCIN(int cin) ;




};

#endif // EMPLOYEE_H

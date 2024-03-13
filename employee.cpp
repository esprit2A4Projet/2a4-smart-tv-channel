#include "employee.h"
#include "connection.h"
#include <QDebug>
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextDocument>
Employee::Employee()
{
id=0;
nom="";
prenom="";
date_embauche="";
salaire=0;
poste="";
cin=0;
email="";
mot_de_passe="";
}

Employee::Employee(QString a,QString b,QString c,QString d,int e,QString f,int g)
{
    nom=a;
    prenom=b;
    email=c;
    date_embauche=d;
    salaire=e;
    poste=f;
    cin=g;


}

void Employee::setnom(QString n){nom=n;}
void Employee::setprenom(QString n){prenom=n;}
void Employee::setdate_embauche(QString n){date_embauche=n;}
void Employee::setsalaire(int n){salaire=n;}
void Employee::setposte(QString n){poste=n;}
void Employee::setcin(int n){cin=n;}
void Employee::setemail(QString n){email=n;}
int Employee::get_id(){return id;}
QString Employee::get_nom(){return nom;}
QString Employee::get_prenom(){return prenom;}
QString Employee::get_date_embauche(){return date_embauche;}
int Employee::get_salaire(){return salaire;}
QString Employee::get_poste(){return poste;}
int Employee::get_cin(){return cin;}
QString Employee::get_email(){return email;}
bool Employee::ajouter()
{
       QSqlQuery query;

             query.prepare("INSERT INTO EMPLOYES (nom, prenom,email, date_embauche,salaire,poste,cin) "
                        "VALUES (:nom, :prenom,:email, :date_embauche,:salaire,:poste,:cin)");
             query.bindValue(0,nom);
             query.bindValue(1,prenom);
             query.bindValue(2,email);
             query.bindValue(3,date_embauche);
             query.bindValue(4,salaire);
             query.bindValue(5,poste);
             query.bindValue(6,cin);


        return query.exec();
}


QSqlQueryModel* Employee::afficher()
{
   QSqlQueryModel* model=new QSqlQueryModel();

         model->setQuery("SELECT nom, prenom,email, date_embauche,salaire,poste,cin FROM EMPLOYES");
         model->setHeaderData(0, Qt::Horizontal, QObject:: tr("Nom"));
         model->setHeaderData(1, Qt::Horizontal, QObject:: tr("Prénom"));
         model->setHeaderData(2, Qt::Horizontal, QObject:: tr("email"));
         model->setHeaderData(3, Qt::Horizontal, QObject:: tr("Date d'embauche"));
         model->setHeaderData(4, Qt::Horizontal, QObject:: tr("Salaire"));
         model->setHeaderData(5, Qt::Horizontal, QObject:: tr("Poste"));
         model->setHeaderData(6, Qt::Horizontal, QObject:: tr("cin"));

 return model;
}


bool Employee::supprimer(int id)
{

       QSqlQuery query;
       query.prepare("Delete from EMPLOYES where cin=:id");
       query.bindValue(0, id);
       return query.exec();

}




















bool Employee::update(int id)
{
    QSqlQuery query;
    // Exécuter la mise à jour avec les nouvelles valeurs des attributs de l'employé
    query.prepare("UPDATE EMPLOYES SET nom=:nom, prenom=:prenom, email=:email,date_embauche=:date_embauche, salaire=:salaire, poste=:poste,cin=:cin_new WHERE cin=:cin_old");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":salaire", salaire);
    query.bindValue(":poste", poste);
    query.bindValue(":cin_new", cin);
    query.bindValue(":cin_old", id);
    if (!query.exec()) {
            // Handle query execution errors
            QMessageBox msgBox;
            msgBox.setText("Échec de la mise à jour. Vérifiez la connexion à la base de données ou les valeurs saisies.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return false;
        }

        return true;
    if (!query.exec())
    {
        qDebug() << "Update operation failed.";
        return false; // Return false if the update operation fails
    }
else
    {
    return true; // Return true if the update operation is successful
    }
}









QSqlQueryModel* Employee::Rechercher(int id)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;

    // Préparer la requête SQL avec un paramètre lié à l'ID
    query.prepare("SELECT * FROM EMPLOYES WHERE cin = :id");
    query.bindValue(":id", id); // Lier la valeur de l'ID à la requête

    // Exécuter la requête et vérifier si elle a réussi
    if (query.exec()) {
        // Si la requête réussit, associer le modèle à la requête
        model->setQuery(query);
    } else {
        // Si la requête échoue, afficher un message d'erreur et détruire le modèle
        qDebug() << "Erreur lors de l'exécution de la requête SQL:" << query.lastError().text();
        delete model;
        model = nullptr;
    }

    return model;
}


QSqlQueryModel* Employee::tri()
{
   QSqlQueryModel * model=new QSqlQueryModel();
   model->setQuery("SELECT * FROM EMPLOYES ORDER BY cin ASC ");


   model->setHeaderData(0,Qt::Horizontal,QObject::tr("nom"));
   model->setHeaderData(1,Qt::Horizontal,QObject::tr("prenom"));
   model->setHeaderData(2,Qt::Horizontal,QObject::tr("email"));
   model->setHeaderData(3,Qt::Horizontal,QObject::tr("date d'embauche"));
   model->setHeaderData(4,Qt::Horizontal,QObject::tr("salaire"));
   model->setHeaderData(5,Qt::Horizontal,QObject::tr("poste"));
   model->setHeaderData(5,Qt::Horizontal,QObject::tr("cin"));


   return  model;

}



void Employee::genererPDFact()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
           if (QFileInfo(fileName).suffix().isEmpty())
               { fileName.append(".pdf"); }

           QPrinter printer(QPrinter::PrinterResolution);
           printer.setOutputFormat(QPrinter::PdfFormat);
           printer.setPaperSize(QPrinter::A4);
           printer.setOutputFileName(fileName);

           QTextDocument doc;
           QSqlQuery q;
           q.prepare("SELECT * FROM EMPLOYES ");
           q.exec();
           QString pdf="<br> <h1  style='color:blue'>LISTE DES EMPLOYES  <br></h1>\n <br> <table>  <tr>  <th>ID </th> <th>Nom </th> <th>Prenom  </th><th>email  </th> <th>date d'embauche  </th><th>SALAIRE  </th><th>poste</th><th>cin  </th> </tr>" ;
       //br traja ll star oel tr tzidlek colonne th tzidlek ligne h1 asghrr size o akbr size h6 //

           while ( q.next())
               {

               pdf= pdf+ " <br> <tr> <td>"+ q.value(0).toString()+" " + q.value(1).toString() +"</td>   <td>" +q.value(2).toString() +" <td>" +q.value(3).toString() +" <td>" +q.value(4).toString() +" <td>" +q.value(5).toString() +" "" " "</td> </td>" ;
           } while (q.next()) {
               pdf += "<tr><td>" + q.value(0).toString() + "</td>";
               pdf += "<td>" + q.value(1).toString() + "</td>";
               pdf += "<td>" + q.value(2).toString() + "</td>";
               pdf += "<td>" + q.value(3).toString() + "</td>";
               pdf += "<td>" + q.value(4).toString() + "</td>";
               pdf += "<td>" + q.value(5).toString() + "</td>";
               pdf += "<td>" + q.value(6).toString() + "</td>";  // Include cin column
               pdf += "<td>" + q.value(7).toString() + "</td>";
               pdf += "</tr>";
           }

           pdf += "</table>";
           doc.setHtml(pdf);
           doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
           doc.print(&printer);


}

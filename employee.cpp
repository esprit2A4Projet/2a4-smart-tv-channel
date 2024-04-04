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
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
//#include <QZXing>
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

/*
QPixmap Employee::generateQRCode() const {
    // Créer une instance de QZXing
    QZXing qzxing;

    // Générer le contenu du code QR à partir des informations de l'employé
    QString content = "Nom: " + nom + "\nPrénom: " + prenom + "\nPoste: " + poste +
                      "\nDate d'embauche: " + date_embauche + "\nSalaire: " + QString::number(salaire) +
                      "\nEmail: " + email + "\nCIN: " + cin;

    // Convertir le contenu en code QR sous forme de QImage
    QImage qrImage = qzxing.encodeData(content);

    // Convertir l'image QR en QPixmap pour une utilisation plus facile dans l'interface utilisateur
    QPixmap qrPixmap = QPixmap::fromImage(qrImage);

    return qrPixmap;
}
*/

bool Employee::ajouter(QString pos)
{
       QSqlQuery query;

             query.prepare("INSERT INTO EMPLOYES (nom, prenom,email, date_embauche,salaire,poste,cin,mot_de_passe) "
                        "VALUES (:nom, :prenom,:email, :date_embauche,:salaire,:poste,:cin,:mot_de_passe)");
             query.bindValue(0,nom);
             query.bindValue(1,prenom);
             query.bindValue(2,email);
             query.bindValue(3,date_embauche);
             query.bindValue(4,salaire);
             query.bindValue(5,poste);
             query.bindValue(6,cin);

             QString motDePasse;
                if (pos == "Ressources Humaines") {
                    motDePasse = "rh2023";
                } else if (pos == "R. podcast") {
                    motDePasse = "podcast";
                } else if (pos == "R. sponsoring") {
                    motDePasse = "sponsoring";
                } else if (pos == "tresorier") {
                    motDePasse = "tresorier";
                } else if (pos == "C.Invités") {
                    motDePasse = "invite";
                } else if (pos =="R.logistique ")
                {
                    motDePasse = "logistique";
                } else if (pos == "Autre") {
                    motDePasse = "autre";
                } else {
                    motDePasse = "default";
                }
                query.bindValue(":mot_de_passe", motDePasse);

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
    query.prepare("UPDATE EMPLOYES SET nom=:nom, prenom=:prenom, email=:email,date_embauche=:date_embauche, salaire=:salaire, poste=:poste,cin=:cin_new,mot_de_passe=:mot_de_passe  WHERE cin=:cin_old");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":salaire", salaire);
    query.bindValue(":poste", poste);
    query.bindValue(":cin_new", cin);

    query.bindValue(":cin_old", id);

    QString motDePasse;
       if (poste == "Ressources Humaines") {
           motDePasse = "rh2023";
       } else if (poste == "R. podcast") {
           motDePasse = "podcast";
       } else if (poste == "R. sponsoring") {
           motDePasse = "sponsoring";
       } else if (poste == "tresorier") {
           motDePasse = "tresorier";
       } else if (poste == "C.Invites") {
           motDePasse = "invite";
       } else if (poste == "R.logistique ") {
           motDePasse = "logistique";
       } else if (poste == "Autre") {
           motDePasse = "autre";
       }
    query.bindValue(":mot_de_passe", motDePasse);
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
    query.prepare("SELECT nom, prenom,email, date_embauche,salaire,poste,cin FROM EMPLOYES WHERE cin = :id");
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
   model->setQuery("SELECT nom, prenom,email, date_embauche,salaire,poste,cin FROM EMPLOYES ORDER BY cin ASC ");


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



// ... (other Employee class members)

// Signal emitted when employee data changes
/*void Employee::dataChanged() {
  emit employeeDataChanged(); // Emit signal
}*/

// ... (implementation of stat())

/*
QChartView * Employee ::stat()
{
    int row_count = 0;
            int row_count1 = 0;

                    QSqlQuery query,query1;

                    query.prepare("SELECT * FROM employes where SALAIRE>=2000");
                    query.exec();
                    while(query.next())
                        row_count++;

                    query1.prepare("SELECT * FROM employes where SALAIRE<2000");
                    query1.exec();
                    while(query1.next())
                        row_count1++;

            QPieSeries *series = new QPieSeries();
            series->append("Des Employes ayant un salaire>=2000 dt", row_count);
            series->append("Des Employes ayant un salaire<2000 dt", row_count1);

            //pour slider les employes ayant un salaire>=2000dt
            QPieSlice *slice= series->slices().at(0);
            slice->setExploded(true);
            slice->setLabelVisible(true);
            slice->setPen((QPen(Qt::white)));

            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("STATISTIQUE DES SALAIRES");
            chart->legend()->setAlignment(Qt::AlignRight);
            chart->legend()->setBackgroundVisible(true);
            chart->legend()->setBrush(QBrush(QColor(128, 128, 128, 128)));
            chart->legend()->setPen(QPen(QColor(192, 192, 192, 192)));
            series->setLabelsVisible();

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            return chartView;

}


*/



QChartView *Employee::stat()
{
    int a1 = 0; // Number of employees with salary < 1000
    int a2 = 0; // Number of employees with salary between 1000 and 1500
    int a3 = 0; // Number of employees with salary between 1500 and 2000
    int a4 = 0; // Number of employees with salary >= 2000

    QSqlQuery query;

    query.prepare("SELECT * FROM employes WHERE SALAIRE < 1000");
    query.exec();
    while (query.next())
        a1++;

    query.prepare("SELECT * FROM employes WHERE SALAIRE >= 1000 AND SALAIRE < 1500");
    query.exec();
    while (query.next())
        a2++;

    query.prepare("SELECT * FROM employes WHERE SALAIRE >= 1500 AND SALAIRE < 2000");
    query.exec();
    while (query.next())
        a3++;

    query.prepare("SELECT * FROM employes WHERE SALAIRE >= 2000");
    query.exec();
    while (query.next())
        a4++;

    qDebug() << a1 << a2 << a3 << a4;

    QPieSeries *series = new QPieSeries();
    series->append("Salaire &lt; 1000", a1);
    series->append("Salaire 1000-1499", a2);
    series->append("Salaire 1500-1999", a3);
    series->append("Salaire >= 2000", a4);

    QPieSlice *slice1 = series->slices().at(0);
    slice1->setExploded(true);
    slice1->setColor("#CCCCCC");

    QPieSlice *slice2 = series->slices().at(1);
    slice2->setColor("#4682B4");

    QPieSlice *slice3 = series->slices().at(2);
    slice3->setColor("#D1D0FB");

    QPieSlice *slice4 = series->slices().at(3);
    slice4->setColor("#041041");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistique des salaires des employés");

    series->setLabelsVisible();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setAnimationOptions(QChart::AllAnimations);
    chartView->chart()->legend()->hide();
    chartView->resize(1000, 500);

    return chartView;
}

/*
QChartView *Employee::stat() {

    // Initialize the pie series
       QPieSeries *series = new QPieSeries();

       // Open the database connection
       QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
       db.setDatabaseName("your_database_name.sqlite");  // Replace with your database name
       if (!db.open()) {
           qDebug() << "Error: Couldn't open the database";
           return nullptr;  // Return nullptr on database error
       }
       // Perform SQL query to get employee data
          QSqlQuery query;
          query.prepare("SELECT SALAIRE FROM employes");
          if (!query.exec()) {
              qDebug() << "Error executing query:" << query.lastError().text();
              db.close();
              return nullptr;  // Return nullptr on query execution error
          }
          // Create variables to store the count of employees in each salary range
             int salaryLessThan1000 = 0;
             int salary1000To1499 = 0;
             int salary1500To1999 = 0;
             int salary2000AndAbove = 0;

             // Loop through query results and count employees in each salary range
                while (query.next()) {
                    int salary = query.value(0).toInt();
                    if (salary < 1000) {
                        salaryLessThan1000++;
                    } else if (salary < 1500) {
                        salary1000To1499++;
                    } else if (salary < 2000) {
                        salary1500To1999++;
                    } else {
                        salary2000AndAbove++;
                    }
                }
                // Add salary ranges and employee counts to the pie series
                   series->append("Salary < 1000", salaryLessThan1000);
                   series->append("Salary 1000-1499", salary1000To1499);
                   series->append("Salary 1500-1999", salary1500To1999);
                   series->append("Salary >= 2000", salary2000AndAbove);

                   // Create the chart
                   QChart *chart = new QChart();
                   chart->addSeries(series);
                   chart->setTitle("Employee Salary Statistics");

                   // Create the chart view and return it
                   QChartView *chartView = new QChartView(chart);
                   chartView->setRenderHint(QPainter::Antialiasing);

                   // Close the database connection
                   db.close();

                   return chartView;


}
*/
/*
bool Employee::isCinUnique(int cin) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM employees WHERE cin = :cin");
    query.bindValue(":cin", cin);

    if (query.exec() && query.first()) {
        int count = query.value(0).toInt();
        qDebug() << "Count for CIN:" << cin << "is" << count;
        return (count == 0);  // True if count is 0 (CIN is unique), false otherwise
    } else {
        qDebug() << "Error executing query for CIN:" << cin << "Error:" << query.lastError().text();
        return false;  // Return false in case of query execution error
    }
}



bool Employee::verifierCIN(int cin) {
      // Connect to the database (assuming connection is established elsewhere)
      QSqlDatabase db = QSqlDatabase::database();

      // Prepare a query to check for existing CIN
      QSqlQuery query(db);
      query.prepare("SELECT COUNT(*) FROM employees WHERE cin = :cin");
      query.bindValue(":cin", cin);

      // Execute the query and check for results
      if (!query.exec()) {
          // Handle database error (optional)
          qWarning() << "Error checking CIN in database:" << query.lastError().text();
          return false; // Or throw an exception if preferred
      }

      query.next();
      int count = query.value(0).toInt();

      return count > 0; // Return true if CIN exists, false otherwise
  }
*/



bool Employee:: isCinUnique(int cin)
{
      /*  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("employee_database.db"); // Replace with your database name

        if (!db.open()) {
            qDebug() << "Error: connection with database failed";
            return false;
        }*/

        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM employes WHERE cin = :cin");
        query.bindValue(":cin", cin);
        if (!query.exec()) {
            qDebug() << "Error executing query: " << query.lastError().text();
            return false;
        }

        query.next();
        int rowCount = query.value(0).toInt();
       // db.close();

        return rowCount == 0; // Return true if rowCount is 0 (CIN is unique)
}

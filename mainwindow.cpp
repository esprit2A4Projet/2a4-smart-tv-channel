#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include "connection.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
#include <QDialog>
#include <QPrinter>
#include <QFileDialog>
#include <QPdfWriter>
#include <QTextDocument>
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QUrl>
//#include <QUrlQuery>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget())); // Connect the signal to the slot

    // Initialize the table widget
    ui->tableWidget->setColumnCount(7);
    QStringList labels{"ID","Nom", "Budget", "Pack", "Date debut", "Date fin", "Telephone"};
    ui->tableWidget->setHorizontalHeaderLabels(labels);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    Sponsor s;
    s.Setnom(ui->lineEdit_nom->text());
    s.Setbudget(ui->lineEdit_budget->text());
    s.Setpack(ui->lineEdit_pack->text());
    s.Setdate_deb(ui->dateEdit->text());
    s.Setdate_fin(ui->dateEdit_2->text());
    s.Settelephone(ui->lineEdit_tel->text());

    if (s.Getnom().isEmpty() || s.Getbudget().isEmpty() || s.Getpack().isEmpty() ||
            s.Getdate_deb().isEmpty() || s.Getdate_fin().isEmpty() || s.Gettelephone().isEmpty()) {
            QMessageBox::critical(this, "Error", "The fields must not be empty.");
            return; // Exit the function to prevent further execution
        }
    // Input validation
        if (s.Getnom().length() > 20) {
            QMessageBox::critical(this, "Error", "Nom should not exceed 20 characters.");
            return;
        }

        bool budgetConversionOk;
        double budgetValue = s.Getbudget().toDouble(&budgetConversionOk);
        if (!budgetConversionOk || s.Getbudget().length() > 20) {
            QMessageBox::critical(this, "Error", "Budget should be a valid number.");
            return;
        }

        QStringList validPacks = {"bronze", "silver", "gold"};
        if (!validPacks.contains(s.Getpack().toLower())) {
            QMessageBox::critical(this, "Error", "Pack should be either bronze, silver, or gold.");
            return;
        }

        // Date validation
        QDate dateDeb = QDate::fromString(s.Getdate_deb(), "dd-MM-yyyy");
        QDate dateFin = QDate::fromString(s.Getdate_fin(), "dd-MM-yyyy");
        if (!dateDeb.isValid() || !dateFin.isValid() || dateFin < dateDeb) {
            QMessageBox::critical(this, "Error", "Invalid date range. Date_fin should not be before date_deb.");
            return;
        }

        // Telephone validation
        if (s.Gettelephone().length() != 8 || !s.Gettelephone().toInt()) {
            QMessageBox::critical(this, "Error", "Telephone should be composed of 8 digits.");
            return;
        }


        Connection c;
        if (c.createconnect())
        {
            if (c.insertData(s.Getnom(), s.Getbudget(), s.Getpack(), s.Getdate_deb(), s.Getdate_fin(), s.Gettelephone() ))
            {
                emit dataUpdated(); // Emit the signal after successful insertion
                QMessageBox::information(this, "Success", "Data inserted into the database.");
            }
            else
            {
                QMessageBox::critical(this, "Error", "Failed to insert data into the database.");
            }
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        }
}

void MainWindow::on_pushButton_annuler_clicked()
{

       // Update the UI or clear the input fields if needed
       ui->lineEdit_nom->clear();
       ui->lineEdit_budget->clear();
       ui->lineEdit_pack->clear();
       ui->dateEdit->clear();
       ui->dateEdit_2->clear();
       ui->lineEdit_tel->clear();

}


void MainWindow::updateTableWidget()
{
    Connection c;
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SPONSOR");

    int RowNumber = 0;
    ui->tableWidget->setRowCount(0);

    while (model->canFetchMore())
        model->fetchMore();

    for (int row = 0; row < model->rowCount(); ++row)
    {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 7; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
        RowNumber++;
    }

    delete model; // Release the memory occupied by the model
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
         int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        int idToDelete = item->text().toInt();

        Sponsor s;
        if (s.supprimer(idToDelete))
        {
            // Supprimer la ligne de l'affichage
            ui->tableWidget->removeRow(row);
            emit dataUpdated(); // Mettre à jour la vue après la suppression
            QMessageBox::information(this, "Success", "Data deleted from the database.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete data from the database.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please select a row to delete.");
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidget->item(row, 0)->text().toInt();

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidget->item(row, 1)->text();
        QString budget = ui->tableWidget->item(row, 2)->text();
        QString pack = ui->tableWidget->item(row, 3)->text();
        QString date_deb = ui->tableWidget->item(row, 4)->text();
        QString date_fin = ui->tableWidget->item(row, 5)->text();
        QString telephone = ui->tableWidget->item(row, 6)->text();

        // Appelez la fonction modifierInvite pour mettre à jour la base de données
        Sponsor s;
        if (s.modifier(idToModify, nom, budget, pack, date_deb, date_fin, telephone))
        {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(budget));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(pack));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(date_deb));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(date_fin));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(telephone));

            emit dataUpdated(); // Mettre à jour la vue après la modification
            QMessageBox::information(this, "Success", "Data modified in the database.");
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to modify data in the database.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please select a row to modify.");
    }
}

void MainWindow::on_pushButton_rechercher_clicked()
{
    bool conversionOk;
    int id_sponsor = ui->lineEdit_rechercher->text().toInt(&conversionOk);

    if (conversionOk)
    {
        // Perform the search by id_sponsor
        Sponsor s;
        s.rechercher(id_sponsor, ui->tableWidget);

    }
    else
    {
        QMessageBox::warning(this, "Error", "Please enter a valid ID for the search.");
    }
}


void MainWindow::on_pushButton_trier_clicked()
{
    Sponsor s;
    if (s.trierParPack(ui->tableWidget)) {
        QMessageBox::information(this,"Success", "Data sorted by Packs");
    } else {
        QMessageBox::critical(this,"Error", "Failed to sort data.");
    }
}

void MainWindow::on_PDF_clicked()
{
    QString strStream;
      QTextStream out(&strStream);

      const int rowCount = ui->tableWidget->rowCount();
      const int columnCount = ui->tableWidget->columnCount();

      out <<  "<html>\n"
          "<head>\n"
          "<meta Content=\"Text/html; charset=Windows-1251\">\n"
          <<  QString("<title>%1</title>\n").arg("Liste des sponsors")
          <<  "</head>\n"
          "<body bgcolor=#ffffff link=#5000A0>\n"
          "<center> <H1>Liste Des Sponsors </H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n";

      // headers
      out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
      for (int column = 0; column < columnCount; column++)
          if (!ui->tableWidget->isColumnHidden(column))
              out << QString("<th>%1</th>").arg(ui->tableWidget->horizontalHeaderItem(column)->text());
      out << "</tr></thead>\n";

      // data table
      for (int row = 0; row < rowCount; row++)
      {
          out << "<tr> <td bkcolor=0>" << row + 1 << "</td>";
          for (int column = 0; column < columnCount; column++)
          {
              if (!ui->tableWidget->isColumnHidden(column))
              {
                  QTableWidgetItem *item = ui->tableWidget->item(row, column);
                  QString data = (item) ? item->text().simplified() : QString("&nbsp;");
                  out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
              }
          }
          out << "</tr>\n";
      }
      out <<  "</table> </center>\n"
          "</body>\n"
          "</html>\n";

      QString fileName = QFileDialog::getSaveFileName((QWidget * )0, "Sauvegarder en PDF", QString(), "*.pdf");
      if (QFileInfo(fileName).suffix().isEmpty())
      {
          fileName.append(".pdf");
      }

      QPrinter printer (QPrinter::PrinterResolution);
      printer.setOutputFormat(QPrinter::PdfFormat);
      printer.setPaperSize(QPrinter::A4);
      printer.setOutputFileName(fileName);

      QTextDocument doc;
      doc.setHtml(strStream);
      doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
      doc.print(&printer);
}


/*
void sendSMS(const QString& phoneNumber, const QString& message)
{
    // Twilio Account SID, Auth Token, and Twilio phone number
    QString accountSid = "your_account_sid";
    QString authToken = "your_auth_token";
    QString twilioPhoneNumber = "your_twilio_phone_number";

    // Twilio API endpoint
    QUrl apiUrl("https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages.json");

    // Create a request
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", "Basic " + QByteArray((accountSid + ":" + authToken).toUtf8()).toBase64());

    // Prepare POST data
    QUrlQuery postData;
    postData.addQueryItem("To", phoneNumber);
    postData.addQueryItem("From", twilioPhoneNumber);
    postData.addQueryItem("Body", message);

    // Create a network manager and send the request
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    // Handle the reply
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(nullptr, "SMS Sent", "SMS sent successfully.");
        } else {
            QMessageBox::critical(nullptr, "SMS Error", "Failed to send SMS. Error: " + reply->errorString());
        }

        // Clean up
        reply->deleteLater();
    });
}*/


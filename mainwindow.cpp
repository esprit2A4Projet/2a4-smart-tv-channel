#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include "connection.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDialog>
#include <QLabel>
#include <QPrinter>
#include <QFileDialog>
#include <QPdfWriter>
#include <QTextDocument>
#include <QComboBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QObject>
#include <QTcpSocket>
#include <QSignalMapper>
#include <QOverload>
//#include <twilio-cpp/Rest.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateTableWidget_S())); // Connect the signal to the slot

    // Initialize the table widget
    ui->tableWidget_S->setColumnCount(7);
    ui->tableWidget_S->hideColumn(0);
    QStringList labels{"ID","Nom", "Budget", "Pack", "Date debut", "Date fin", "Telephone"};
    ui->tableWidget_S->setHorizontalHeaderLabels(labels);

    ui->label_NomError->setVisible(false);
    ui->label_BudgetError->setVisible(false);
    ui->label_DateError->setVisible(false);
    ui->label_Date2Error->setVisible(false);
    ui->label_TelError->setVisible(false);
    QSignalMapper *dateMapper = new QSignalMapper(this);

    connect(ui->lineEdit_nomS, &QLineEdit::textChanged, this, &MainWindow::validateNom);
    connect(ui->lineEdit_budget, &QLineEdit::textChanged, this, &MainWindow::validateBudget);
    /*connect(ui->dateEdit, &QDateEdit::dateChanged, this, &MainWindow::validateDate);
    connect(ui->dateEdit_2, &QDateEdit::dateChanged, this, &MainWindow::validateDate);*/
    connect(ui->lineEdit_tel, &QLineEdit::textChanged, this, &MainWindow::validateTelephone);

    connect(ui->dateEdit_2, &QDateEdit::dateChanged, dateMapper, [dateMapper, this](){
       dateMapper->setProperty("dateDeb", ui->dateEdit->date().toString("dd/MM/yyyy"));
       dateMapper->setProperty("dateFin", ui->dateEdit_2->date().toString("dd/MM/yyyy"));
       dateMapper->map();
    });

    /*connect(dateMapper, QOverload<>::of(&QSignalMapper::mapped), this, [this, dateMapper]() {
       validateDate(dateMapper->property("dateDeb").toString(), dateMapper->property("dateFin").toString());
    });*/




}

MainWindow::~MainWindow()
{
    delete ui;
}

//control de saisie

void MainWindow::validateNom(const QString &text) {
    QRegExp rx("^[a-zA-Z]+$");
        bool isValid = rx.exactMatch(text) && text.length() <= 20;
        ui->label_NomError->setVisible(!isValid);
        ui->label_NomError->setStyleSheet(isValid ? "" : "color: red;");
}

void MainWindow::validateBudget(const QString &text) {
    bool budgetConversionOk;
    double budgetValue = text.toDouble(&budgetConversionOk);
    bool isValid = budgetConversionOk && text.length() <= 20;
    // Add visibility and style setting for the budget error label
    ui->label_BudgetError->setVisible(!isValid);
    ui->label_BudgetError->setStyleSheet(isValid ? "" : "color: red;");

}

void MainWindow::validateDate(const QString &dateDeb, const QString &dateFin) {
    QDate startDate = QDate::fromString(dateDeb, "dd/MM/yyyy");
    QDate endDate = QDate::fromString(dateFin, "dd/MM/yyyy");

    bool isValid = startDate.isValid() && endDate.isValid() && endDate >= startDate;

    // Set visibility and style for date error labels
    ui->label_DateError->setVisible(!startDate.isValid());
    ui->label_Date2Error->setVisible(!endDate.isValid() || endDate < startDate);

}

void MainWindow::validateTelephone(const QString &text) {
    bool isValid = text.length() == 8 && text.toInt(); // Check for 8 digits and numeric characters
    ui->label_TelError->setVisible(!isValid);
    ui->label_TelError->setStyleSheet(isValid ? "" : "color: red;");

}


void MainWindow::on_pushButton_ajouterS_clicked()
{
    Sponsor s;
    s.Setnom(ui->lineEdit_nomS->text());
    s.Setbudget(ui->lineEdit_budget->text());
    s.Setpack(ui->comboBox_pack->currentText());
    s.Setdate_deb(ui->dateEdit->text());
    s.Setdate_fin(ui->dateEdit_2->text());
    s.Settelephone(ui->lineEdit_tel->text());

    /*if (s.Getnom().isEmpty() || s.Getbudget().isEmpty() || s.Getpack().isEmpty() ||
            s.Getdate_deb().isEmpty() || s.Getdate_fin().isEmpty() || s.Gettelephone().isEmpty()) {
            QMessageBox::critical(this, "Error", "The fields must not be empty.");
            return; // Exit the function to prevent further execution
        }*/


    if (ui->label_NomError->isVisible() ||
        ui->lineEdit_budget->isVisible() ||
        ui->dateEdit_2->isVisible() ||
        ui->label_DateError->isVisible() ||
        ui->label_Date2Error->isVisible() ||
        ui->label_TelError->isVisible() || s.Getnom().isEmpty() || s.Getbudget().isEmpty() || s.Getpack().isEmpty() ||
            s.Getdate_deb().isEmpty() || s.Getdate_fin().isEmpty() || s.Gettelephone().isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez vérifier les données saisies.");
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




void MainWindow::on_pushButton_annulerS_clicked()
{

       // Update the UI or clear the input fields if needed
       ui->lineEdit_nomS->clear();
       ui->lineEdit_budget->clear();
       ui->comboBox_pack->clear();
       ui->dateEdit->clear();
       ui->dateEdit_2->clear();
       ui->lineEdit_tel->clear();

}


void MainWindow::updateTableWidget_S()
{
    Connection c;
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SPONSOR");

    int RowNumber = 0;
    ui->tableWidget_S->setRowCount(0);

    while (model->canFetchMore())
        model->fetchMore();

    for (int row = 0; row < model->rowCount(); ++row)
    {
        ui->tableWidget_S->insertRow(row);
        for (int col = 0; col < 7; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget_S->setItem(row, col, item);
        }
        RowNumber++;
    }

    delete model; // Release the memory occupied by the model
}

void MainWindow::on_pushButton_supprimerS_clicked()
{
    QItemSelectionModel *select = ui->tableWidget_S->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
         int row = selectedIndexes.first().row();
        QTableWidgetItem *item = ui->tableWidget_S->item(row, 0);
        int idToDelete = item->text().toInt();

        Sponsor s;
        if (s.supprimer(idToDelete))
        {
            // Supprimer la ligne de l'affichage
            ui->tableWidget_S->removeRow(row);
            emit dataUpdated(); // Mettre à jour la vue après la suppression
            QMessageBox::information(this, "Success", "Data deleted from the database.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete data from the database.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please select a row to delete.");
    }
}

void MainWindow::on_pushButton_modifierS_clicked()
{
    QItemSelectionModel *select = ui->tableWidget_S->selectionModel();
    QModelIndexList selectedIndexes = select->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int idToModify = ui->tableWidget_S->item(row, 0)->text().toInt();

        // Obtenez les nouvelles valeurs à partir des cellules sélectionnées dans le tableau
        QString nom = ui->tableWidget_S->item(row, 1)->text();
        QString budget = ui->tableWidget_S->item(row, 2)->text();
        QString pack = ui->tableWidget_S->item(row, 3)->text();
        QString date_deb = ui->tableWidget_S->item(row, 4)->text();
        QString date_fin = ui->tableWidget_S->item(row, 5)->text();
        QString telephone = ui->tableWidget_S->item(row, 6)->text();

        // Contrôles de saisie
        if (nom.isEmpty() || budget.isEmpty() ||
                date_deb.isEmpty() || date_fin.isEmpty() || telephone.isEmpty()) {
            QMessageBox::critical(this, "Error", "Les champs ne doivent pas être vides.");
            return;
        }

        // Validation des saisies
        QRegExp rx("^[a-zA-Z]+$");
        if (nom.length() > 20 || !rx.exactMatch(nom)) {
            QMessageBox::critical(this, "Error", "Le nom ne doit pas dépasser 20 caractères et doit être alphabétique.");
            return;
        }

        bool budgetConversionOk;
        double budgetValue = budget.toDouble(&budgetConversionOk);
        if (!budgetConversionOk || budget.length() > 20) {
            QMessageBox::critical(this, "Error", "Le budget doit être un nombre valide.");
            return;
        }

        // Date validation
        QDate dateDeb = ui->dateEdit->date();
        QDate dateFin = ui->dateEdit_2->date();
        if (dateFin < dateDeb) {
            QMessageBox::critical(this, "Error", "Plage de dates non valide. Date_fin ne devrait pas l’être avant date_deb.");
            return;
        }

        // Telephone validation
        if (telephone.length() != 8 || !telephone.toInt()) {
            QMessageBox::critical(this, "Error", "Le téléphone doit être composé de 8 chiffres.");
            return;
        }

        // Appelez la fonction modifierInvite pour mettre à jour la base de données
        Sponsor s;
        if (s.modifier(idToModify, nom, budget, pack, date_deb, date_fin, telephone))
        {
            // Mettez à jour la ligne dans le tableau avec les nouvelles valeurs
            ui->tableWidget_S->setItem(row, 1, new QTableWidgetItem(nom));
            ui->tableWidget_S->setItem(row, 2, new QTableWidgetItem(budget));
            ui->tableWidget_S->setItem(row, 3, new QTableWidgetItem(pack));
            ui->tableWidget_S->setItem(row, 4, new QTableWidgetItem(date_deb));
            ui->tableWidget_S->setItem(row, 5, new QTableWidgetItem(date_fin));
            ui->tableWidget_S->setItem(row, 6, new QTableWidgetItem(telephone));

            emit dataUpdated(); // Mettre à jour la vue après la modification
            QMessageBox::information(this, "Success", "Données modifiées dans la base de données.");
        }
        else
        {
            QMessageBox::critical(this, "Error", "Échec de la modification des données dans la base de données.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Veuillez sélectionner une ligne à modifier.");
    }
}



void MainWindow::on_pushButton_reset_clicked()
{
    Sponsor s;
    s.afficher(ui->tableWidget_S);
}


void MainWindow::on_pushButton_rechercherS_clicked()
{
    bool conversionOk;
    int id_sponsor = ui->lineEdit_rechercherS->text().toInt(&conversionOk);

    if (conversionOk)
    {
        // Perform the search by id_sponsor
        Sponsor s;
        emit dataUpdated();
        s.rechercher(id_sponsor, ui->tableWidget_S);

    }
    else
    {
        QMessageBox::warning(this, "Error", "Please enter a valid ID for the search.");
    }
}


void MainWindow::on_pushButton_trierS_clicked()
{
    Sponsor s;
    if (s.trierParPack(ui->tableWidget_S)) {
        QMessageBox::information(this,"Success", "Data sorted by Packs");
    } else {
        QMessageBox::critical(this,"Error", "Failed to sort data.");
    }
}

void MainWindow::on_PDF_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableWidget_S->rowCount();
    const int columnCount = ui->tableWidget_S->columnCount();

    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           << QString("<title>%1</title>\n").arg("Liste des sponsors")
           << "</head>\n"
              "<body bgcolor=#ffffff link=#5000A0>\n"
              "<center> <H1>Liste Des Sponsors </H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 1; column < columnCount; column++)
        if (!ui->tableWidget_S->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(ui->tableWidget_S->horizontalHeaderItem(column)->text());
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr> <td bkcolor=0>" << row + 1 << "</td>";
        for (int column = 1; column < columnCount; column++)
        {
            if (!ui->tableWidget_S->isColumnHidden(column))
            {
                QTableWidgetItem *item = ui->tableWidget_S->item(row, column);
                QString data = (item) ? item->text().simplified() : QString("&nbsp;");
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out << "</table> </center>\n"
           "</body>\n"
           "</html>\n";

    QString fileName = QFileDialog::getSaveFileName((QWidget *)0, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);
}


/*void MainWindow::on_SMS_clicked(const QString& message)
{
    // Twilio Account SID, Auth Token, and Twilio phone number
    QString accountSid = "AC7e920da38070c8ccc778abcd213cb528";
    QString authToken = "a2cd619d4cbe94d226bc2d7e48c2722f";
    QString twilioPhoneNumber = "+16505499759";

    // Get selected phone number from the QTableWidget
    QTableWidgetItem *item = ui->tableWidget_S->item(ui->tableWidget_S->currentRow(), 6); // Replace 6 with the actual column index of the phone number in your table

    if (!item) {
        QMessageBox::warning(nullptr, "Phone Number Error", "Please select a row in the table.");
        return;
    }

    //QString phoneNumber = item->text();
    QString phoneNumber ="+21656623537";

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
    postData.addQueryItem("Body", "test13/03");

    // Create a network manager and send the request
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    // Handle the reply
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(nullptr, "SMS Sent", "SMS sent successfully.");
        } else {
            QMessageBox::critical(nullptr, "SMS Error", "Failed to send SMS. Error: " + reply->errorString());
        }

        // Clean up
        reply->deleteLater();
        manager->deleteLater();
    });
}

void MainWindow::on_SMS_clicked() {
    std::string accountSid = "ACa18405085019ebc311b26b8ade0edd23";
    std::string authToken = "f4842fc74fb08e3c8918b82b67bbaffc";

    // Initialize Twilio client
    twilio::Rest::Response::setAuth(accountSid, authToken);

    // Construct message options
    twilio::PhoneNumber toNumber("+21656623537");
    twilio::PhoneNumber fromNumber("+12185146708");
    std::string body = "test 14/03 00:21";
    twilio::MessageOptions messageOptions(toNumber, body);
    messageOptions.setFrom(fromNumber);

    // Send message
    twilio::MessageResource message = twilio::MessageResource::create(messageOptions);

    // Output response
    std::cout << "Message sent. SID: " << message.sid() << std::endl;
}

void MainWindow::on_SMS_clicked()
{
    QMessageBox::warning(this,"Success", "sms tenzal");
    // Twilio Account SID, Auth Token, and Twilio phone number
    QString accountSid = "ACfbf00a28b60d1857ef04da7502bc1849";
    QString authToken = "25ea7f30559ff9bd6b6c4fc4869aa4fb";
    QString twilioPhoneNumber = "+12185146708";

    // Get selected phone number from the QTableWidget
    QTableWidgetItem *item = ui->tableWidget_S->item(ui->tableWidget_S->currentRow(), 6); // Replace 6 with the actual column index of the phone number in your table

    if (!item) {
        QMessageBox::warning(nullptr, "Phone Number Error", "Please select a row in the table.");
        return;
    }

    QString phoneNumber = "+21656623537";

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
    postData.addQueryItem("Body", "message1");

    // Create a network manager and send the request
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    // Handle the reply
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(nullptr, "SMS Sent", "SMS sent successfully.");
        } else {
            QMessageBox::critical(nullptr, "SMS Error", "Failed to send SMS. Error: " + reply->errorString());
        }

        // Clean up
        reply->deleteLater();
        manager->deleteLater();
    });
}*/




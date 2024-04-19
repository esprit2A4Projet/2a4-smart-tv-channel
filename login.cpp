#include "login.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
Login::Login()
{
    id = 0;
    password = "";
}

Login::Login(int cin, QString pwd)
{
    this->id = cin;
    this->password = pwd;
}
int Login::validateLogin(int enteredUsercin, QString enteredPassword) {
    // Vérifiez les informations de connexion en recherchant l'employé correspondant au numéro de cin

    QSqlQuery query;
    query.prepare("SELECT * FROM employes WHERE cin = :cin AND mot_de_passe = :password");
    query.bindValue(":cin", enteredUsercin);
    query.bindValue(":password", enteredPassword);
    if (query.exec() && query.next())
    {
        // Si l'employé est trouvé avec le bon mot de passe, vérifiez le type de poste
        QString poste = query.value("poste").toString();
        if (poste == "Ressources Humaines")
        {
            return 1;
        }
        else if (poste == "C.Invités")
        {
            return 2;
        }
        else if (poste == "R.logistique ")
        {
            return 3;
        }
        else if (poste == "R. podcast")
        {
            return 4;
        }
        else if (poste == "tresorier")
        {
            return 5;
        }
        else if (poste == "R. sponsoring")
        {
            return 6;
        }
        else if (poste == "autre")
        {
            return 7;
        }
      /*  else if ()
        {
            return 7;
        }*/
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

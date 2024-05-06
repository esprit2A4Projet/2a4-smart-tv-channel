#ifndef LOGIN_H
#define LOGIN_H
#include <QString>

class Login
{
public:
    Login();
    Login(int, QString );
    int validateLogin(int , QString );
private:
    int id;
  QString password;
};

#endif // LOGIN_H

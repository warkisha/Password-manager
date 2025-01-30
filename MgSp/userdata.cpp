#include "userdata.h"


UserData &UserData::instance() {
    static UserData instance;
    return instance;
}

void UserData::setLogin(const QString &login) {
    this->login = login;
}

void UserData::setPassword(const QString &password) {
    this->password = password;
}

void UserData::setId(const int &id)
{
    this->id = id;
}

QString UserData::getLogin() const {
    return login;
}

QString UserData::getPassword() const {
    return password;
}

int UserData::getId() const
{
    return id;
}



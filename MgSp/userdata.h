#ifndef USERDATA_H
#define USERDATA_H

#include <QString>

class UserData {
public:
    // Метод для получения единственного экземпляра класса (Singleton)
    static UserData& instance();

    // Методы для установки и получения логина и пароля
    void setLogin(const QString &login);

    void setPassword(const QString &password);

    void setId(const int &id);

    QString getLogin() const;

    QString getPassword() const;

    int getId() const;


private:
    // Закрытый конструктор, чтобы предотвратить создание новых объектов
    UserData() {}

    // Закрытые копирующие конструкторы и операторы присваивания для обеспечения единственности экземпляра
    UserData(const UserData&) = delete;
    void operator=(const UserData&) = delete;

    // Данные пользователя
    QString login;
    QString password;
    int id;
};


#endif // USERDATA_H

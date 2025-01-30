#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QByteArray>
#include <QDebug>
#include "userdata.h"


class DatabaseManager {
public:
    DatabaseManager(const QString &dbName);
    ~DatabaseManager();

    bool open();
    void close();

    bool createUsersTable();
    bool registerUser(const QByteArray &login, const QByteArray &password);
    bool userExists(const QByteArray &login);
    bool createForUserDataTable(QString name);

    bool validateUser(const QByteArray &encryptedLogin, const QByteArray &encryptedPassword);

    void AddNewAccount(int id, QString login, QString pswd, QString service);
private:
    QSqlDatabase db;
    QString dbName;

};

#endif // DATABASEMANAGER_H

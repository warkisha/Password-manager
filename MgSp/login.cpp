#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LogIn)
{
    ui->setupUi(this);
    ui->newUser_groupBox->setVisible(false);
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::on_next_pushButton_clicked()
{
    qDebug() << "Go....";
    QString login = ui->login_lineEdit->text();
    QString password = ui->pswd_lineEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Login and password cannot be empty.");
        return;
    }

    AesEncryptionManager encryptionManager(login, password);
    QByteArray encryptedlogin = login.toUtf8();
    QByteArray encryptedpswd = password.toUtf8();
    encryptedlogin = encryptionManager.encrypt(encryptedlogin);
    encryptedpswd = encryptionManager.encrypt(encryptedpswd);

    qDebug() << "Encrypted data:" << encryptedlogin.toHex();
    qDebug() << "Encrypted data:" << encryptedpswd.toHex();
    // QByteArray decryptedData = encryptionManager.decrypt(encryptedlogin);
    // qDebug() << "Decrypted data:" << decryptedData;
    // Работа с базой данных
    DatabaseManager dbManager("users.db");

    if (!dbManager.open()) {
        QMessageBox::critical(this, "Database Error", "Unable to open database.");
        return;
    }

    if (dbManager.validateUser(encryptedlogin, encryptedpswd)) {
        QMessageBox::information(this, "Login Successful", "Login and password are correct.");
        mainWindow.show();
        UserData::instance().setLogin(login);
        UserData::instance().setPassword(password);
        close();
    } else {
        QMessageBox::warning(this, "Login Error", "Invalid login or password.");
    }


    qDebug() << "Go stop";
}


void LogIn::on_new_pushButton_clicked()
{
    ui->newUser_groupBox->setVisible(true);
    ui->Login_groupBox->setVisible(false);
}


void LogIn::on_saveUser_pushButton_clicked()
{
    qDebug() << "regist start";
    QString login = ui->newLogin_lineEdit->text();
    QString password = ui->newPswd_lineEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Login and password cannot be empty.");
        return;
    }


    AesEncryptionManager encryptionManager(login, password);
    QByteArray encryptedlogin = login.toUtf8();
    QByteArray encryptedpswd = password.toUtf8();
     encryptedlogin = encryptionManager.encrypt(encryptedlogin);
     encryptedpswd = encryptionManager.encrypt(encryptedpswd);

    qDebug() << "Encrypted data:" << encryptedlogin.toHex();
    qDebug() << "Encrypted data:" << encryptedpswd.toHex();
    // Расшифрование данных
    // QByteArray decryptedData = encryptionManager.decrypt(encryptedlogin);
    // qDebug() << "Decrypted data:" << decryptedData;

    // Создаем и открываем базу данных
    DatabaseManager dbManager("users.db");

    if (!dbManager.open()) {
        QMessageBox::critical(this, "Database Error", "Unable to open database.");
        return;
    }

    if (!dbManager.createUsersTable()) {
        QMessageBox::critical(this, "Database Error", "Unable to create users table.");
        return;
    }

    if (!dbManager.registerUser(encryptedlogin, encryptedpswd)) {
        QMessageBox::warning(this, "Registration Error", "User already exists or database error.");
    } else {
        QMessageBox::information(this, "Registration Successful", "User registered successfully!");
    }

    dbManager.close();
    ui->newUser_groupBox->setVisible(false);
    ui->Login_groupBox->setVisible(true);

    qDebug() << "regist end";
}


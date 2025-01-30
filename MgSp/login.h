#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include "databasemanager.h"
#include "AesEncryptionManager.h"
#include "mainwindow.h"
#include "userdata.h"

QT_BEGIN_NAMESPACE
namespace Ui {
   class LogIn;
}
QT_END_NAMESPACE

class LogIn : public QDialog
{
    Q_OBJECT

public:
    LogIn(QWidget *parent = nullptr);
    ~LogIn();

private slots:
    void on_next_pushButton_clicked();

    void on_new_pushButton_clicked();

    void on_saveUser_pushButton_clicked();

private:
    Ui::LogIn *ui;
    MainWindow mainWindow;

};
#endif // LOGIN_H

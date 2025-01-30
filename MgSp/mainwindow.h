#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <vector>
#include "userdata.h"
#include "databasemanager.h"
#include <QRandomGenerator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void populateAccountsList();



private slots:
    void on_listpswdOpen_pushButton_clicked();

    void on_exitPswdMg_pushButton_clicked();

    void on_addNewAc_pushButton_clicked();

    void on_gemPswd_pushButton_clicked();

    void on_genPswd_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void clearLayout(QLayout *layout);
    std::vector<QWidget*> accountWidgets;
    void removeAccountRow(QWidget *widget, int rowId, const QString &tableName);
};

#endif // MAINWINDOW_H

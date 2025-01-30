#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pswdMg_groupBox->setVisible(false);
    // std::vector<QString> accountWidgets;
     accountWidgets.clear();
    ui->gemPswd_groupBox->setVisible(false);
     connect(ui->horizontalSlider, &QSlider::valueChanged, this, [=](int value) {
         ui->sliderValueLabel->setText(QString::number(value));  // Обновление QLabel
     });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listpswdOpen_pushButton_clicked()
{
    ui->pswdMg_groupBox->setVisible(true);
    ui->menu_groupBox->setVisible(false);
    populateAccountsList();
}

void MainWindow::populateAccountsList() {
    // Очистка listAcc_verticalLayout перед добавлением новых строк
    QLayoutItem *child;
    while ((child = ui->listAcc_verticalLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    int userId = UserData::instance().getId();
    QString tableName = "user_" + QString::number(userId);
    DatabaseManager dbManager("users.db");

    if (!dbManager.open()) {
        QMessageBox::critical(this, "Database Error", "Unable to open database.");
        return;
    }

    // Выполняем SQL-запрос для получения данных
    QSqlQuery query;
    if (!query.exec("SELECT rowid, login, pswd, service FROM " + tableName)) {  // rowid для удаления
        qDebug() << "Error fetching data from table:" << query.lastError();
        return;
    }

    // Проходим по каждой строке результата запроса
    while (query.next()) {
        int rowId = query.value(0).toInt();  // Получаем rowid
        QString login = query.value(1).toString();
        QString password = query.value(2).toString();
        QString service = query.value(3).toString();

        // Создаем QHBoxLayout для каждой строки
        QHBoxLayout *accountLayout = new QHBoxLayout;

        // Создаем QLabel для login, password, и service
        QLabel *loginLabel = new QLabel(login);
        QLabel *passwordLabel = new QLabel(password);
        QLabel *serviceLabel = new QLabel(service);
        QPushButton *deleteButton = new QPushButton();
        deleteButton->setText("x");

        // Добавляем их в layout
        accountLayout->addWidget(loginLabel);
        accountLayout->addWidget(passwordLabel);
        accountLayout->addWidget(serviceLabel);
        accountLayout->addWidget(deleteButton);

        // Создаем QWidget для хранения layout
        QWidget *widget = new QWidget;
        widget->setLayout(accountLayout);
        accountWidgets.push_back(widget);  // Добавляем в std::vector

        // Добавляем layout в listAcc_verticalLayout
        ui->listAcc_verticalLayout->addWidget(widget);

        // Связываем нажатие кнопки с удалением строки и записи из базы данных
        connect(deleteButton, &QPushButton::clicked, this, [=]() {
            removeAccountRow(widget, rowId, tableName);  // Передаем rowId и название таблицы
        });
    }

    dbManager.close();
}

void MainWindow::removeAccountRow(QWidget *widget, int rowId, const QString &tableName) {
    // Удаляем виджет из listAcc_verticalLayout и очищаем память
    ui->listAcc_verticalLayout->removeWidget(widget);
    accountWidgets.erase(std::remove(accountWidgets.begin(), accountWidgets.end(), widget), accountWidgets.end());
    widget->deleteLater();  // Корректное удаление виджета

    // Открываем базу данных для удаления записи
    DatabaseManager dbManager("users.db");

    if (!dbManager.open()) {
        qDebug() << "Error: Could not open database to delete record.";
        return;
    }

    // Удаляем строку с rowId из таблицы
    QSqlQuery query;
    query.prepare("DELETE FROM " + tableName + " WHERE rowid = :rowid");
    query.bindValue(":rowid", rowId);

    if (!query.exec()) {
        qDebug() << "Error deleting row from database:" << query.lastError();
    } else {
        qDebug() << "Row with ID" << rowId << "deleted from table" << tableName;
    }

    dbManager.close();
}

void MainWindow::on_exitPswdMg_pushButton_clicked()
{
    DatabaseManager dbManager("users.db");
    ui->pswdMg_groupBox->setVisible(false);
    ui->menu_groupBox->setVisible(true);
}


void MainWindow::on_addNewAc_pushButton_clicked()
{
    QString login = ui->addNewAcLog_lineEdit->text();
    QString pswd = ui->addNewAcPswd_lineEdit->text();
    QString service = ui->addNewAcServ_lineEdit->text();
    int id = UserData::instance().getId();

    if (login.isEmpty() || pswd.isEmpty() || service.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled.");
        return;
    }


 DatabaseManager dbManager("users.db");
    // Открываем базу данных

    if (!dbManager.open()) {
        QMessageBox::critical(this, "Database Error", "Unable to open database.");
        return;
    }

    dbManager.AddNewAccount(id, login, pswd, service);


    dbManager.close();
    clearLayout(ui->listAcc_verticalLayout);
    populateAccountsList();
    ui->addNewAcLog_lineEdit->setText("");
    ui->addNewAcPswd_lineEdit->setText("");
    ui->addNewAcServ_lineEdit->setText("");
}



void MainWindow::clearLayout(QLayout *layout) {
    QLayoutItem *item;

    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget(); // Удаляем сам виджет
            qDebug() << "clear";
        }
        delete item; // Удаляем сам элемент
    }
}

void MainWindow::on_gemPswd_pushButton_clicked()
{
    ui->gemPswd_groupBox->setVisible(true);
    ui->gemPswd_pushButton->setVisible(false);
    qDebug() << "Error";
}


void MainWindow::on_genPswd_pushButton_clicked()
{
     // ui->gemPswd_groupBox->setVisible(false);
    QString possibleCharacters;
    if (ui->genPswdSym_checkBox->isChecked()) possibleCharacters += "!@#$%^&*()_-+=<>?/";
    if (ui->gemPswdNum_checkBox->isChecked()) possibleCharacters += "0123456789";
    if (ui->gemPswdLow_checkBox->isChecked()) possibleCharacters += "abcdefghijklmnopqrstuvwxyz";
    if (ui->gemPswdCap_checkBox->isChecked()) possibleCharacters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int passwordLength = ui->horizontalSlider->value();
    QString password;
    for (int i = 0; i < passwordLength; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        password.append(possibleCharacters.at(index));
    }

    ui->addNewAcPswd_lineEdit->setText(password);
    // ui->gemPswd_pushButton->setVisible(true);
}



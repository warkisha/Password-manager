#include "databasemanager.h"

DatabaseManager::DatabaseManager(const QString &dbName)
    : dbName(dbName) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
}



DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::open() {
    if (!db.open()) {
        qDebug() << "Error: Unable to open database" << dbName;
        return false;
    }
    return true;
}

void DatabaseManager::close() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::createUsersTable() {
    QSqlQuery query;
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS users ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "login TEXT NOT NULL, "
                               "pswd TEXT NOT NULL)";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table: " << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::registerUser(const QByteArray &login, const QByteArray &password) {
    if (userExists(login)) {
        qDebug() << "Error: User already exists";
        return false;
    }

    QSqlQuery query;

    // Вставка данных пользователя в таблицу "users"
    query.prepare("INSERT INTO users (login, pswd) VALUES (:login, :pswd)");
    query.bindValue(":login", login.toHex());
    query.bindValue(":pswd", password.toHex());

    if (!query.exec()) {
        qDebug() << "Error inserting data into users table: " << query.lastError().text();
        return false;
    }

    // Получаем ID вставленного пользователя
    int userId = query.lastInsertId().toInt();

    if (userId == 0) {
        qDebug() << "Error retrieving user ID after insert.";
        return false;
    }

    // Создание имени для новой таблицы на основе id пользователя
    QString tableName = "user_" + QString::number(userId);

    // Создание новой таблицы с именем, равным id пользователя
    QString createTableQuery = QString(
                                   "CREATE TABLE IF NOT EXISTS %1 ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "login TEXT NOT NULL, "
                                   "pswd TEXT NOT NULL, "
                                   "service TEXT NOT NULL)"
                                   ).arg(tableName);

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table for user: " << query.lastError().text();
        return false;
    }

    qDebug() << "Table created for user with ID:" << userId;

    return true;
}

void DatabaseManager::AddNewAccount(int id, QString login, QString pswd, QString service){

    QString tableName = "user_" + QString::number(id);
    QSqlQuery query;
    QString insertQuery = QString("INSERT INTO %1 (login, pswd, service) VALUES (:login, :pswd, :servise)").arg(tableName);

    if (!query.prepare(insertQuery)) {
        qDebug() << "Failed to prepare query:" << query.lastError();
        return;
    }

    query.bindValue(":login", login);
    query.bindValue(":pswd", pswd);
    query.bindValue(":servise", service);

    // Выполняем запрос
    if (!query.exec()) {
        qDebug() << "Failed to insert data into table:" << query.lastError();
    } else {
        qDebug() << "Account added successfully.";
    }
}

bool DatabaseManager::userExists(const QByteArray &login) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE login = :login");
    query.bindValue(":login", login.toHex());

    if (!query.exec()) {
        qDebug() << "Error checking if user exists: " << query.lastError().text();
        return false;
    }

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

bool DatabaseManager::validateUser(const QByteArray &encryptedLogin, const QByteArray &encryptedPassword) {
    // Проверяем, что база данных открыта
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return false;
    }
    // Подготавливаем SQL-запрос для поиска пользователя
    QSqlQuery query;
    if (!query.prepare("SELECT * FROM users WHERE login = :login AND pswd = :password")) {
        qDebug() << "Failed to prepare query:" << query.lastError();
        return false;
    }

    query.bindValue(":login", encryptedLogin.toHex());
    query.bindValue(":password", encryptedPassword.toHex());

    // Выполняем запрос
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError();
        return false;
    }

    // Проверяем, вернул ли запрос результаты (существует ли такой пользователь)
    if (query.next()) {
        int userId = query.value("id").toInt();
        UserData::instance().setId(userId);
        return true;
    } else {
        return false;
    }
}

bool DatabaseManager::createForUserDataTable( QString name ){
    // Создание имени для новой таблицы на основе зашифрованного имени пользователя
    QSqlQuery query;
    qDebug() << "start create for user";
    // Создание новой таблицы с зашифрованным именем пользователя
    QString createTableQuery = QString(
        "CREATE TABLE IF NOT EXISTS "+name+" ("
                                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                               "login TEXT NOT NULL, "
                                               "pswd TEXT NOT NULL, "
                                               "service TEXT NOT NULL)"
        );

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table for user: " << query.lastError().text();
        return false;
    }

    qDebug() << "Table created for user:" << name;

    return true;
}

// void DatabaseManager::readUserData(){
//     QSqlQuery query;
//     if (!query.exec("SELECT * FROM " + tableName)) {
//         qDebug() << "Error fetching data from table:" << query.lastError();
//         return;
//     }

//     // Проходим по каждой строке результата запроса
//     while (query.next()) {
//         QString login = query.value(0).toString();
//         QString password = query.value(1).toString();
//         QString service = query.value(2).toString();
//         qDebug() << "hi";
//         // Создаем новый QHBoxLayout
//         QHBoxLayout *accountLayout = new QHBoxLayout;

//         // Создаем QLabel для login, password, и service и добавляем в layout
//         QLabel *loginLabel = new QLabel(login);
//         QLabel *passwordLabel = new QLabel(password);
//         QLabel *serviceLabel = new QLabel(service);

//         accountLayout->addWidget(loginLabel);
//         accountLayout->addWidget(passwordLabel);
//         accountLayout->addWidget(serviceLabel);

//         // Добавляем созданный layout в listAcc_verticalLayout
//         ui->listAcc_verticalLayout->addLayout(accountLayout);
//     }
// }

#include "connection.h"

Connection::Connection() {
    db = QSqlDatabase::addDatabase("QODBC");
}

Connection::~Connection() {
    if (db.isOpen())
        db.close();
}

Connection& Connection::createInstance() {
    static Connection instance;
    return instance;
}

bool Connection::createConnection() {
    db.setDatabaseName("fares");   // nom de VOTRE source ODBC
    db.setUserName("fares");     // ex : fares
    db.setPassword("fares");    // ex : fares

    if (db.open()) {
        qDebug() << "Connexion établie";
        return true;
    }
    qDebug() << "Échec de la connexion :" << db.lastError().text();
    return false;
}

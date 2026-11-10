#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>

// Singleton : une seule connexion Oracle (ODBC) pour toute l'application.
class Connection {
public:
    static Connection& createInstance();
    bool createConnection();

private:
    QSqlDatabase db;
    Connection();
    ~Connection();
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif // CONNECTION_H

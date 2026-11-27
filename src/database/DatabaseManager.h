
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    DatabaseManager();

    bool connect();
    bool initializeDatabase();

    QString errorMessage() const;

private:
    QSqlDatabase database;
    QString error;
};

#endif

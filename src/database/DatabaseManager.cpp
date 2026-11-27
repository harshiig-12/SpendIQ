
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager()
{
}

bool DatabaseManager::connect()
{
    if (database.isOpen())
        return true;

    database = QSqlDatabase::addDatabase("QSQLITE");

    QString folder = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation
    );

    QDir().mkpath(folder);

    database.setDatabaseName(folder + "/spendiq.db");

    if (!database.open()) {
        error = database.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::initializeDatabase()
{
    if (!database.isOpen() && !connect())
        return false;

    QSqlQuery query;

    QString transactions = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            amount REAL NOT NULL,
            category TEXT NOT NULL,
            type TEXT NOT NULL,
            date TEXT NOT NULL,
            description TEXT
        )
    )";

    if (!query.exec(transactions)) {
        error = query.lastError().text();
        return false;
    }

    QString budgets = R"(
        CREATE TABLE IF NOT EXISTS budgets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            category TEXT NOT NULL,
            amount REAL NOT NULL,
            spent REAL DEFAULT 0,
            month INTEGER NOT NULL,
            year INTEGER NOT NULL
        )
    )";

    if (!query.exec(budgets)) {
        error = query.lastError().text();
        return false;
    }

    return true;
}

QString DatabaseManager::errorMessage() const
{
    return error;
}

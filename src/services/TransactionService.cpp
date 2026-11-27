
#include "TransactionService.h"
#include "../database/DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

bool TransactionService::addTransaction(
    const Transaction& transaction)
{
    QSqlQuery query;

    query.prepare(R"(
        INSERT INTO transactions
        (title, amount, category, type, date, description)
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(transaction.title);
    query.addBindValue(transaction.amount);
    query.addBindValue(transaction.category);

    query.addBindValue(
        transaction.type == TransactionType::Income
        ? "Income" : "Expense"
    );

    query.addBindValue(transaction.date.toString(Qt::ISODate));
    query.addBindValue(transaction.description);

    return query.exec();
}

QList<Transaction> TransactionService::getAllTransactions()
{
    QList<Transaction> transactions;
    QSqlQuery query(
        "SELECT id, title, amount, category, type, date, description "
        "FROM transactions ORDER BY date DESC"
    );

    while (query.next()) {
        Transaction t;

        t.id = query.value(0).toInt();
        t.title = query.value(1).toString();
        t.amount = query.value(2).toDouble();
        t.category = query.value(3).toString();

        t.type = query.value(4).toString() == "Income"
            ? TransactionType::Income
            : TransactionType::Expense;

        t.date = QDate::fromString(
            query.value(5).toString(), Qt::ISODate
        );

        t.description = query.value(6).toString();

        transactions.append(t);
    }

    return transactions;
}

bool TransactionService::deleteTransaction(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM transactions WHERE id = ?");
    query.addBindValue(id);

    return query.exec();
}

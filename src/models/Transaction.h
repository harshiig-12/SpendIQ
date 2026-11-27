
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDate>

enum class TransactionType {
    Income,
    Expense
};

class Transaction
{
public:
    int id = 0;
    QString title;
    double amount = 0.0;
    QString category;
    TransactionType type = TransactionType::Expense;
    QDate date;
    QString description;
};

#endif

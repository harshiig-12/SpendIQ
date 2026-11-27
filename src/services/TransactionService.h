
#ifndef TRANSACTIONSERVICE_H
#define TRANSACTIONSERVICE_H

#include "../models/Transaction.h"
#include <QList>

class TransactionService
{
public:
    bool addTransaction(const Transaction& transaction);
    QList<Transaction> getAllTransactions();
    bool deleteTransaction(int id);
};

#endif

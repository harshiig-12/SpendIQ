
#ifndef BUDGETSERVICE_H
#define BUDGETSERVICE_H

#include "../models/Budget.h"
#include <QList>

class BudgetService
{
public:
    bool addBudget(const Budget& budget);
    QList<Budget> getAllBudgets();
    bool deleteBudget(int id);
};

#endif

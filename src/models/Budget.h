
#ifndef BUDGET_H
#define BUDGET_H

#include <QString>

class Budget
{
public:
    int id = 0;
    QString category;
    double limit = 0.0;
    double spent = 0.0;
    int month = 0;
    int year = 0;
};

#endif

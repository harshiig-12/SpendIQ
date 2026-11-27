
#include "BudgetService.h"

#include <QSqlQuery>
#include <QVariant>

bool BudgetService::addBudget(const Budget& budget)
{
    QSqlQuery query;

    query.prepare(R"(
        INSERT INTO budgets (category, amount, spent, month, year)
        VALUES (?, ?, ?, ?, ?)
    )");

    query.addBindValue(budget.category);
    query.addBindValue(budget.limit);
    query.addBindValue(budget.spent);
    query.addBindValue(budget.month);
    query.addBindValue(budget.year);

    return query.exec();
}

QList<Budget> BudgetService::getAllBudgets()
{
    QList<Budget> budgets;

    QSqlQuery query(
        "SELECT id, category, amount, spent, month, year FROM budgets"
    );

    while (query.next()) {
        Budget b;

        b.id = query.value(0).toInt();
        b.category = query.value(1).toString();
        b.limit = query.value(2).toDouble();
        b.spent = query.value(3).toDouble();
        b.month = query.value(4).toInt();
        b.year = query.value(5).toInt();

        budgets.append(b);
    }

    return budgets;
}

bool BudgetService::deleteBudget(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM budgets WHERE id = ?");
    query.addBindValue(id);

    return query.exec();
}

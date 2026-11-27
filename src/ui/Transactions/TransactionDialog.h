
#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include "../../models/Transaction.h"

class QLineEdit;
class QDoubleSpinBox;
class QComboBox;
class QDateEdit;
class QTextEdit;

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDialog(QWidget* parent = nullptr);

    Transaction getTransaction() const;

private:
    QLineEdit* titleInput;
    QDoubleSpinBox* amountInput;
    QComboBox* categoryInput;
    QComboBox* typeInput;
    QDateEdit* dateInput;
    QTextEdit* descriptionInput;
};

#endif

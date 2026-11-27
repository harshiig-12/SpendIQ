
#include "TransactionDialog.h"

#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

TransactionDialog::TransactionDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Add Transaction");
    setMinimumWidth(440);
    setStyleSheet(R"(
        QDialog {
            background-color: #101827;
            color: #E2E8F0;
        }

        QLabel {
            color: #CBD5E1;
            font-size: 13px;
        }

        QLineEdit, QDoubleSpinBox, QComboBox,
        QDateEdit, QTextEdit {
            background-color: #1E293B;
            color: #F8FAFC;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 9px;
            selection-background-color: #059669;
            selection-color: white;
        }

        QLineEdit:focus, QDoubleSpinBox:focus,
        QComboBox:focus, QDateEdit:focus,
        QTextEdit:focus {
            border: 1px solid #34D399;
        }

        QLineEdit::placeholder {
            color: #64748B;
        }

        QComboBox QAbstractItemView {
            background-color: #1E293B;
            color: #F8FAFC;
            selection-background-color: #153A35;
            selection-color: #34D399;
            border: 1px solid #334155;
        }

        QPushButton {
            background-color: #1E293B;
            color: #E2E8F0;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 9px 20px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #334155;
        }

        QDialogButtonBox QPushButton[text="Save"] {
            background-color: #059669;
            color: white;
            border: none;
        }

        QDialogButtonBox QPushButton[text="Save"]:hover {
            background-color: #10B981;
        }
    )");

    titleInput = new QLineEdit(this);
    titleInput->setPlaceholderText("e.g. Grocery shopping");

    amountInput = new QDoubleSpinBox(this);
    amountInput->setRange(0.01, 100000000);
    amountInput->setDecimals(2);
    amountInput->setPrefix("₹ ");

    categoryInput = new QComboBox(this);
    categoryInput->addItems({
        "Food", "Transport", "Shopping",
        "Bills", "Entertainment", "Salary",
        "Healthcare", "Education", "Other"
    });

    typeInput = new QComboBox(this);
    typeInput->addItems({"Expense", "Income"});

    dateInput = new QDateEdit(QDate::currentDate(), this);
    dateInput->setCalendarPopup(true);
    dateInput->setDisplayFormat("dd MMM yyyy");

    descriptionInput = new QTextEdit(this);
    descriptionInput->setMaximumHeight(85);
    descriptionInput->setPlaceholderText("Optional notes...");

    auto* form = new QFormLayout;
    form->setSpacing(13);
    form->addRow("Title", titleInput);
    form->addRow("Amount", amountInput);
    form->addRow("Category", categoryInput);
    form->addRow("Type", typeInput);
    form->addRow("Date", dateInput);
    form->addRow("Description", descriptionInput);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel,
        this
    );

    connect(buttons, &QDialogButtonBox::accepted,
            this, &QDialog::accept);

    connect(buttons, &QDialogButtonBox::rejected,
            this, &QDialog::reject);

    auto* heading = new QLabel("Transaction Details");
    heading->setStyleSheet(
        "color: #34D399; font-size: 18px; font-weight: bold;"
    );

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 22, 24, 22);
    layout->setSpacing(18);

    layout->addWidget(heading);
    layout->addLayout(form);
    layout->addWidget(buttons);
}

Transaction TransactionDialog::getTransaction() const
{
    Transaction transaction;

    transaction.title = titleInput->text().trimmed();
    transaction.amount = amountInput->value();
    transaction.category = categoryInput->currentText();

    transaction.type = typeInput->currentText() == "Income"
        ? TransactionType::Income
        : TransactionType::Expense;

    transaction.date = dateInput->date();
    transaction.description = descriptionInput->toPlainText();

    return transaction;
}

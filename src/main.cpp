
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QSpacerItem>
#include <QStatusBar>
#include <QDate>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QColor>
#include <QLineEdit>
#include <QComboBox>
#include <QStackedWidget>
#include <QStyle>
#include <functional>
#include "database/DatabaseManager.h"
#include "services/TransactionService.h"
#include "ui/Transactions/TransactionDialog.h"
QFrame* createCard(
    const QString& title,
    const QString& amount,
    const QString& subtitle,
    const QString& accent = "#34D399",
    QLabel** valueOutput = nullptr)
{
    auto* card = new QFrame;
    card->setObjectName("statCard");
    card->setMinimumHeight(145);

    auto* layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 18, 22, 18);
    layout->setSpacing(10);

    auto* heading = new QLabel(title);
    heading->setObjectName("cardHeading");

    auto* value = new QLabel(amount);
    value->setObjectName("cardValue");
    value->setStyleSheet("color: " + accent + ";");

    if (valueOutput != nullptr) {
        *valueOutput = value;
    }

    auto* detail = new QLabel(subtitle);
    detail->setObjectName("mutedText");

    layout->addWidget(heading);
    layout->addWidget(value);
    layout->addWidget(detail);
    layout->addStretch();

    return card;
}

QPushButton* createNavButton(const QString& text)
{
    auto* button = new QPushButton(text);
    button->setObjectName("navButton");
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(43);
    return button;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize the local SQLite database before building the dashboard.
    DatabaseManager databaseManager;

    if (!databaseManager.connect() ||
        !databaseManager.initializeDatabase()) {

        QMessageBox::critical(
            nullptr,
            "Database Error",
            databaseManager.errorMessage()
        );

        return 1;
    }

    TransactionService transactionService;

    QMainWindow window;
    window.setWindowTitle("SpendIQ | Personal Finance");
    window.resize(1350, 850);
    window.setMinimumSize(1000, 650);

    // Main container
    auto* central = new QWidget;
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Sidebar
    auto* sidebar = new QFrame;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(225);

    auto* sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(18, 28, 18, 22);
    sideLayout->setSpacing(12);

    auto* brand = new QLabel("SpendIQ");
    brand->setObjectName("brand");

    auto* tagline = new QLabel("PERSONAL FINANCE");
    tagline->setObjectName("sideCaption");

    sideLayout->addWidget(brand);
    sideLayout->addWidget(tagline);
    sideLayout->addSpacing(30);

    auto* overview = createNavButton("  ◈   Overview");
    auto* transactions = createNavButton("  ⇄   Transactions");
    auto* budgets = createNavButton("  ◎   Budgets");
    auto* analytics = createNavButton("  ▥   Analytics");

    overview->setObjectName("activeNav");

    sideLayout->addWidget(overview);
    sideLayout->addWidget(transactions);
    sideLayout->addWidget(budgets);
    sideLayout->addWidget(analytics);
    sideLayout->addSpacing(20);

    auto* toolsLabel = new QLabel("PREFERENCES");
    toolsLabel->setObjectName("sideCaption");

    auto* settings = createNavButton("  ⚙   Settings");

    sideLayout->addWidget(toolsLabel);
    sideLayout->addWidget(settings);
    sideLayout->addStretch();

    auto* version = new QLabel("SpendIQ  •  Version 1.0");
    version->setObjectName("sideCaption");
    sideLayout->addWidget(version);

    // Main dashboard
    auto* mainArea = new QWidget;
    mainArea->setObjectName("mainArea");

    auto* mainLayout = new QVBoxLayout(mainArea);
    mainLayout->setContentsMargins(32, 26, 32, 26);
    mainLayout->setSpacing(22);

    // Header
    auto* header = new QHBoxLayout;

    auto* headingBox = new QVBoxLayout;

    auto* pageTitle = new QLabel("Overview");
    pageTitle->setObjectName("pageTitle");

    auto* welcome = new QLabel(
        "Welcome back! Here's your financial snapshot."
    );
    welcome->setObjectName("mutedText");

    headingBox->addWidget(pageTitle);
    headingBox->addWidget(welcome);

    auto* dateLabel = new QLabel(
        QDate::currentDate().toString("dddd, dd MMMM yyyy")
    );
    dateLabel->setObjectName("dateLabel");

    header->addLayout(headingBox);
    header->addStretch();
    header->addWidget(dateLabel);

    mainLayout->addLayout(header);

    // Summary cards. Their value labels are retained so SQLite totals
    // can refresh them whenever transactions change.
    QLabel* totalBalanceValue = nullptr;
    QLabel* totalIncomeValue = nullptr;
    QLabel* totalExpensesValue = nullptr;
    QLabel* monthlySavingsValue = nullptr;

    auto* cards = new QGridLayout;
    cards->setSpacing(16);

    cards->addWidget(
        createCard("TOTAL BALANCE", "₹ 0.00",
                   "Your current net balance", "#34D399",
                   &totalBalanceValue), 0, 0);

    cards->addWidget(
        createCard("TOTAL INCOME", "₹ 0.00",
                   "Income recorded this month", "#34D399",
                   &totalIncomeValue), 0, 1);

    cards->addWidget(
        createCard("TOTAL EXPENSES", "₹ 0.00",
                   "Expenses recorded this month", "#F87171",
                   &totalExpensesValue), 0, 2);

    cards->addWidget(
        createCard("MONTHLY SAVINGS", "₹ 0.00",
                   "Income minus expenses", "#60A5FA",
                   &monthlySavingsValue), 0, 3);

    mainLayout->addLayout(cards);

    // Middle section
    auto* middle = new QHBoxLayout;
    middle->setSpacing(16);

    auto* analyticsCard = new QFrame;
    analyticsCard->setObjectName("panel");

    auto* analyticsLayout = new QVBoxLayout(analyticsCard);
    analyticsLayout->setContentsMargins(22, 20, 22, 20);

    auto* analyticsTitle = new QLabel("Spending Overview");
    analyticsTitle->setObjectName("sectionTitle");

    auto* chartPlaceholder = new QLabel(
        "Your spending analytics will appear here.\n\n"
        "Start adding transactions to see your financial trends."
    );
    chartPlaceholder->setObjectName("emptyState");
    chartPlaceholder->setAlignment(Qt::AlignCenter);
    chartPlaceholder->setMinimumHeight(180);

    analyticsLayout->addWidget(analyticsTitle);
    analyticsLayout->addWidget(chartPlaceholder, 1);

    auto* budgetCard = new QFrame;
    budgetCard->setObjectName("panel");
    budgetCard->setMinimumWidth(240);

    auto* budgetLayout = new QVBoxLayout(budgetCard);
    budgetLayout->setContentsMargins(22, 20, 22, 20);

    auto* budgetTitle = new QLabel("Budget Snapshot");
    budgetTitle->setObjectName("sectionTitle");

    auto* budgetAmount = new QLabel("₹ 0.00");
    budgetAmount->setObjectName("cardValue");

    auto* budgetInfo = new QLabel(
        "No budgets created yet.\n\n"
        "Create your first monthly budget to track spending."
    );
    budgetInfo->setObjectName("mutedText");
    budgetInfo->setWordWrap(true);

    budgetLayout->addWidget(budgetTitle);
    budgetLayout->addSpacing(20);
    budgetLayout->addWidget(budgetAmount);
    budgetLayout->addWidget(budgetInfo);
    budgetLayout->addStretch();

    middle->addWidget(analyticsCard, 2);
    middle->addWidget(budgetCard, 1);

    mainLayout->addLayout(middle);

    // Recent transactions
    auto* recentPanel = new QFrame;
    recentPanel->setObjectName("panel");

    auto* recentLayout = new QVBoxLayout(recentPanel);
    recentLayout->setContentsMargins(22, 18, 22, 18);

    auto* recentHeading = new QHBoxLayout;

    auto* recentTitle = new QLabel("Recent Transactions");
    recentTitle->setObjectName("sectionTitle");

    auto* viewAll = new QLabel("TRANSACTION HISTORY");
    viewAll->setObjectName("mutedText");

    recentHeading->addWidget(recentTitle);
    recentHeading->addStretch();
    recentHeading->addWidget(viewAll);

    auto* table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels({
        "DESCRIPTION", "CATEGORY", "DATE", "AMOUNT"
    });

    table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setShowGrid(false);
    table->setAlternatingRowColors(false);
    table->setMinimumHeight(130);

    // Recalculate the summary cards from the saved SQLite transactions.
    auto refreshDashboardTotals = [&]() {
        const QList<Transaction> records =
            transactionService.getAllTransactions();

        double allTimeIncome = 0.0;
        double allTimeExpenses = 0.0;
        double currentMonthIncome = 0.0;
        double currentMonthExpenses = 0.0;

        const QDate today = QDate::currentDate();

        for (const Transaction& transaction : records) {
            if (transaction.type == TransactionType::Income) {
                allTimeIncome += transaction.amount;

                if (transaction.date.month() == today.month() &&
                    transaction.date.year() == today.year()) {
                    currentMonthIncome += transaction.amount;
                }
            } else {
                allTimeExpenses += transaction.amount;

                if (transaction.date.month() == today.month() &&
                    transaction.date.year() == today.year()) {
                    currentMonthExpenses += transaction.amount;
                }
            }
        }

        const double totalBalance = allTimeIncome - allTimeExpenses;
        const double monthlySavings =
            currentMonthIncome - currentMonthExpenses;

        totalBalanceValue->setText(
            "₹ " + QString::number(totalBalance, 'f', 2)
        );
        totalIncomeValue->setText(
            "₹ " + QString::number(currentMonthIncome, 'f', 2)
        );
        totalExpensesValue->setText(
            "₹ " + QString::number(currentMonthExpenses, 'f', 2)
        );
        monthlySavingsValue->setText(
            "₹ " + QString::number(monthlySavings, 'f', 2)
        );
    };

    // Load the latest transactions from SQLite into the dashboard table.
    auto refreshTransactions = [&]() {
        const QList<Transaction> records =
            transactionService.getAllTransactions();

        table->setRowCount(0);

        const int visibleCount = qMin(records.size(), 10);

        for (int i = 0; i < visibleCount; ++i) {
            const Transaction& transaction = records[i];

            const int row = table->rowCount();
            table->insertRow(row);

            table->setItem(
                row, 0,
                new QTableWidgetItem(transaction.title)
            );

            table->setItem(
                row, 1,
                new QTableWidgetItem(transaction.category)
            );

            table->setItem(
                row, 2,
                new QTableWidgetItem(
                    transaction.date.toString("dd MMM yyyy")
                )
            );

            const QString amountText =
                (transaction.type == TransactionType::Income
                    ? "+ ₹ " : "- ₹ ")
                + QString::number(transaction.amount, 'f', 2);

            auto* amountItem = new QTableWidgetItem(amountText);
            amountItem->setTextAlignment(
                Qt::AlignRight | Qt::AlignVCenter
            );
            amountItem->setForeground(
                QColor(
                    transaction.type == TransactionType::Income
                        ? "#34D399" : "#F87171"
                )
            );

            table->setItem(row, 3, amountItem);
        }

        refreshDashboardTotals();
    };

    // Populate the dashboard and summary cards from saved transactions.
    refreshTransactions();

    recentLayout->addLayout(recentHeading);
    recentLayout->addWidget(table);

    mainLayout->addWidget(recentPanel, 1);

    // Full Transaction History page
    auto* transactionPage = new QWidget;
    transactionPage->setObjectName("mainArea");

    auto* historyLayout = new QVBoxLayout(transactionPage);
    historyLayout->setContentsMargins(32, 26, 32, 26);
    historyLayout->setSpacing(20);

    auto* historyHeader = new QHBoxLayout;
    auto* historyHeadingBox = new QVBoxLayout;

    auto* historyTitle = new QLabel("Transaction History");
    historyTitle->setObjectName("pageTitle");

    auto* historySubtitle = new QLabel(
        "Review, search and manage all your income and expenses."
    );
    historySubtitle->setObjectName("mutedText");

    historyHeadingBox->addWidget(historyTitle);
    historyHeadingBox->addWidget(historySubtitle);

    auto* historyDate = new QLabel(
        QDate::currentDate().toString("dddd, dd MMMM yyyy")
    );
    historyDate->setObjectName("dateLabel");

    historyHeader->addLayout(historyHeadingBox);
    historyHeader->addStretch();
    historyHeader->addWidget(historyDate);
    historyLayout->addLayout(historyHeader);

    auto* historyPanel = new QFrame;
    historyPanel->setObjectName("panel");

    auto* historyPanelLayout = new QVBoxLayout(historyPanel);
    historyPanelLayout->setContentsMargins(22, 22, 22, 22);
    historyPanelLayout->setSpacing(16);

    auto* filterRow = new QHBoxLayout;
    filterRow->setSpacing(12);

    auto* searchInput = new QLineEdit;
    searchInput->setPlaceholderText("Search transactions by title...");
    searchInput->setClearButtonEnabled(true);

    auto* typeFilter = new QComboBox;
    typeFilter->addItems({"All Types", "Income", "Expense"});

    auto* categoryFilter = new QComboBox;
    categoryFilter->addItems({
        "All Categories", "Food", "Transport", "Shopping",
        "Bills", "Entertainment", "Salary", "Healthcare",
        "Education", "Other"
    });

    auto* addTransactionButton = new QPushButton("+  Add Transaction");
    addTransactionButton->setObjectName("primaryButton");
    addTransactionButton->setCursor(Qt::PointingHandCursor);
    addTransactionButton->setMinimumHeight(42);

    filterRow->addWidget(searchInput, 2);
    filterRow->addWidget(typeFilter, 1);
    filterRow->addWidget(categoryFilter, 1);
    filterRow->addWidget(addTransactionButton);

    auto* historyTable = new QTableWidget(0, 6);
    historyTable->setHorizontalHeaderLabels({
        "DESCRIPTION", "CATEGORY", "TYPE", "DATE", "AMOUNT", "ACTION"
    });
    historyTable->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::Stretch
    );
    historyTable->horizontalHeader()->setSectionResizeMode(
        1, QHeaderView::ResizeToContents
    );
    historyTable->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::ResizeToContents
    );
    historyTable->horizontalHeader()->setSectionResizeMode(
        3, QHeaderView::ResizeToContents
    );
    historyTable->horizontalHeader()->setSectionResizeMode(
        4, QHeaderView::ResizeToContents
    );
    historyTable->horizontalHeader()->setSectionResizeMode(
        5, QHeaderView::Fixed
    );
    historyTable->setColumnWidth(5, 100);
    historyTable->verticalHeader()->setVisible(false);
    historyTable->verticalHeader()->setDefaultSectionSize(48);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    historyTable->setShowGrid(false);
    historyTable->setMinimumHeight(420);

    auto* historyEmpty = new QLabel(
        "No transactions match your search or filters."
    );
    historyEmpty->setObjectName("emptyState");
    historyEmpty->setAlignment(Qt::AlignCenter);
    historyEmpty->setMinimumHeight(220);
    historyEmpty->setVisible(false);

    historyPanelLayout->addLayout(filterRow);
    historyPanelLayout->addWidget(historyTable, 1);
    historyPanelLayout->addWidget(historyEmpty);
    historyLayout->addWidget(historyPanel, 1);

    // The central stack switches between Overview and Transaction History.
    auto* pageStack = new QStackedWidget;
    pageStack->addWidget(mainArea);
    pageStack->addWidget(transactionPage);

    // Rebuild the history table from SQLite and apply the active filters.
    std::function<void()> refreshTransactionHistory;

refreshTransactionHistory = [&]() {
        const QList<Transaction> records =
            transactionService.getAllTransactions();

        const QString searchText = searchInput->text().trimmed();
        const QString selectedType = typeFilter->currentText();
        const QString selectedCategory = categoryFilter->currentText();

        historyTable->setRowCount(0);

        for (const Transaction& transaction : records) {
            if (!searchText.isEmpty() &&
                !transaction.title.contains(searchText, Qt::CaseInsensitive)) {
                continue;
            }

            const QString transactionType =
                transaction.type == TransactionType::Income
                    ? "Income" : "Expense";

            if (selectedType != "All Types" &&
                transactionType != selectedType) {
                continue;
            }

            if (selectedCategory != "All Categories" &&
                transaction.category != selectedCategory) {
                continue;
            }

            const int row = historyTable->rowCount();
            historyTable->insertRow(row);

            historyTable->setItem(
                row, 0, new QTableWidgetItem(transaction.title)
            );
            historyTable->setItem(
                row, 1, new QTableWidgetItem(transaction.category)
            );
            historyTable->setItem(
                row, 2, new QTableWidgetItem(transactionType)
            );
            historyTable->setItem(
                row, 3,
                new QTableWidgetItem(
                    transaction.date.toString("dd MMM yyyy")
                )
            );

            const QString amountText =
                (transaction.type == TransactionType::Income
                    ? "+ ₹ " : "- ₹ ")
                + QString::number(transaction.amount, 'f', 2);

            auto* amountItem = new QTableWidgetItem(amountText);
            amountItem->setTextAlignment(
                Qt::AlignRight | Qt::AlignVCenter
            );
            amountItem->setForeground(
                QColor(transaction.type == TransactionType::Income
                    ? "#34D399" : "#F87171")
            );
            historyTable->setItem(row, 4, amountItem);

            auto* deleteButton = new QPushButton("Delete");
            deleteButton->setObjectName("deleteButton");
            deleteButton->setCursor(Qt::PointingHandCursor);

            QObject::connect(
                deleteButton, &QPushButton::clicked,
                [&, transactionId = transaction.id]() {
QMessageBox confirmBox(&window);

confirmBox.setWindowTitle("Delete Transaction");
confirmBox.setText(
    "Are you sure you want to permanently delete this transaction?"
);
confirmBox.setIcon(QMessageBox::Warning);

confirmBox.setStandardButtons(
    QMessageBox::Yes | QMessageBox::No
);

confirmBox.setDefaultButton(QMessageBox::No);

confirmBox.setStyleSheet(R"(
    QMessageBox {
        background-color: #0B1120;
        color: #E2E8F0;
    }

    QLabel {
        color: #E2E8F0;
        font-size: 13px;
    }

    QPushButton {
        background-color: #1E293B;
        color: #E2E8F0;
        border: 1px solid #334155;
        border-radius: 6px;
        padding: 8px 20px;
        min-width: 65px;
    }

    QPushButton:hover {
        background-color: #334155;
    }

    QPushButton:default {
        border: 1px solid #34D399;
    }
)");

const auto answer = confirmBox.exec();

                    if (answer != QMessageBox::Yes) {
                        return;
                    }

                    if (transactionService.deleteTransaction(transactionId)) {
                        refreshTransactions();
                        refreshTransactionHistory();
                        window.statusBar()->showMessage(
                            "Transaction deleted successfully.", 4000
                        );
                    } else {
                        QMessageBox::warning(
                            &window,
                            "Delete Failed",
                            "Could not delete this transaction."
                        );
                    }
                }
            );

            historyTable->setCellWidget(row, 5, deleteButton);
        }

        const bool hasResults = historyTable->rowCount() > 0;
        historyTable->setVisible(hasResults);
        historyEmpty->setVisible(!hasResults);
    };

    // Initial population of both transaction views.
    refreshTransactionHistory();

    QObject::connect(
        searchInput, &QLineEdit::textChanged,
        [&](const QString&) { refreshTransactionHistory(); }
    );
    QObject::connect(
        typeFilter, &QComboBox::currentTextChanged,
        [&](const QString&) { refreshTransactionHistory(); }
    );
    QObject::connect(
        categoryFilter, &QComboBox::currentTextChanged,
        [&](const QString&) { refreshTransactionHistory(); }
    );

    // Navigation and active sidebar styling
    auto setActiveNavigation = [&](QPushButton* activeButton) {
        for (QPushButton* button : {
                 overview, transactions, budgets, analytics, settings
             }) {
            button->setObjectName(
                button == activeButton ? "activeNav" : "navButton"
            );
            button->style()->unpolish(button);
            button->style()->polish(button);
        }
    };

    QObject::connect(overview, &QPushButton::clicked, [&]() {
        pageStack->setCurrentWidget(mainArea);
        setActiveNavigation(overview);
        pageTitle->setText("Overview");
    });

    QObject::connect(transactions, &QPushButton::clicked, [&]() {
        pageStack->setCurrentWidget(transactionPage);
        setActiveNavigation(transactions);
        refreshTransactionHistory();
    });

    auto showComingSoon = [&window](const QString& page) {
        window.statusBar()->showMessage(
            page + " module is coming soon."
        );
    };

    QObject::connect(addTransactionButton, &QPushButton::clicked, [&]() {
        TransactionDialog dialog(&window);

        if (dialog.exec() == QDialog::Accepted) {
            const Transaction transaction = dialog.getTransaction();

            if (transaction.title.trimmed().isEmpty()) {
                QMessageBox::warning(
                    &window,
                    "Missing Title",
                    "Please enter a transaction title."
                );
                return;
            }

            if (transactionService.addTransaction(transaction)) {
                refreshTransactions();
                refreshTransactionHistory();
                window.statusBar()->showMessage(
                    "Transaction saved successfully!", 4000
                );
            } else {
                QMessageBox::warning(
                    &window,
                    "Save Failed",
                    "Could not save the transaction to the database."
                );
            }
        }
    });

    QObject::connect(budgets, &QPushButton::clicked,
        [&]() { showComingSoon("Budgets"); });

    QObject::connect(analytics, &QPushButton::clicked,
        [&]() { showComingSoon("Analytics"); });

    QObject::connect(settings, &QPushButton::clicked,
        [&]() { showComingSoon("Settings"); });

    // Add sidebar and stacked content pages to the main layout
    root->addWidget(sidebar);
    root->addWidget(pageStack, 1);
    window.setCentralWidget(central);

    window.setStyleSheet(R"(
        QMainWindow {
            background-color: #0B1120;
        }

        QWidget {
            color: #E2E8F0;
            font-family: "Segoe UI";
            font-size: 13px;
        }

        QFrame#sidebar {
            background-color: #0B1220;
            border-right: 1px solid #263247;
        }

        QLabel#brand {
            color: #34D399;
            font-size: 29px;
            font-weight: bold;
        }

        QLabel#sideCaption {
            color: #64748B;
            font-size: 10px;
            font-weight: bold;
            letter-spacing: 1px;
        }

        QPushButton#navButton {
            background: transparent;
            color: #94A3B8;
            text-align: left;
            border: none;
            border-radius: 8px;
            padding: 10px;
        }

        QPushButton#navButton:hover {
            background-color: #1E293B;
            color: white;
        }

        QPushButton#activeNav {
            background-color: #153A35;
            color: #34D399;
            text-align: left;
            border: none;
            border-radius: 8px;
            padding: 10px;
            font-weight: bold;
        }

        QWidget#mainArea {
            background-color: #101827;
        }

        QLabel#pageTitle {
            font-size: 27px;
            font-weight: bold;
            color: #F8FAFC;
        }

        QLabel#dateLabel {
            color: #94A3B8;
            font-size: 12px;
        }

        QFrame#statCard, QFrame#panel {
            background-color: #1E293B;
            border: 1px solid #29374D;
            border-radius: 13px;
        }

        QLabel#cardHeading {
            color: #94A3B8;
            font-size: 11px;
            font-weight: bold;
        }

        QLabel#cardValue {
            color: #34D399;
            font-size: 25px;
            font-weight: bold;
        }

        QLabel#mutedText {
            color: #94A3B8;
            font-size: 12px;
        }

        QLabel#sectionTitle {
            color: #F8FAFC;
            font-size: 16px;
            font-weight: bold;
        }

        QLabel#emptyState {
            color: #64748B;
            background-color: #172235;
            border-radius: 9px;
        }

        QLineEdit, QComboBox {
            background-color: #172235;
            color: #E2E8F0;
            border: 1px solid #334155;
            border-radius: 7px;
            padding: 10px;
            min-height: 20px;
        }

        QLineEdit:focus, QComboBox:focus {
            border: 1px solid #34D399;
        }

        QComboBox QAbstractItemView {
            background-color: #1E293B;
            color: #E2E8F0;
            selection-background-color: #153A35;
            selection-color: #34D399;
        }

        QPushButton#primaryButton {
            background-color: #059669;
            color: white;
            border: none;
            border-radius: 7px;
            padding: 10px 16px;
            font-weight: bold;
        }

        QPushButton#primaryButton:hover {
            background-color: #10B981;
        }

        QPushButton#deleteButton {
            background-color: #3B1F2A;
            color: #F87171;
            border: 1px solid #7F1D1D;
            border-radius: 6px;
            padding: 6px 12px;
        }

        QPushButton#deleteButton:hover {
            background-color: #7F1D1D;
            color: white;
        }

        QTableWidget {
            background-color: transparent;
            border: none;
            gridline-color: #29374D;
        }

        QHeaderView::section {
            background-color: #172235;
            color: #94A3B8;
            border: none;
            padding: 12px;
            font-size: 10px;
            font-weight: bold;
        }

        QTableWidget::item {
            padding: 10px;
            border-bottom: 1px solid #29374D;
        }

        QStatusBar {
            background-color: #0B1220;
            color: #94A3B8;
        }
    )");

    window.show();

    return app.exec();
}

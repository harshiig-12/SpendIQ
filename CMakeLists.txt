
cmake_minimum_required(VERSION 3.21)

project(SpendIQ VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS
    Widgets
    Sql
)

qt_add_executable(SpendIQ
    src/main.cpp

    src/database/DatabaseManager.cpp
    src/database/DatabaseManager.h

    src/services/TransactionService.cpp
    src/services/TransactionService.h

    src/services/BudgetService.cpp
    src/services/BudgetService.h

    src/ui/Transactions/TransactionDialog.cpp
    src/ui/Transactions/TransactionDialog.h

    src/models/Transaction.h
    src/models/Budget.h
)

target_link_libraries(SpendIQ PRIVATE
    Qt6::Widgets
    Qt6::Sql
)

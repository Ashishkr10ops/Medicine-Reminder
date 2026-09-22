#include "history.h"
#include <iostream>

HistoryStack historyStack;

// Constructor
HistoryStack::HistoryStack()
{
    top = nullptr;
}

// Adds a new history record to the top of the stack
void HistoryStack::push(const History &history)
{

    HistoryNode *newNode = new HistoryNode;

    newNode->history = history;
    newNode->next = top;

    top = newNode;
}

// Removes the top history record
bool HistoryStack::pop()
{

    // Check if the stack is empty
    if (top == nullptr)
    {
        return false;
    }

    HistoryNode *temp = top;

    top = top->next;

    delete temp;

    return true;
}

// Returns the top history record without removing it
History *HistoryStack::peek()
{

    // Check if the stack is empty
    if (top == nullptr)
    {
        return nullptr;
    }

    return &top->history;
}

// Displays all history records
void HistoryStack::display()
{

    if (top == nullptr)
    {
        std::cout << "History is empty.\n";
        return;
    }

    std::cout << "\n===== History =====\n";

    HistoryNode *current = top;

    while (current != nullptr)
    {

        std::cout << current->history.operation << "\n";

        current = current->next;
    }
}

// User facing functions

// Records a new operation in the history
void recordOperation()
{

    History history;

    std::cout << "\nEnter operation: ";
    std::cin.ignore();
    std::getline(std::cin, history.operation);

    historyStack.push(history);

    std::cout << "Operation recorded successfully.\n";
}

// Displays the medicine operation history
void viewHistory()
{

    historyStack.display();
}

// Adds an operation to the history stack
void addHistory(const std::string &operation)
{
    History history;

    history.operation = operation;

    historyStack.push(history);
}
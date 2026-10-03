#include "history.h"

#include <iostream>

HistoryStack historyStack;

// Constructor
// Initially, the stack is empty.
HistoryStack::HistoryStack()
{
    top = nullptr;
}

// Pushes a new operation onto the stack.
void HistoryStack::push(const History &history)
{

    HistoryNode *newNode = new HistoryNode;

    newNode->history = history;
    newNode->next = top;

    top = newNode;
}

// Removes the most recent operation from the stack.
bool HistoryStack::pop()
{

    if (top == nullptr)
    {
        return false;
    }

    HistoryNode *temp = top;

    top = top->next;

    delete temp;

    return true;
}

// Returns the most recent operation without removing it.
History *HistoryStack::peek()
{

    if (top == nullptr)
    {
        return nullptr;
    }

    return &top->history;
}

// Displays the history from newest to oldest.
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

// Allows the user to manually record an operation.
void recordOperation()
{

    History history;

    std::cout << "\nEnter operation: ";

    std::cin.ignore();
    std::getline(std::cin, history.operation);

    historyStack.push(history);

    std::cout << "Operation recorded successfully.\n";
}

// Displays the complete history.
void viewHistory()
{
    historyStack.display();
}

// Used by other modules to automatically record operations.
void addHistory(const std::string &operation)
{

    History history;

    history.operation = operation;

    historyStack.push(history);
}

#include "history.h"

#include <iostream>
#include <fstream>
#include <vector>

// Shared history stack used throughout the application.
HistoryStack historyStack;

// Initialize an empty stack.
HistoryStack::HistoryStack()
{
    top = nullptr;
}

// Push an operation onto the stack (O(1)).
void HistoryStack::push(const History &history)
{
    // Create a new node and link it to the current top.
    HistoryNode *newNode = new HistoryNode;
    newNode->history = history;
    newNode->next = top;

    // The new node becomes the top of the stack.
    top = newNode;
}

// Remove the most recently recorded operation (O(1)).
bool HistoryStack::pop()
{
    if (top == nullptr)
    {
        return false; // Stack underflow: nothing to remove.
    }

    HistoryNode *temp = top;
    top = top->next;

    delete temp; // Release the removed node's memory.
    return true;
}

// Return the top operation without removing it (O(1)).
History *HistoryStack::peek()
{
    return (top == nullptr) ? nullptr : &top->history;
}

// Display operations from newest to oldest.
// Time complexity: O(n), where n is the number of history entries.
void HistoryStack::display()
{
    if (top == nullptr)
    {
        std::cout << "History is empty.\n";
        return;
    }

    std::cout << "\n===== History =====\n";

    // Traverse the linked stack from top to bottom.
    HistoryNode *current = top;

    while (current != nullptr)
    {
        std::cout << current->history.operation << '\n';
        current = current->next;
    }
}

// Save history to a file so it can be restored in a future session.
void HistoryStack::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open history file for writing.\n";
        return;
    }

    // Collect entries from newest to oldest.
    std::vector<std::string> operations;
    HistoryNode *current = top;

    while (current != nullptr)
    {
        operations.push_back(current->history.operation);
        current = current->next;
    }

    // Write oldest to newest so loading restores the original stack order.
    for (auto it = operations.rbegin(); it != operations.rend(); ++it)
    {
        file << *it << '\n';
    }

    file.close();

    if (!file)
    {
        std::cerr << "Error: Failed to save history completely.\n";
    }
}

// Restore history from a file.
// The newest operation ends up at the top of the stack.
void HistoryStack::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);

    // A missing history file is normal on the first run.
    if (!file.is_open())
    {
        return;
    }

    // Read entries first so an input error does not erase current history.
    std::vector<std::string> operations;
    std::string operation;

    while (std::getline(file, operation))
    {
        if (!operation.empty())
        {
            operations.push_back(operation);
        }
    }

    if (file.bad())
    {
        std::cerr << "Error: Failed while reading history file.\n";
        return;
    }

    file.close();

    // Replace the current stack with the successfully read entries.
    while (pop())
    {
    }

    // The file is oldest-first, so pushing in this order restores the stack.
    for (const std::string &entry : operations)
    {
        History history;
        history.operation = entry;
        push(history);
    }
}

// Allow a user to manually add an operation to the history.
void recordOperation()
{
    History history;

    std::cout << "\nEnter operation: ";

    // Read the entire description, including spaces.
    std::getline(std::cin >> std::ws, history.operation);

    historyStack.push(history);
    std::cout << "Operation recorded successfully.\n";
}

// Display the recorded operations.
void viewHistory()
{
    historyStack.display();
}

// Add an operation from another module, such as inventory management.
void addHistory(const std::string &operation)
{
    History history;
    history.operation = operation;
    historyStack.push(history);
}

// Clear the stack and persist the empty history.
void clearHistory()
{
    while (historyStack.pop())
    {
        // Continue until the stack is empty.
    }

    historyStack.saveToFile("history.txt");
    std::cout << "History cleared successfully.\n";
}

// Release all dynamically allocated stack nodes.
HistoryStack::~HistoryStack()
{
    while (pop())
    {
        // Each pop deletes one node.
    }
}

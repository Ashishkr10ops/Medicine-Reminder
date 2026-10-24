
#include "history.h"

#include <iostream>

// Define the shared history stack declared in history.h.
HistoryStack historyStack;

// Constructor: initialize the stack as empty.
HistoryStack::HistoryStack()
{
    top = nullptr;
}

// Push a new operation onto the top of the stack.
void HistoryStack::push(const History &history)
{
    // Allocate memory for a new stack node.
    HistoryNode *newNode = new HistoryNode;

    newNode->history = history;

    // Link the new node to the previous top node.
    newNode->next = top;

    // Make the new node the top of the stack.
    top = newNode;
}

// Remove the most recently added operation.
bool HistoryStack::pop()
{
    // An empty stack has no operation to remove.
    if (top == nullptr)
    {
        return false;
    }

    // Temporarily store the current top node.
    HistoryNode *temp = top;

    // Move the top pointer to the next node.
    top = top->next;

    // Free the memory of the removed node.
    delete temp;

    return true;
}

// Return the latest operation without removing it.
History *HistoryStack::peek()
{
    // Return nullptr when the stack contains no elements.
    if (top == nullptr)
    {
        return nullptr;
    }

    // Return the address of the operation at the top.
    return &top->history;
}

// Display the history from newest operation to oldest.
void HistoryStack::display()
{
    // Handle the case where no operations have been recorded.
    if (top == nullptr)
    {
        std::cout << "History is empty.\n";
        return;
    }

    std::cout << "\n===== History =====\n";

    // Start traversing from the top of the stack.
    HistoryNode *current = top;

    while (current != nullptr)
    {
        // Display the operation stored in the current node.
        std::cout << current->history.operation << "\n";

        // Move to the next older operation.
        current = current->next;
    }
}

// Allow the user to manually enter a history operation.
void recordOperation()
{
    History history;

    std::cout << "\nEnter operation: ";

    // Ignore the leftover newline before reading a full line.
    std::cin.ignore();

    // Read the operation, including spaces.
    std::getline(std::cin, history.operation);

    // Store the operation at the top of the stack.
    historyStack.push(history);

    std::cout << "Operation recorded successfully.\n";
}

// Display the complete operation history.
void viewHistory()
{
    historyStack.display();
}

// Add an operation to history from another module.
void addHistory(const std::string &operation)
{
    History history;

    // Store the supplied operation description.
    history.operation = operation;

    // Push it onto the stack so it becomes the newest entry.
    historyStack.push(history);
}

HistoryStack::~HistoryStack() {
    while (pop());
}

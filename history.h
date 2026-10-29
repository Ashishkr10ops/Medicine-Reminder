
#ifndef HISTORY_H
#define HISTORY_H

#include <string>

// Stores the description of a single operation performed in the system.
struct History
{
    std::string operation;
};

// Node of the linked list used to implement the history stack.
struct HistoryNode
{
    History history;   // Operation stored in this node.
    HistoryNode *next; // Pointer to the next node below it.
};

// Implements a stack using a singly linked list.
// Follows LIFO (Last In, First Out): the newest operation is processed first.
class HistoryStack
{
private:
    HistoryNode *top; // Points to the most recently added operation.

public:
    // Initializes an empty stack.
    HistoryStack();

    // Pushes an operation onto the top of the stack. O(1).
    void push(const History &history);

    // Removes the top operation; returns false if the stack is empty. O(1).
    bool pop();

    // Returns a pointer to the top operation without removing it.
    // Returns nullptr if the stack is empty.
    History *peek();

    // Displays all operations from newest to oldest. O(n).
    void display();

    // Writes stack entries to a file for persistent storage.
    void saveToFile(const std::string &filename);

    // Reads entries from a file and restores the stack.
    void loadFromFile(const std::string &filename);

    // Frees all dynamically allocated nodes to prevent memory leaks.
    ~HistoryStack();
};

// Shared history stack used by the application's modules.
// Its definition is in history.cpp.
extern HistoryStack historyStack;

// User-facing function to manually record an operation.
void recordOperation();

// Displays the recorded operation history.
void viewHistory();

// Adds an operation to the stack from another module.
void addHistory(const std::string &operation);

// Removes all history entries and saves the empty history.
void clearHistory();

#endif

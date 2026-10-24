
#ifndef HISTORY_H
#define HISTORY_H

#include <string>

// Stores the description of one operation performed in the system.
struct History
{
    std::string operation;
};

// Node used to implement the history stack as a linked list.
struct HistoryNode
{
    History history;
    HistoryNode *next;
};

// Stack that stores operations in Last In, First Out (LIFO) order.
class HistoryStack
{
private:
    // Points to the most recently added operation.
    HistoryNode *top;

public:
    // Initializes an empty stack.
    HistoryStack();

    // Adds a new operation to the top of the stack.
    void push(const History &history);

    // Removes the top operation; returns false if the stack is empty.
    bool pop();

    // Returns the top operation without removing it.
    History *peek();

    // Displays operations from newest to oldest.
    void display();
};

// Shared stack object defined in history.cpp.
extern HistoryStack historyStack;

// User-facing function to manually record an operation.
void recordOperation();

// Displays the operation history.
void viewHistory();

// Allows other modules to add operations to the history stack.
void addHistory(const std::string &operation);

#endif

#include "history.h"
#include <iostream>

// Constructor
HistoryStack::HistoryStack()
{
    top = nullptr;
}

// Adds a new history record to the top of the stack
void HistoryStack::push(const History &history)
{

    Node *newNode = new Node;

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

    Node *temp = top;

    top = top->next;

    delete temp;

    return true;
}

void recordOperation()
{
}

void viewHistory()
{
}

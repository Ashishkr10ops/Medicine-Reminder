#ifndef HISTORY_H
#define HISTORY_H

#include <string>

struct History
{
    std::string operation;
};

struct Node
{
    History history;
    Node *next;
};

class HistoryStack
{
private:
    Node *top;

public:
    HistoryStack();

    void push(const History &history);

    bool pop();

    History *peak();

    void display();
};

// User facing functions

void recordOperation();

void viewHistory();

#endif
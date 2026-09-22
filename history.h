#ifndef HISTORY_H
#define HISTORY_H

#include <string>

struct History
{
    std::string operation;
};

struct HistoryNode
{
    History history;
    HistoryNode *next;
};

class HistoryStack
{
private:
    HistoryNode *top;

public:
    HistoryStack();

    void push(const History &history);
    bool pop();
    History *peek();
    void display();
};

// User-facing functions

void recordOperation();

void viewHistory();

void addHistory(const std::string &operation);

#endif
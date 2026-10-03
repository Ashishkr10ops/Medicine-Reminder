#ifndef HISTORY_H
#define HISTORY_H

#include <string>

// Stores one history operation
struct History
{
    std::string operation;
};

// Node used by the history stack
struct HistoryNode
{
    History history;
    HistoryNode *next;
};

// Stack used to maintain operation history
class HistoryStack
{
private:
    HistoryNode *top;

public:
    HistoryStack();

    // Stack operations
    void push(const History &history);
    bool pop();
    History *peek();
    void display();
};

// Global history stack
extern HistoryStack historyStack;

// User-facing functions
void recordOperation();
void viewHistory();
void addHistory(const std::string &operation);

#endif
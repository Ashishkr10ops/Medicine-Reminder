#ifndef ALERT_H
#define ALERT_H

#include <string>

// Stores one generated alert
struct Alert
{
    int medicineId;
    std::string message;
};

// Node used to store alerts in a linked list
struct AlertNode
{
    Alert alert;
    AlertNode *next;
};

// Manages the list of generated alerts
class AlertManager
{
private:
    AlertNode *head;

public:
    AlertManager();

    // Alert list operations
    void addAlert(const Alert &alert);
    void clearAlerts();
    void displayAlerts();
};

// User-facing alert functions
void checkAlerts();
void displayAlerts();

#endif
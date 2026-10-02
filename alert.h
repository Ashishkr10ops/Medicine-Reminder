#ifndef ALERT_H
#define ALERT_H

#include <string>

struct Alert
{
    int medicineId;
    std::string message;
};

struct AlertNode
{
    Alert alert;
    AlertNode *next;
};

class AlertManager
{
private:
    AlertNode *head;

public:
    AlertManager();

    void addAlert(const Alert &alert);
    void clearAlerts();
    void displayAlerts();
};

// Alert functions

void checkAlerts();
void displayAlerts();

#endif
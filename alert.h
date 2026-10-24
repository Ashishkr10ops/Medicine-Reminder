
#ifndef ALERT_H
#define ALERT_H

#include <string>

// Stores the medicine ID and message for one alert.
struct Alert
{
    int medicineId;
    std::string message;
};

// Node used to store alerts in a linked list.
struct AlertNode
{
    Alert alert;
    AlertNode *next;
};

// Manages alerts using a singly linked list.
class AlertManager
{
private:
    // Points to the first alert in the list.
    AlertNode *head;

public:
    // Initializes an empty alert list.
    AlertManager();

    // Adds a new alert to the end of the list.
    void addAlert(const Alert &alert);

    // Deletes all existing alerts and frees their memory.
    void clearAlerts();

    // Displays all alerts currently stored in the list.
    void displayAlerts();
};

// Checks inventory records and generates applicable alerts.
void checkAlerts();

// Displays the generated alerts.
void displayAlerts();

#endif

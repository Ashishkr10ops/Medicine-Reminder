#include "alert.h"
#include "inventory.h"
#include <iostream>
#include <vector>
#include <ctime>

AlertManager alertManager;

// Constructor
AlertManager::AlertManager()
{
    head = nullptr;
}

// Adds a new alert to the alert list
void AlertManager::addAlert(const Alert &alert)
{
    AlertNode *newNode = new AlertNode;

    newNode->alert = alert;
    newNode->next = nullptr;

    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    AlertNode *current = head;

    while (current->next != nullptr)
    {
        current = current->next;
    }

    current->next = newNode;
}

// Checks whether a medicine has expired
bool isExpired(const std::string &expiryDate)
{
    int year, month, day;

    // Expected format: YYYY-MM-DD
    sscanf(expiryDate.c_str(), "%d-%d-%d", &year, &month, &day);

    time_t currentTime = time(nullptr);
    tm *currentDate = localtime(&currentTime);

    int currentYear = currentDate->tm_year + 1900;
    int currentMonth = currentDate->tm_mon + 1;
    int currentDay = currentDate->tm_mday;

    if (year < currentYear)
    {
        return true;
    }

    if (year == currentYear && month < currentMonth)
    {
        return true;
    }

    if (year == currentYear &&
        month == currentMonth &&
        day < currentDay)
    {
        return true;
    }

    return false;
}

// Checks the inventory and generates alerts
void checkAlerts()
{
    const int LOW_STOCK_THRESHOLD = 5;

    // Remove alerts from the previous check
    alertManager.clearAlerts();

    std::vector<Medicine> medicines =
        inventoryTable.getAllMedicines();

    for (const Medicine &medicine : medicines)
    {

        // Check low stock
        if (medicine.quantity <= LOW_STOCK_THRESHOLD)
        {
            Alert alert;

            alert.medicineId = medicine.id;
            alert.message = "Low stock: " + medicine.name;

            alertManager.addAlert(alert);
        }

        // Check expiry
        if (isExpired(medicine.expiryDate))
        {
            Alert alert;

            alert.medicineId = medicine.id;
            alert.message = "Medicine expired: " + medicine.name;

            alertManager.addAlert(alert);
        }
    }

    std::cout << "Alert check completed.\n";
}

// Displays all generated alerts
void AlertManager::displayAlerts()
{
    if (head == nullptr)
    {
        std::cout << "\nNo alerts.\n";
        return;
    }

    std::cout << "\n===== Alerts =====\n";

    AlertNode *current = head;

    while (current != nullptr)
    {
        std::cout << "Medicine ID: "
                  << current->alert.medicineId << "\n";

        std::cout << "Alert: "
                  << current->alert.message << "\n";

        std::cout << "-----------------------------\n";

        current = current->next;
    }
}

// Removes all existing alerts
void AlertManager::clearAlerts()
{
    while (head != nullptr)
    {
        AlertNode *temp = head;
        head = head->next;

        delete temp;
    }
}

// Displays all currently generated alerts
void displayAlerts()
{
    alertManager.displayAlerts();
}

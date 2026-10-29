
#include "alert.h"
#include "inventory.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdio>
#include <string>

// Shared alert manager used by the application.
AlertManager alertManager;

// Initialize an empty linked list of alerts.
AlertManager::AlertManager()
{
    head = nullptr;
}

// Append an alert to the end of the linked list.
// Time complexity: O(n), because the list is traversed to find its last node.
void AlertManager::addAlert(const Alert &alert)
{
    AlertNode *newNode = new AlertNode;
    newNode->alert = alert;
    newNode->next = nullptr;

    // If the list is empty, the new node becomes the first node.
    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    // Find the last node and append the new alert.
    AlertNode *current = head;

    while (current->next != nullptr)
    {
        current = current->next;
    }

    current->next = newNode;
}

// Display all generated alerts by traversing the linked list.
// Time complexity: O(n), where n is the number of alerts.
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
                  << current->alert.medicineId << '\n';

        std::cout << "Alert: "
                  << current->alert.message << '\n';

        std::cout << "-----------------------------\n";

        current = current->next;
    }
}

// Delete every alert node and release its dynamically allocated memory.
void AlertManager::clearAlerts()
{
    while (head != nullptr)
    {
        AlertNode *temp = head;
        head = head->next;
        delete temp;
    }
}

// Return true if the supplied expiry date is before today's local date.
// Expected format: YYYY-MM-DD.
bool isExpired(const std::string &expiryDate)
{
    int year, month, day;
    char extra;

    // Parse the date and reject trailing characters.
    if (sscanf(expiryDate.c_str(), "%d-%d-%d%c",
               &year, &month, &day, &extra) != 3)
    {
        std::cout << "Invalid expiry date format. "
                  << "Use YYYY-MM-DD.\n";
        return false;
    }

    // Check basic month and day ranges before indexing daysInMonth.
    if (month < 1 || month > 12 ||
        day < 1 || day > 31)
    {
        std::cout << "Invalid expiry date.\n";
        return false;
    }

    // Number of days in each month; February is adjusted for leap years.
    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31};

    // A leap year is divisible by 400, or by 4 but not by 100.
    bool leapYear = (year % 400 == 0) ||
                    (year % 4 == 0 && year % 100 != 0);

    if (leapYear)
    {
        daysInMonth[1] = 29;
    }

    if (day > daysInMonth[month - 1])
    {
        std::cout << "Invalid expiry date.\n";
        return false;
    }

    // Obtain the current local date from the system clock.
    time_t currentTime = time(nullptr);
    tm *currentDate = localtime(&currentTime);

    if (currentDate == nullptr)
    {
        std::cout << "Unable to read the current date.\n";
        return false;
    }

    int currentYear = currentDate->tm_year + 1900;
    int currentMonth = currentDate->tm_mon + 1;
    int currentDay = currentDate->tm_mday;

    // Compare year, month, and day in chronological order.
    if (year < currentYear)
        return true;

    if (year == currentYear && month < currentMonth)
        return true;

    if (year == currentYear &&
        month == currentMonth &&
        day < currentDay)
        return true;

    // Today's date and future dates are not considered expired.
    return false;
}

// Generate alerts for low-stock and expired medicines.
void checkAlerts()
{
    const int LOW_STOCK_THRESHOLD = 5;

    // Rebuild the alert list so old alerts do not remain after inventory changes.
    alertManager.clearAlerts();

    // Obtain the current medicine records from the inventory hash table.
    std::vector<Medicine> medicines = inventoryTable.getAllMedicines();

    for (const Medicine &medicine : medicines)
    {
        // Alert when the available quantity is at or below the threshold.
        if (medicine.quantity <= LOW_STOCK_THRESHOLD)
        {
            Alert alert;
            alert.medicineId = medicine.id;
            alert.message = "Low stock: " + medicine.name;

            alertManager.addAlert(alert);
        }

        // Check expiry independently; one medicine can trigger both alerts.
        if (isExpired(medicine.expiryDate))
        {
            Alert alert;
            alert.medicineId = medicine.id;
            alert.message = "Medicine expired: " + medicine.name;

            alertManager.addAlert(alert);
        }
    }
}

// User-facing function to display the current alerts.
void displayAlerts()
{
    alertManager.displayAlerts();
}

// Release all remaining alert nodes when the manager is destroyed.
AlertManager::~AlertManager()
{
    clearAlerts();
}

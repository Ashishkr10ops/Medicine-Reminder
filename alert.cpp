
#include "alert.h"
#include "inventory.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdio>
#include <string>

// Define the shared alert manager.
AlertManager alertManager;

// Constructor: initialize the alert list as empty.
AlertManager::AlertManager()
{
    head = nullptr;
}

// Add a new alert to the linked list.
void AlertManager::addAlert(const Alert &alert)
{
    // Create a new node to store the alert.
    AlertNode *newNode = new AlertNode;

    newNode->alert = alert;
    newNode->next = nullptr;

    // If the list is empty, the new node becomes the head.
    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    // Traverse the list to find its last node.
    AlertNode *current = head;

    while (current->next != nullptr)
    {
        current = current->next;
    }

    // Link the new alert at the end of the list.
    current->next = newNode;
}

// Display all alerts stored in the linked list.
void AlertManager::displayAlerts()
{
    // Handle the case where no alerts exist.
    if (head == nullptr)
    {
        std::cout << "\nNo alerts.\n";
        return;
    }

    std::cout << "\n===== Alerts =====\n";

    // Start traversing from the first alert.
    AlertNode *current = head;

    while (current != nullptr)
    {
        // Display the medicine ID and its alert message.
        std::cout << "Medicine ID: "
                  << current->alert.medicineId << "\n";

        std::cout << "Alert: "
                  << current->alert.message << "\n";

        std::cout << "-----------------------------\n";

        // Move to the next alert node.
        current = current->next;
    }
}

// Remove every existing alert and release its memory.
void AlertManager::clearAlerts()
{
    while (head != nullptr)
    {
        // Temporarily save the first node.
        AlertNode *temp = head;

        // Move the head pointer to the next node.
        head = head->next;

        // Free the memory occupied by the removed node.
        delete temp;
    }
}

// Check whether a medicine's expiry date is before today's date.
// Expected date format: YYYY-MM-DD.
bool isExpired(const std::string &expiryDate)
{
    int year, month, day;
    char extra;

    // Extract the date and reject extra characters.
    if (sscanf(
            expiryDate.c_str(),
            "%d-%d-%d%c",
            &year, &month, &day, &extra) != 3)
    {
        std::cout << "Invalid expiry date format. "
                  << "Use YYYY-MM-DD.\n";
        return false;
    }

    // Validate the month and day ranges.
    if (month < 1 || month > 12 ||
        day < 1 || day > 31)
    {
        std::cout << "Invalid expiry date.\n";
        return false;
    }

    // Validate the number of days in the given month.
    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31};

    // Account for leap years.
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

    // Obtain the current local date.
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

    // Compare the expiry date with today's date.
    if (year < currentYear)
        return true;

    if (year == currentYear && month < currentMonth)
        return true;

    if (year == currentYear &&
        month == currentMonth &&
        day < currentDay)
        return true;

    return false;
}

// Check every medicine for low-stock and expiry conditions.
void checkAlerts()
{
    const int LOW_STOCK_THRESHOLD = 5;

    // Clear alerts from the previous check to avoid stale results.
    alertManager.clearAlerts();

    // Retrieve medicine records from the Inventory module.
    std::vector<Medicine> medicines =
        inventoryTable.getAllMedicines();

    // Examine each medicine independently.
    for (const Medicine &medicine : medicines)
    {
        // Generate an alert when the quantity is 5 or less.
        if (medicine.quantity <= LOW_STOCK_THRESHOLD)
        {
            Alert alert;

            alert.medicineId = medicine.id;
            alert.message =
                "Low stock: " + medicine.name;

            // Store the generated alert in the linked list.
            alertManager.addAlert(alert);
        }

        // Generate a separate alert if the medicine has expired.
        if (isExpired(medicine.expiryDate))
        {
            Alert alert;

            alert.medicineId = medicine.id;
            alert.message =
                "Medicine expired: " + medicine.name;

            // A medicine can have both low-stock and expiry alerts.
            alertManager.addAlert(alert);
        }
    }

    std::cout << "Alert check completed.\n";
}

// User-facing function to display the generated alerts.
void displayAlerts()
{
    alertManager.displayAlerts();
}

AlertManager::~AlertManager() {
    clearAlerts();
}

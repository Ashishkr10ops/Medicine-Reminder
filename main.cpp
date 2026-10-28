
#include <iostream>
#include <limits>

// Include the header files to access functions from each module.
#include "inventory.h"
#include "history.h"
#include "alert.h"
#include "reminder.h"
#include "conflict.h"
#include "design.h"

int main()
{
    initializeConflicts();

    int choice;

    // Load saved medicines when the program starts.
    if (!inventoryTable.loadInventory("medicines.txt"))
    {
        std::cout << "Warning: Inventory could not be loaded.\n";
    }

    // Load saved history when the program starts.
    historyStack.loadFromFile("history.txt");

    reminderQueue.loadFromFile("reminders.txt");

    // Repeat the menu until the user chooses 0 to exit.
    do
    {
        std::cout << YELLOW << BOLD
                  << "\n===============================================\n";
        std::cout << "   Medicine Reminder and Inventory Management\n";
        std::cout << "===============================================\n"
                  << RESET;

        // Inventory management
        std::cout << GREEN << BOLD
                  << "\n[ Inventory Management ]\n"
                  << RESET;
        std::cout << "1. Add Medicine\n";
        std::cout << "2. Search Medicine\n";
        std::cout << "3. Update Medicine\n";
        std::cout << "4. Delete Medicine\n";
        std::cout << "5. Display Inventory\n";

        // Reminder management
        std::cout << BLUE << BOLD
                  << "\n[ Reminder Management ]\n"
                  << RESET;
        std::cout << "6. Add Reminder\n";
        std::cout << "7. Remove Next Reminder\n";
        std::cout << "8. View Next Reminder\n";
        std::cout << "9. View All Reminders\n";

        // Alert management
        std::cout << RED << BOLD
                  << "\n[ Alerts ]\n"
                  << RESET;
        std::cout << "10. Display Alerts\n";

        // History
        std::cout << CYAN << BOLD
                  << "\n[ History ]\n"
                  << RESET;
        std::cout << "11. View History\n";

        std::cout << YELLOW << BOLD
                  << "\n0. Exit\n"
                  << RESET;

        // Read and validate the user's choice.
        std::cout << "\nEnter your choice: ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
        // Inventory module
        case 1:
            addMedicine();
            break;

        case 2:
            searchMedicine();
            break;

        case 3:
            updateMedicine();
            break;

        case 4:
            deleteMedicine();
            break;

        case 5:
            displayInventory();
            break;

            // Reminder module

        case 6:
        {
            int id;

            std::cout << "\nEnter Medicine ID: ";

            if (!(std::cin >> id))
            {
                std::cout << RED << "Invalid Medicine ID!\n"
                          << RESET;
                std::cin.clear();
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }

            Medicine *medicine = inventoryTable.searchMedicine(id);

            if (medicine == nullptr)
            {
                std::cout << RED
                          << "Medicine ID not found in inventory!\n"
                          << RESET;
                break;
            }

            // Check for conflicts with medicines already scheduled.
            std::vector<std::string> currentMeds =
                reminderQueue.getAllScheduledMedicines();

            if (conflictGraph.hasConflict(medicine->name, currentMeds))
            {
                std::cout << RED
                          << "Reminder not added due to a configured conflict.\n"
                          << RESET;
                break;
            }

            // Read and validate reminder time.
            std::string reminderTime;

            std::cout << "Enter Reminder Time (HH:MM): ";
            std::cin >> reminderTime;

            if (!isValidReminderTime(reminderTime))
            {
                std::cout << RED
                          << "Invalid reminder time! "
                          << "Use HH:MM format (00:00 to 23:59).\n"
                          << RESET;
                break;
            }

            // Create and add the reminder only after validation.
            Reminder reminder;
            reminder.medicineId = medicine->id;
            reminder.medicineName = medicine->name;
            reminder.reminderTime = reminderTime;

            reminderQueue.addReminder(reminder);

            std::cout << GREEN
                      << "Reminder added successfully for "
                      << medicine->name << "!\n"
                      << RESET;
            break;
        }

        case 7:
            removeReminder();
            break;

        case 8:
            viewNextReminder();
            break;

        case 9:
            viewAllReminders();
            break;

            // Alert module

        case 10:
            checkAlerts();   // Refresh alerts using current inventory
            displayAlerts(); // Display the refreshed alerts
            break;

        // History module
        case 11:
            viewHistory();
            break;

        // Exit
        case 0:
            if (inventoryTable.saveInventory("medicines.txt"))
            {
                std::cout << "Inventory saved successfully.\n";
            }
            else
            {
                std::cout << "Warning: Inventory could not be saved.\n";
            }

            historyStack.saveToFile("history.txt");

            reminderQueue.saveToFile("reminders.txt");

            std::cout << GREEN
                      << "\nExiting system... Have a healthy day!\n"
                      << RESET;
            break;

        default:
            std::cout << RED
                      << "\nInvalid choice. Please try again.\n"
                      << RESET;
        }

    } while (choice != 0);

    return 0;
}
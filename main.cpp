
#include <iostream>
#include <limits>

// Header files for the different modules of the application.
#include "inventory.h"
#include "history.h"
#include "alert.h"
#include "reminder.h"
#include "checkup.h"
#include "conflict.h"
#include "design.h"

int main()
{
    // Initialize the medicine interaction graph with known conflict pairs.
    initializeConflicts();

    int choice = -1;

    // Load the inventory stored in the file into the hash table.
    if (!inventoryTable.loadInventory("medicines.txt"))
    {
        std::cout << "Warning: Inventory could not be loaded.\n";
    }

    // Restore the history stack and reminder priority queue from files.
    historyStack.loadFromFile("history.txt");
    reminderQueue.loadFromFile("reminders.txt");

    // Update recurring reminders so that their scheduled dates are current.
    reminderQueue.updateRecurringReminders();

    // Display the main menu repeatedly until the user selects Exit (0).
    do
    {
        std::cout << YELLOW << BOLD
                  << "\n===============================================\n"
                  << "   Medicine Reminder and Inventory Management\n"
                  << "===============================================\n"
                  << RESET;

        // Inventory operations: add, search, update, delete, and display.
        std::cout << GREEN << BOLD
                  << "\n[ Inventory Management ]\n"
                  << RESET;
        std::cout << "1. Add Medicine\n";
        std::cout << "2. Search Medicine\n";
        std::cout << "3. Update Medicine\n";
        std::cout << "4. Delete Medicine\n";
        std::cout << "5. Display Inventory\n";

        // Reminder operations: manage scheduled medicine reminders.
        std::cout << BLUE << BOLD
                  << "\n[ Reminder Management ]\n"
                  << RESET;
        std::cout << "6. Add Reminder\n";
        std::cout << "7. Remove Reminder\n";
        std::cout << "8. View Next Reminder\n";
        std::cout << "9. View All Reminders\n";

        // Store and retrieve upcoming medical checkups.
        std::cout << BLUE << BOLD
                  << "\n[ Checkup Management ]\n"
                  << RESET;
        std::cout << "10. Add Next Checkup Date\n";
        std::cout << "11. View Next Checkup Date\n";

        // Check medicine-related alerts.
        std::cout << RED << BOLD
                  << "\n[ Alerts ]\n"
                  << RESET;
        std::cout << "12. Display Alerts\n";

        // View or clear the recorded operation history.
        std::cout << CYAN << BOLD
                  << "\n[ History ]\n"
                  << RESET;
        std::cout << "13. View History\n";
        std::cout << "14. Clear History\n";

        // Exit the application.
        std::cout << YELLOW << BOLD
                  << "\n0. Exit\n"
                  << RESET;

        // Read the menu choice and handle non-numeric input safely.
        std::cout << "\nEnter your choice: ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Invalid input. Please enter a number.\n";
            choice = -1;
            continue;
        }

        // Dispatch the selected operation to its corresponding module.
        switch (choice)
        {
        // Inventory management
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

        // Reminder management
        case 6:
            addReminder();
            break;

        case 7:
            removeReminder();
            break;

        case 8:
            viewNextReminder();
            break;

        case 9:
            viewAllReminders();
            break;

        // Checkup management
        case 10:
            addNextCheckupDate();
            break;

        case 11:
            viewNextCheckupDate();
            break;

        // Check alerts before displaying them.
        case 12:
            checkAlerts();
            displayAlerts();
            break;

        // History management
        case 13:
            viewHistory();
            break;

        case 14:
            clearHistory();
            break;

        // Save application data before terminating.
        case 0:
            if (!inventoryTable.saveInventory("medicines.txt"))
            {
                std::cout << "Warning: Inventory could not be saved.\n";
            }

            historyStack.saveToFile("history.txt");
            reminderQueue.saveToFile("reminders.txt");

            std::cout << GREEN
                      << "\nExiting system... Have a healthy day!\n"
                      << RESET;
            break;

        // Handle menu choices outside the supported range.
        default:
            std::cout << RED
                      << "\nInvalid choice. Please try again.\n"
                      << RESET;
        }

    } while (choice != 0);

    return 0;
}

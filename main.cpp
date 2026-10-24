
#include <iostream>

#include <limits>

// Include the header files to access functions from each module.
#include "inventory.h"
#include "history.h"
#include "alert.h"
#include "reminder.h"

int main()
{
    int choice;

    // Load saved medicines when the program starts.
    if (!inventoryTable.loadInventory("medicines.txt"))
    {
        std::cout << "Warning: Inventory could not be loaded.\n";
    }

    // Repeat the menu until the user chooses 0 to exit.
    do
    {
        std::cout << "\n===============================================\n";
        std::cout << "   Medicine Reminder and Inventory Management\n";
        std::cout << "===============================================\n";

        // Display inventory management options.
        std::cout << "\n----- Inventory Management -----\n";
        std::cout << "1. Add Medicine\n";
        std::cout << "2. Search Medicine\n";
        std::cout << "3. Update Medicine\n";
        std::cout << "4. Delete Medicine\n";
        std::cout << "5. Display Inventory\n";

        // Display reminder management options.
        std::cout << "\n----- Reminder Management -----\n";
        std::cout << "6. Add Reminder\n";
        std::cout << "7. Remove Next Reminder\n";
        std::cout << "8. View Next Reminder\n";
        std::cout << "9. View All Reminders\n";

        // Display history options.
        std::cout << "\n----- History -----\n";
        std::cout << "10. View History\n";

        // Display alert options.
        std::cout << "\n----- Alerts -----\n";
        std::cout << "11. Check Alerts\n";
        std::cout << "12. Display Alerts\n";

        std::cout << "\n0. Exit\n";

        // Read the user's menu selection and validate the input.
        std::cout << "\nEnter your choice: ";

        if (!(std::cin >> choice))
        {
            // Clear the failed input state.
            std::cin.clear();

            // Discard the invalid input from the current line.
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Invalid input. Please enter a number.\n";

            continue;
        }

        // Execute the operation corresponding to the selected option.
        switch (choice)
        {
        // Inventory module: manage medicine records.
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

        // Reminder module: manage medicine reminders.
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

        // History module: display recorded operations.
        case 10:
            viewHistory();
            break;

        // Alert module: check and display medicine alerts.
        case 11:
            checkAlerts();
            break;

        case 12:
            displayAlerts();
            break;

        // Exit the program.
        case 0:
            // Save inventory before exiting.
            if (inventoryTable.saveInventory("medicines.txt"))
            {
                std::cout << "Inventory saved successfully.\n";
            }

            std::cout << "\nExiting system...\n";
            break;

        // Handle an invalid menu selection.
        default:
            std::cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
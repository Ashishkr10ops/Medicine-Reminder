#include <iostream>

#include "inventory.h"
#include "history.h"
#include "alert.h"
#include "reminder.h"

int main()
{

    int choice;

    do
    {
        std::cout << "\n===============================================\n";
        std::cout << "   Medicine Reminder and Inventory Management\n";
        std::cout << "===============================================\n";

        // Inventory module
        std::cout << "\n----- Inventory Management -----\n";
        std::cout << "1. Add Medicine\n";
        std::cout << "2. Search Medicine\n";
        std::cout << "3. Update Medicine\n";
        std::cout << "4. Delete Medicine\n";
        std::cout << "5. Display Inventory\n";

        // Reminder module
        std::cout << "\n----- Reminder Management -----\n";
        std::cout << "6. Add Reminder\n";
        std::cout << "7. Remove Next Reminder\n";
        std::cout << "8. View Next Reminder\n";
        std::cout << "9. View All Reminders\n";

        // History module
        std::cout << "\n----- History -----\n";
        std::cout << "10. View History\n";

        // Alert module
        std::cout << "\n----- Alerts -----\n";
        std::cout << "11. Check Alerts\n";
        std::cout << "12. Display Alerts\n";

        std::cout << "\n0. Exit\n";

        std::cout << "\nEnter your choice: ";
        std::cin >> choice;

        switch (choice)
        {

        // Inventory
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

        // Reminder
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

        // History
        case 10:
            viewHistory();
            break;

        // Alerts
        case 11:
            checkAlerts();
            break;

        case 12:
            displayAlerts();
            break;

        case 0:
            std::cout << "\nExiting system...\n";
            break;

        default:
            std::cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
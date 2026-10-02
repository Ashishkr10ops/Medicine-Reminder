#include <iostream>

#include "inventory.h"
#include "history.h"
#include "alert.h"

int main()
{
    int choice;

    do
    {
        std::cout << "\n===== Medicine Reminder and Inventory Management =====\n";
        std::cout << "1. Inventory Management\n";
        std::cout << "2. Search Medicine\n";
        std::cout << "3. Update Medicine\n";
        std::cout << "4. Delete Medicine\n";
        std::cout << "5. Display Inventory\n";
        std::cout << "6. View History\n";
        std::cout << "7. Check Alerts\n";
        std::cout << "8. Display Alerts\n";
        std::cout << "0. Exit\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {

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

        case 6:
            viewHistory();
            break;

        case 7:
            checkAlerts();
            break;

        case 8:
            displayAlerts();
            break;

        case 0:
            std::cout << "Exiting system...\n";
            break;

        default:
            std::cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
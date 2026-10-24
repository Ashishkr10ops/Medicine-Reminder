
#include <iostream>

#include <limits>

// Include the header files to access functions from each module.
#include "inventory.h"
#include "history.h"
#include "alert.h"
#include "reminder.h"
#include "conflict.h"
#include "storage.h"
#include "patient.h"
#include "disease.h"
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

    // Repeat the menu until the user chooses 0 to exit.
    do
    {
        std::cout << YELLOW << BOLD << "\n===============================================\n";
        std::cout << "   Medicine Reminder and Inventory Management\n";
        std::cout << "===============================================\n" << RESET;

        // Display inventory management options.
        std::cout << "\n----- Inventory Management -----\n";
        std::cout << GREEN << BOLD << "\n[ Inventory Management ]" << RESET << "\n";
        std::cout << "1. Add Medicine\n";
        std::cout << "2. Search Medicine\n";
        std::cout << "3. Update Medicine\n";
        std::cout << "4. Delete Medicine\n";
        std::cout << "5. Display Inventory\n";

        // Display reminder management options.
        std::cout << "\n----- Reminder Management -----\n";
        std::cout << BLUE << BOLD << "\n[ Reminder Management ]" << RESET << "\n";
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
        std::cout << CYAN << BOLD << "\n[ Patient Management ]" << RESET << "\n";
        std::cout << "10. Add Patient Profile\n";
        std::cout << "11. View Patient Profile\n";
        std::cout << "12. Display All Patients\n";

        std::cout << YELLOW << BOLD << "\n[ Disease & Treatment ]" << RESET << "\n";
        std::cout << "13. Link Medicine to a Disease\n";
        std::cout << "14. View Treatments for a Disease\n";
        std::cout << "15. Suggest Medicine for Patient\n";

        std::cout << RED << BOLD << "\n[ Alerts & Data ]" << RESET << "\n";
        std::cout << "16. Check Alerts\n";
        std::cout << "17. Display Alerts\n";
        std::cout << "18. Backup Inventory\n";
        std::cout << "19. View History\n";

        std::cout << BOLD << "\n0. Exit\n" << RESET;

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

        // Patient management
        case 10:
            patientManager.addPatient();
            break;

        case 11:
            patientManager.viewPatient();
            break;

        case 12:
            patientManager.displayAllPatients();
            break;

        // Disease and treatment management
        case 13:
            diseaseManager.addTreatment();
            break;

        case 14:
            diseaseManager.viewTreatments();
            break;

        case 15:
            diseaseManager.suggestForPatient();
            break;

        // Alerts
        case 16:
            checkAlerts();
            break;

        case 17:
            displayAlerts();
            break;

        // Inventory and history
        case 18:
            saveInventory();
            break;

        case 19:
            viewHistory();
            break;

        // Exit
        case 0:
            if (inventoryTable.saveInventory("medicines.txt"))
            {
                std::cout << "Inventory saved successfully.\n";
            }

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
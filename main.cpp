#include <iostream>

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

    do
    {
        std::cout << YELLOW << BOLD << "\n===============================================\n";
        std::cout << "   Medicine Reminder and Inventory Management\n";
        std::cout << "===============================================\n" << RESET;

        std::cout << GREEN << BOLD << "\n[ Inventory Management ]" << RESET << "\n";
        std::cout << "1. Add Medicine\n";
        std::cout << "2. Search Medicine\n";
        std::cout << "3. Update Medicine\n";
        std::cout << "4. Delete Medicine\n";
        std::cout << "5. Display Inventory\n";

        std::cout << BLUE << BOLD << "\n[ Reminder Management ]" << RESET << "\n";
        std::cout << "6. Add Reminder\n";
        std::cout << "7. Remove Next Reminder\n";
        std::cout << "8. View Next Reminder\n";
        std::cout << "9. View All Reminders\n";

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

        std::cout << "\nEnter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1: addMedicine(); break;
        case 2: searchMedicine(); break;
        case 3: updateMedicine(); break;
        case 4: deleteMedicine(); break;
        case 5: displayInventory(); break;
        case 6: addReminder(); break;
        case 7: removeReminder(); break;
        case 8: viewNextReminder(); break;
        case 9: viewAllReminders(); break;
        case 10: patientManager.addPatient(); break;
        case 11: patientManager.viewPatient(); break;
        case 12: patientManager.displayAllPatients(); break;
        case 13: diseaseManager.addTreatment(); break;
        case 14: diseaseManager.viewTreatments(); break;
        case 15: diseaseManager.suggestForPatient(); break;
        case 16: checkAlerts(); break;
        case 17: displayAlerts(); break;
        case 18: saveInventory(); break;
        case 19: viewHistory(); break;
        case 0:
            std::cout << GREEN << "\nExiting system... Have a healthy day!\n" << RESET;
            break;
        default:
            std::cout << RED << "\nInvalid choice. Please try again.\n" << RESET;
        }

    } while (choice != 0);

    return 0;
}
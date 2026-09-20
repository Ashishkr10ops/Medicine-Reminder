#ifndef INVENTORY_H

#define INVENTORY_H

#include <string>

//
struct Medicine
{
    int id;
    std::string name;
    int quantity;
    std::string expiryDate;
};

// Function declaration

void addMedicine();
void searchMedicine();
void updateMedicine();
void deleteMedicine();
void displayInventory();

#endif

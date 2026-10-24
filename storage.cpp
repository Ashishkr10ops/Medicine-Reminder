#include "storage.h"
#include "inventory.h"
#include <fstream>
#include <iostream>

void saveInventory() {
    std::ofstream file("inventory_backup.txt");
    if (!file) {
        std::cout << "Error creating backup file.\n";
        return;
    }
    std::vector<Medicine> meds = inventoryTable.getAllMedicines();
    for (const Medicine& m : meds) {
        file << m.id << "," << m.name << "," << m.quantity << "," << m.expiryDate << "\n";
    }
    file.close();
    std::cout << "Inventory safely backed up to disk.\n";
}
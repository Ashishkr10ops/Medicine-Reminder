#include "inventory.h"
#include <iostream>

HashTable inventoryTable;

// HashTable functions
HashTable::HashTable()
{

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = nullptr;
    }
}


int HashTable::hashFunction(int id)
{
    return id % TABLE_SIZE;
}

// Insert medicine
void HashTable::insertMedicine(const Medicine &medicine)
{

    int index = hashFunction(medicine.id);

    Node *newNode = new Node;
    newNode->medicine = medicine;
    newNode->next = nullptr;

    if (table[index] == nullptr)
    {
        table[index] = newNode;
        return;
    }

    Node *current = table[index];

    while (current->next != nullptr)
    {
        current = current->next;
    }

    current->next = newNode;
}

// Search medicine
Medicine *HashTable::searchMedicine(int id)
{

    int index = hashFunction(id);

    Node *current = table[index];

    while (current != nullptr)
    {

        if (current->medicine.id == id)
        {
            return &current->medicine;
        }

        current = current->next;
    }

    return nullptr;
}

// Updates quantity and expiry date of a medicine
bool HashTable::updateMedicine(int id, int quantity, const std::string &expiryDate)
{
    Medicine *medicine = searchMedicine(id);

    // Medicine not found
    if (medicine == nullptr)
    {
        return false;
    }

    // Update the existing medicine
    medicine->quantity = quantity;
    medicine->expiryDate = expiryDate;

    return true;
}

// Delete a medicine
bool HashTable::deleteMedicine(int id)
{

    int index = hashFunction(id);

    Node *current = table[index];
    Node *previous = nullptr;

    while (current != nullptr)
    {

        // Medicine found
        if (current->medicine.id == id)
        {

            // Case 1: Delete the first node
            if (previous == nullptr)
            {
                table[index] = current->next;
            }

            // Case 2: Delete a node after the first node
            else
            {
                previous->next = current->next;
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    // Medicine not found
    return false;
}

// Displays all medicines in the inventory
void HashTable::displayInventory()
{

    bool isEmpty = true;

    std::cout << "\n===== Medicine Inventory =====\n";

    for (int i = 0; i < TABLE_SIZE; i++)
    {

        Node *current = table[i];

        while (current != nullptr)
        {
            isEmpty = false;

            std::cout << "ID: " << current->medicine.id << "\n";
            std::cout << "Name: " << current->medicine.name << "\n";
            std::cout << "Quantity: " << current->medicine.quantity << "\n";
            std::cout << "Expiry Date: " << current->medicine.expiryDate << "\n";
            std::cout << "-----------------------------\n";

            current = current->next;
        }

        if (isEmpty)
        {
            std::cout << "Inventory is empty.\n";
        }
    }
}

// User-facing functions

// Adds a new medicine to the inventory
void addMedicine() {

    Medicine medicine;

    std::cout << "\nEnter Medicine ID: ";
    std::cin >> medicine.id;

    std::cout << "Enter Medicine Name: ";
    std::cin >> medicine.name;

    std::cout << "Enter Quantity: ";
    std::cin >> medicine.quantity;

    std::cout << "Enter Expiry Date: ";
    std::cin >> medicine.expiryDate;

    inventoryTable.insertMedicine(medicine);

    std::cout << "Medicine added successfully.\n";
}

// Searches for a medicine using its ID
void searchMedicine() {

    int id;

    std::cout << "\nEnter Medicine ID to search: ";
    std::cin >> id;

    Medicine* medicine = inventoryTable.searchMedicine(id);

    if (medicine == nullptr) {
        std::cout << "Medicine not found.\n";
        return;
    }

    std::cout << "\nMedicine Found\n";
    std::cout << "ID: " << medicine->id << "\n";
    std::cout << "Name: " << medicine->name << "\n";
    std::cout << "Quantity: " << medicine->quantity << "\n";
    std::cout << "Expiry Date: " << medicine->expiryDate << "\n";
}

// Updates an existing medicine
void updateMedicine() {

    int id;
    int quantity;
    std::string expiryDate;

    std::cout << "\nEnter Medicine ID to update: ";
    std::cin >> id;

    std::cout << "Enter new Quantity: ";
    std::cin >> quantity;

    std::cout << "Enter new Expiry Date: ";
    std::cin >> expiryDate;

    bool updated = inventoryTable.updateMedicine(id, quantity, expiryDate);

    if (updated)
    {
        std::cout << "Medicine updated successfully.\n";
    }
    else
    {
        std::cout << "Medicine not found.\n";
    }
}

// Deletes a medicine from the inventory
void deleteMedicine() {

    int id;

    std::cout << "\nEnter Medicine ID to delete: ";
    std::cin >> id;

    bool deleted = inventoryTable.deleteMedicine(id);

    if (deleted)
    {
        std::cout << "Medicine deleted successfully.\n";
    }
    else
    {
        std::cout << "Medicine not found.\n";
    }
}

// Displays the complete inventory
void displayInventory()
{

    inventoryTable.displayInventory();
}
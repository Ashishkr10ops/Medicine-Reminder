#include <iostream>
#include "inventory.h"

// Constructor
HashTable::HashTable()
{

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = nullptr;
    }
}

// Hash function
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
void HashTable::displayInventory() {

    bool isEmpty = true;

    std::cout << "\n===== Medicine Inventory =====\n";

    for (int i = 0; i<TABLE_SIZE; i++) {

        Node *current = table[i];

        while (current != nullptr) {
            isEmpty = false;

            std::cout << "ID: " << current->medicine.id << "\n";
            std::cout << "Name: " << current->medicine.name << "\n";
            std::cout << "Quantity: " << current->medicine.quantity << "\n";
            std::cout << "Expiry Date: " << current->medicine.expiryDate << "\n";
            std::cout << "-----------------------------\n";

            current = current->next;
        }

        if (isEmpty) {
            std::cout << "Inventory is empty.\n";
        }
    }
}

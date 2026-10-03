#include "inventory.h"
#include "history.h"

#include <iostream>

HashTable inventoryTable;

// Constructor
// Initializes every hash table bucket as empty.
HashTable::HashTable()
{

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = nullptr;
    }
}

// Hash function
// Maps a medicine ID to a bucket in the hash table.
int HashTable::hashFunction(int id)
{
    return id % TABLE_SIZE;
}

// Inserts a medicine into the hash table.
// Separate chaining is used to handle collisions.
void HashTable::insertMedicine(const Medicine &medicine)
{

    int index = hashFunction(medicine.id);

    Node *newNode = new Node;

    newNode->medicine = medicine;
    newNode->next = nullptr;

    // If the bucket is empty, insert directly.
    if (table[index] == nullptr)
    {
        table[index] = newNode;
        return;
    }

    // Otherwise, traverse the linked list
    // and insert the new node at the end.
    Node *current = table[index];

    while (current->next != nullptr)
    {
        current = current->next;
    }

    current->next = newNode;
}

// Searches for a medicine using its ID.
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

// Updates the quantity and expiry date of a medicine.
bool HashTable::updateMedicine(
    int id,
    int quantity,
    const std::string &expiryDate)
{

    Medicine *medicine = searchMedicine(id);

    if (medicine == nullptr)
    {
        return false;
    }

    medicine->quantity = quantity;
    medicine->expiryDate = expiryDate;

    return true;
}

// Deletes a medicine from the hash table.
bool HashTable::deleteMedicine(int id)
{

    int index = hashFunction(id);

    Node *current = table[index];
    Node *previous = nullptr;

    while (current != nullptr)
    {

        if (current->medicine.id == id)
        {

            // Deleting the first node of the bucket
            if (previous == nullptr)
            {
                table[index] = current->next;
            }
            else
            {
                // Removing a node from the middle/end
                previous->next = current->next;
            }

            delete current;

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

// Displays all medicines stored in the hash table.
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

            std::cout << "ID: "
                      << current->medicine.id << "\n";

            std::cout << "Name: "
                      << current->medicine.name << "\n";

            std::cout << "Quantity: "
                      << current->medicine.quantity << "\n";

            std::cout << "Expiry Date: "
                      << current->medicine.expiryDate << "\n";

            std::cout << "-----------------------------\n";

            current = current->next;
        }
    }

    if (isEmpty)
    {
        std::cout << "Inventory is empty.\n";
    }
}

// Returns all medicines stored in the hash table.
// Used by the Alert module to check stock and expiry.
std::vector<Medicine> HashTable::getAllMedicines()
{

    std::vector<Medicine> medicines;

    for (int i = 0; i < TABLE_SIZE; i++)
    {

        Node *current = table[i];

        while (current != nullptr)
        {

            medicines.push_back(current->medicine);

            current = current->next;
        }
    }

    return medicines;
}

// User-facing function to add a medicine.
void addMedicine()
{

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

    // Record the operation in the history stack.
    addHistory("Added medicine: " + medicine.name);

    std::cout << "Medicine added successfully.\n";
}

// User-facing function to search for a medicine.
void searchMedicine()
{

    int id;

    std::cout << "\nEnter Medicine ID to search: ";
    std::cin >> id;

    Medicine *medicine = inventoryTable.searchMedicine(id);

    if (medicine == nullptr)
    {
        std::cout << "Medicine not found.\n";
        return;
    }

    std::cout << "\nMedicine Found\n";

    std::cout << "ID: "
              << medicine->id << "\n";

    std::cout << "Name: "
              << medicine->name << "\n";

    std::cout << "Quantity: "
              << medicine->quantity << "\n";

    std::cout << "Expiry Date: "
              << medicine->expiryDate << "\n";
}

// User-facing function to update a medicine.
void updateMedicine()
{

    int id;
    int quantity;
    std::string expiryDate;

    std::cout << "\nEnter Medicine ID to update: ";
    std::cin >> id;

    std::cout << "Enter new Quantity: ";
    std::cin >> quantity;

    std::cout << "Enter new Expiry Date: ";
    std::cin >> expiryDate;

    bool updated =
        inventoryTable.updateMedicine(id, quantity, expiryDate);

    if (updated)
    {

        // Record the operation in the history stack.
        addHistory("Updated medicine ID: " +
                   std::to_string(id));

        std::cout << "Medicine updated successfully.\n";
    }
    else
    {
        std::cout << "Medicine not found.\n";
    }
}

// User-facing function to delete a medicine.
void deleteMedicine()
{

    int id;

    std::cout << "\nEnter Medicine ID to delete: ";
    std::cin >> id;

    bool deleted = inventoryTable.deleteMedicine(id);

    if (deleted)
    {

        // Record the operation in the history stack.
        addHistory("Deleted medicine ID: " +
                   std::to_string(id));

        std::cout << "Medicine deleted successfully.\n";
    }
    else
    {
        std::cout << "Medicine not found.\n";
    }
}

// Displays the complete inventory.
void displayInventory()
{
    inventoryTable.displayInventory();
}
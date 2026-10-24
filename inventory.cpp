
#include "inventory.h"
#include "history.h"

#include <iostream>
#include <fstream>
#include <string>
#include <limits>

// Define the shared inventory object declared in inventory.h.
HashTable inventoryTable;

// Constructor: initialize every bucket as empty.
HashTable::HashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = nullptr;
    }
}

// Calculate the bucket index using the medicine ID.
int HashTable::hashFunction(int id)
{
    // The remainder maps the ID to an index from 0 to TABLE_SIZE - 1.
    return id % TABLE_SIZE;
}

// Insert a medicine using separate chaining to handle collisions.
void HashTable::insertMedicine(const Medicine &medicine)
{
    // Find the bucket where this medicine should be stored.
    int index = hashFunction(medicine.id);

    // Create a new linked-list node for the medicine.
    Node *newNode = new Node;
    newNode->medicine = medicine;
    newNode->next = nullptr;

    // If the bucket is empty, make the new node its first node.
    if (table[index] == nullptr)
    {
        table[index] = newNode;
        return;
    }

    // A collision occurred: traverse the existing linked list.
    Node *current = table[index];

    while (current->next != nullptr)
    {
        current = current->next;
    }

    // Attach the new node at the end of the chain.
    current->next = newNode;
}

// Search for a medicine by ID.
Medicine *HashTable::searchMedicine(int id)
{
    // Search only the bucket associated with this ID.
    int index = hashFunction(id);
    Node *current = table[index];

    // Traverse the chain until the medicine is found or the list ends.
    while (current != nullptr)
    {
        if (current->medicine.id == id)
        {
            // Return the address of the stored medicine.
            return &current->medicine;
        }

        current = current->next;
    }

    // nullptr indicates that the medicine does not exist.
    return nullptr;
}

// Update the quantity and expiry date of an existing medicine.
bool HashTable::updateMedicine(
    int id,
    int quantity,
    const std::string &expiryDate)
{
    // Reuse the search function to locate the medicine.
    Medicine *medicine = searchMedicine(id);

    if (medicine == nullptr)
    {
        return false;
    }

    // Modify the record directly using its pointer.
    medicine->quantity = quantity;
    medicine->expiryDate = expiryDate;

    return true;
}

// Delete a medicine from its hash table bucket.
bool HashTable::deleteMedicine(int id)
{
    int index = hashFunction(id);

    // current points to the node being checked.
    // previous tracks the node immediately before current.
    Node *current = table[index];
    Node *previous = nullptr;

    while (current != nullptr)
    {
        if (current->medicine.id == id)
        {
            // If previous is nullptr, the first node is being deleted.
            if (previous == nullptr)
            {
                table[index] = current->next;
            }
            else
            {
                // Bypass the target node to unlink it from the chain.
                previous->next = current->next;
            }

            // Release the memory occupied by the deleted node.
            delete current;

            return true;
        }

        // Move both pointers forward through the linked list.
        previous = current;
        current = current->next;
    }

    // The requested medicine ID was not found.
    return false;
}

// Display all medicines by traversing every bucket and its chain.
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

    // Print a message if no medicine records were found.
    if (isEmpty)
    {
        std::cout << "Inventory is empty.\n";
    }
}

// Collect all medicine records for the Alert module.
std::vector<Medicine> HashTable::getAllMedicines()
{
    std::vector<Medicine> medicines;

    // Visit every bucket in the hash table.
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Node *current = table[i];

        // Copy each medicine from the linked list into the vector.
        while (current != nullptr)
        {
            medicines.push_back(current->medicine);
            current = current->next;
        }
    }

    // Return the collected records to the calling module.
    return medicines;
}

bool HashTable::saveInventory(const std::string &filename)
{
    std::ofstream file(filename);

    if (!file)
    {
        std::cout << "Error: Unable to save inventory.\n";
        return false;
    }

    // Save one medicine per line.
    for (const Medicine &medicine : getAllMedicines())
    {
        file << medicine.id << '\n'
             << medicine.name << '\n'
             << medicine.quantity << '\n'
             << medicine.expiryDate << '\n';
    }

    return true;
}

bool HashTable::loadInventory(const std::string &filename)
{
    std::ifstream file(filename);

    // A missing file is normal on the first run.
    if (!file)
    {
        return true;
    }

    Medicine medicine;

    // Read each medicine's four fields.
    while (file >> medicine.id)
    {
        file.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

        if (!std::getline(file, medicine.name) ||
            !(file >> medicine.quantity) ||
            !(file >> medicine.expiryDate))
        {
            std::cout << "Error: Invalid inventory file.\n";
            return false;
        }

        file.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

        // Avoid inserting the same ID more than once.
        if (searchMedicine(medicine.id) == nullptr)
        {
            insertMedicine(medicine);
        }
    }

    return true;
}

// User-facing function to add a medicine.
void addMedicine()
{
    Medicine medicine;

    // Step 1: Read the ID.
    std::cout << "Enter Medicine ID: ";
    std::cin >> medicine.id;

    // Step 2: Check the ID immediately.
    if (inventoryTable.searchMedicine(medicine.id) != nullptr)
    {
        std::cout << "Medicine ID already exists. "
                  << "Use a unique ID.\n";
        return;
    }

    // Step 3: Read details only if the ID is unique.
    std::cout << "Enter Medicine Name: ";
    std::getline(std::cin >> std::ws, medicine.name);

    std::cout << "Enter Quantity: ";
    std::cin >> medicine.quantity;

    std::cout << "Enter Expiry Date (YYYY-MM-DD): ";
    std::cin >> medicine.expiryDate;

    // Step 4: Insert and record the operation.
    inventoryTable.insertMedicine(medicine);
    addHistory("Added medicine: " + medicine.name);

    std::cout << "Medicine added successfully.\n";
}

// User-facing function to search for a medicine.
void searchMedicine()
{
    int id;

    std::cout << "\nEnter Medicine ID to search: ";
    std::cin >> id;

    // Get a pointer to the matching medicine, if it exists.
    Medicine *medicine = inventoryTable.searchMedicine(id);

    if (medicine == nullptr)
    {
        std::cout << "Medicine not found.\n";
        return;
    }

    // Display the details of the matching medicine.
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

    // Step 1: Ask for the medicine ID.
    std::cout << "Enter Medicine ID to update: ";
    std::cin >> id;

    // Step 2: Search for the medicine.
    Medicine *medicine = inventoryTable.searchMedicine(id);

    // Step 3: Stop if the medicine does not exist.
    if (medicine == nullptr)
    {
        std::cout << "Medicine not found.\n";
        return;
    }

    // Step 4: Ask for new details only if found.
    std::cout << "Enter new Quantity: ";
    std::cin >> medicine->quantity;

    std::cout << "Enter new Expiry Date (YYYY-MM-DD): ";
    std::cin >> medicine->expiryDate;

    // Step 5: Record the successful update.
    addHistory("Updated medicine: " + medicine->name);

    std::cout << "Medicine updated successfully.\n";
}

// User-facing function to delete a medicine.
void deleteMedicine()
{
    int id;

    std::cout << "\nEnter Medicine ID to delete: ";
    std::cin >> id;

    // Attempt to remove the medicine from the hash table.
    bool deleted = inventoryTable.deleteMedicine(id);

    if (deleted)
    {
        // Record the operation only if deletion succeeds.
        addHistory("Deleted medicine ID: " +
                   std::to_string(id));

        std::cout << "Medicine deleted successfully.\n";
    }
    else
    {
        std::cout << "Medicine not found.\n";
    }
}

// Wrapper function used by the main menu to display inventory.
void displayInventory()
{
    inventoryTable.displayInventory();
}

HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

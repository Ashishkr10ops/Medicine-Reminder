
#include "inventory.h"
#include "history.h"
#include "reminder.h"

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>

// Define the shared inventory object declared in inventory.h.
HashTable inventoryTable;

// Constructor: initialize every bucket as empty.
HashTable::HashTable() : nextMedicineId(100)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = nullptr;
    }

    // Restore the next ID from the previous run.
    std::ifstream file("medicine_id_counter.txt");

    if (file >> nextMedicineId && nextMedicineId < 100)
    {
        nextMedicineId = 100;
    }
}

// Generate a unique medicine ID and save the counter.
int HashTable::generateMedicineId()
{
    // Skip IDs that are already present in the inventory.
    while (searchMedicine(nextMedicineId) != nullptr)
    {
        nextMedicineId++;
    }

    int generatedId = nextMedicineId;
    nextMedicineId++;

    // Persist the counter so deleted IDs are not reused.
    std::ofstream file("medicine_id_counter.txt");

    if (file)
    {
        file << nextMedicineId;
    }
    else
    {
        std::cout << "Warning: Unable to save medicine ID counter.\n";
    }

    return generatedId;
}

// Calculate the bucket index using the medicine ID.
int HashTable::hashFunction(int id)
{
    return ((id % TABLE_SIZE) + TABLE_SIZE) % TABLE_SIZE;
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

bool isValidDateFormat(const std::string &date)
{
    if (date.length() != 10 ||
        date[4] != '-' || date[7] != '-')
        return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;

        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            return false;
    }

    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (year < 1 || month < 1 || month > 12)
        return false;

    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31};

    bool leapYear = (year % 400 == 0) ||
                    (year % 4 == 0 && year % 100 != 0);

    if (leapYear)
        daysInMonth[1] = 29;

    return day >= 1 && day <= daysInMonth[month - 1];
}

bool isValidExpiryDate(const std::string &date)
{
    if (!isValidDateFormat(date))
        return false;

    // Reject dates before today's local date
    // when adding or updating a medicine.
    std::time_t now = std::time(nullptr);
    std::tm *today = std::localtime(&now);

    if (today == nullptr)
        return false;

    std::ostringstream currentDate;
    currentDate << std::put_time(today, "%Y-%m-%d");

    return date >= currentDate.str();
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

        // Stop if writing fails.
        if (!file)
        {
            std::cout << "Error: Failed while writing inventory.\n";
            return false;
        }
    }

    file.close();

    // Check whether closing/flushing the file failed.
    if (!file)
    {
        std::cout << "Error: Failed to finish saving inventory.\n";
        return false;
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

    int loaded = 0;
    int skipped = 0;

    while (true)
    {
        Medicine medicine;

        // Read the medicine ID.
        if (!(file >> medicine.id))
        {
            if (file.eof())
                break;

            std::cout << "Error: Invalid medicine ID in inventory file.\n";
            return false;
        }

        file.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

        // Read the remaining fields.
        if (!std::getline(file, medicine.name) ||
            !(file >> medicine.quantity) ||
            !(file >> medicine.expiryDate))
        {
            std::cout << "Error: Incomplete inventory record.\n";
            return false;
        }

        file.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

        // Validate the record.
        if (medicine.id <= 0 ||
            medicine.name.empty() ||
            medicine.quantity < 0 ||
            !isValidDateFormat(medicine.expiryDate))
        {
            std::cout << "Skipping invalid medicine record.\n";
            skipped++;
            continue;
        }

        // Avoid duplicate IDs.
        if (searchMedicine(medicine.id) != nullptr)
        {
            std::cout << "Skipping duplicate medicine ID: "
                      << medicine.id << '\n';
            skipped++;
            continue;
        }

        insertMedicine(medicine);
        loaded++;
    }

    if (file.bad())
    {
        std::cout << "Error: Failed while reading inventory file.\n";
        return false;
    }

    std::cout << loaded << " medicine(s) loaded.";

    if (skipped > 0)
    {
        std::cout << " " << skipped
                  << " invalid or duplicate record(s) skipped.";
    }

    std::cout << '\n';

    return true;
}

// User-facing function to add a medicine.
void addMedicine()
{
    Medicine medicine;

    // Generate and display the medicine ID automatically.
    medicine.id = inventoryTable.generateMedicineId();

    std::cout << "Generated Medicine ID: "
              << medicine.id << '\n';

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

    if (!(std::cin >> medicine.quantity) || medicine.quantity < 0)
    {
        std::cout << "Invalid quantity. Please enter a non-negative integer.\n";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

        return;
    }

    std::cout << "Enter Expiry Date (YYYY-MM-DD): ";
    std::cin >> medicine.expiryDate;

    if (!isValidExpiryDate(medicine.expiryDate))
    {
        std::cout << "Invalid expiry date. "
                  << "Please use a valid YYYY-MM-DD date.\n";
        return;
    }

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

    if (!(std::cin >> id))
    {
        std::cout << "Invalid input. Please enter a numeric Medicine ID.\n";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

        return;
    }

    // Medicine IDs must be positive.
    if (id <= 0)
    {
        std::cout << "Invalid Medicine ID. ID must be positive.\n";
        return;
    }

    // Search for the medicine.
    Medicine *medicine = inventoryTable.searchMedicine(id);

    if (medicine == nullptr)
    {
        std::cout << "Medicine not found.\n";
        return;
    }

    // Display medicine details.
    std::cout << "\nMedicine Found\n";

    std::cout << "ID: " << medicine->id << "\n";
    std::cout << "Name: " << medicine->name << "\n";
    std::cout << "Quantity: " << medicine->quantity << "\n";
    std::cout << "Expiry Date: " << medicine->expiryDate << "\n";
}

// User-facing function to update a medicine.

void updateMedicine()
{
    int id;

    // Step 1: Ask for the medicine ID.
    std::cout << "Enter Medicine ID to update: ";

    if (!(std::cin >> id))
    {
        std::cout << "Invalid medicine ID.\n";
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    // Step 2: Search for the medicine.
    Medicine *medicine = inventoryTable.searchMedicine(id);

    if (medicine == nullptr)
    {
        std::cout << "Medicine not found.\n";
        return;
    }

    // Step 3: Read the new quantity into a temporary variable.
    int newQuantity;

    std::cout << "Enter new Quantity: ";

    if (!(std::cin >> newQuantity) || newQuantity < 0)
    {
        std::cout << "Invalid quantity. "
                  << "Please enter a non-negative integer.\n";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    // Step 4: Read and validate the new expiry date.
    std::string newExpiryDate;

    std::cout << "Enter new Expiry Date (YYYY-MM-DD): ";
    std::cin >> newExpiryDate;

    if (!isValidExpiryDate(newExpiryDate))
    {
        std::cout << "Invalid expiry date. "
                  << "Please enter a valid YYYY-MM-DD date.\n";
        return;
    }

    // Step 5: Update only after both inputs are valid.
    medicine->quantity = newQuantity;
    medicine->expiryDate = newExpiryDate;

    // Step 6: Record the successful update.
    addHistory("Updated medicine: " + medicine->name);

    std::cout << "Medicine updated successfully.\n";
}

// User-facing function to delete a medicine.

void deleteMedicine()
{
    int id;

    std::cout << "\nEnter Medicine ID to delete: ";

    if (!(std::cin >> id))
    {
        std::cout << "Invalid input. Please enter a numeric Medicine ID.\n";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

        return;
    }

    // Medicine IDs must be positive.
    if (id <= 0)
    {
        std::cout << "Invalid Medicine ID. ID must be positive.\n";
        return;
    }

    // Attempt to remove the medicine from the hash table.
    bool deleted = inventoryTable.deleteMedicine(id);

    if (deleted)
    {
        int removedReminders =
            reminderQueue.removeRemindersForMedicine(id);

        std::cout << "Medicine deleted successfully.\n";
        std::cout << removedReminders
                  << " associated reminder(s) removed.\n";

        addHistory("Deleted medicine ID: " +
                   std::to_string(id));
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

HashTable::~HashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Node *current = table[i];
        while (current != nullptr)
        {
            Node *temp = current;
            current = current->next;
            delete temp;
        }
    }
}

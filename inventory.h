
#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>

// Stores the details of a medicine.
struct Medicine
{
    int id;
    std::string name;
    int quantity;
    std::string expiryDate;
};

// Each node stores one medicine and a pointer to the next node.
// Nodes are linked together to handle hash table collisions.
struct Node
{
    Medicine medicine;
    Node *next;
};

// Hash table for storing and managing medicine records.
class HashTable
{
private:
    // Total number of buckets in the hash table.
    static const int TABLE_SIZE = 10;

    // Each bucket points to the first node of its linked list.
    Node *table[TABLE_SIZE];

    // Converts a medicine ID into a valid bucket index.
    int hashFunction(int id);

public:
    // Initializes all hash table buckets as empty.
    HashTable();

    // Adds a medicine to the appropriate bucket.
    void insertMedicine(const Medicine &medicine);

    // Searches for a medicine by ID; returns nullptr if not found.
    Medicine *searchMedicine(int id);

    // Updates quantity and expiry date; returns success status.
    bool updateMedicine(int id, int quantity,
                        const std::string &expiryDate);

    // Deletes a medicine by ID; returns success status.
    bool deleteMedicine(int id);

    // Displays every medicine stored in the hash table.
    void displayInventory();

    // Returns all medicine records for use by other modules.
    std::vector<Medicine> getAllMedicines();

    // Saves all medicines to a file.
    bool saveInventory(const std::string &filename);

    // Loads medicines from a file.
    bool loadInventory(const std::string &filename);
    ~HashTable(); // Destructor to prevent memory leak
};

// Shared inventory object used across the project.
// The actual object is defined in inventory.cpp.
extern HashTable inventoryTable;

// User-facing functions called from the main menu.
void addMedicine();
void searchMedicine();
void updateMedicine();
void deleteMedicine();
void displayInventory();

#endif

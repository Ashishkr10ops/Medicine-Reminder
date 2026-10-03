#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>

// Stores information about a medicine
struct Medicine
{
    int id;
    std::string name;
    int quantity;
    std::string expiryDate;
};

// Node used for separate chaining in the hash table
struct Node
{
    Medicine medicine;
    Node *next;
};

// Hash table used for inventory management
class HashTable
{
private:
    static const int TABLE_SIZE = 10;

    Node *table[TABLE_SIZE];

    // Calculates the bucket index for a medicine ID
    int hashFunction(int id);

public:
    HashTable();

    // Basic hash table operations
    void insertMedicine(const Medicine &medicine);
    Medicine *searchMedicine(int id);
    bool updateMedicine(int id, int quantity,
                        const std::string &expiryDate);
    bool deleteMedicine(int id);

    // Displays all medicines
    void displayInventory();

    // Returns all medicines for other modules
    std::vector<Medicine> getAllMedicines();
};

// Global inventory table
extern HashTable inventoryTable;

// User-facing inventory functions
void addMedicine();
void searchMedicine();
void updateMedicine();
void deleteMedicine();
void displayInventory();

#endif
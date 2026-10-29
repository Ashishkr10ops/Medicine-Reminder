
#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>

// Stores all information associated with a medicine.
struct Medicine
{
    int id;                 // Unique identifier for the medicine.
    std::string name;       // Name of the medicine.
    int quantity;           // Available stock.
    std::string expiryDate; // Expiry date in YYYY-MM-DD format.
};

// Node used in the linked list for separate chaining.
// Multiple nodes can belong to the same hash table bucket.
struct Node
{
    Medicine medicine; // Medicine record stored in this node.
    Node *next;        // Pointer to the next node in the chain.
};

// Hash table that manages medicine records using medicine IDs.
// Collisions are handled using separate chaining with linked lists.
class HashTable
{
private:
    // Number of buckets in the hash table.
    static const int TABLE_SIZE = 10;

    // Each array element points to the head of a linked list.
    Node *table[TABLE_SIZE];

    // Counter used to generate medicine IDs automatically.
    int nextMedicineId;

    // Maps a medicine ID to a valid bucket index.
    // The hash function is implemented in inventory.cpp.
    int hashFunction(int id);

public:
    // Initializes the hash table with empty buckets.
    HashTable();

    // Generates the next medicine ID.
    int generateMedicineId();

    // Inserts a medicine into its corresponding hash table bucket.
    // Separate chaining handles collisions between medicine IDs.
    void insertMedicine(const Medicine &medicine);

    // Searches for a medicine by ID.
    // Returns a pointer to the record, or nullptr if it is not found.
    Medicine *searchMedicine(int id);

    // Updates the quantity and expiry date of an existing medicine.
    // Returns true if the update succeeds; otherwise, false.
    bool updateMedicine(int id, int quantity,
                        const std::string &expiryDate);

    // Removes a medicine from the hash table.
    // Returns true if the medicine is deleted successfully.
    bool deleteMedicine(int id);

    // Displays all medicine records stored in the hash table.
    void displayInventory();

    // Returns a vector containing the current medicine records.
    // Useful when other modules need to access inventory data.
    std::vector<Medicine> getAllMedicines();

    // Saves the inventory records to the specified file.
    // Returns the success status of the file operation.
    bool saveInventory(const std::string &filename);

    // Loads inventory records from the specified file.
    // Returns the success status of the loading operation.
    bool loadInventory(const std::string &filename);

    // Releases dynamically allocated linked-list nodes.
    // Prevents memory leaks when the hash table is destroyed.
    ~HashTable();
};

// Shared inventory instance used by other project modules.
// Its definition is located in inventory.cpp.
extern HashTable inventoryTable;

// Functions called by the main menu for inventory operations.
void addMedicine();
void searchMedicine();
void updateMedicine();
void deleteMedicine();
void displayInventory();

// Validates a calendar date in YYYY-MM-DD format.
bool isValidDateFormat(const std::string &date);

#endif

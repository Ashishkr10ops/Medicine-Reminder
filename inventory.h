#ifndef INVENTORY_H

#define INVENTORY_H

#include <string>

struct Medicine
{
    int id;
    std::string name;
    int quantity;
    std::string expiryDate;
};

struct Node
{
    Medicine medicine;
    Node *next;
};

class HashTable
{
private:
    static const int TABLE_SIZE = 10;
    Node *table[TABLE_SIZE];

    int hashFunction(int id);

public:
    // Constructor
    HashTable();

    // Adds a medicine to the hash table.
    void insertMedicine(const Medicine &medicine);
    Medicine* searchMedicine(int id);
    bool updateMedicine(int id, int quantity, const std::string& expiryDate);
    bool deleteMedicine(int id);
    void displayInventory();

};

// Inventory module functions

void addMedicine();
void searchMedicine();
void updateMedicine();
void deleteMedicine();
void displayInventory();

#endif

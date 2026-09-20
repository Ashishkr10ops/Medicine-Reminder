#include <iostream>

#include "inventory.h"
#include "reminder.h"
#include "alert.h"
#include "history.h"

using namespace std;

int main()
{
    cout << "=== MEDICINE REMINDER SYSTEM ===" << endl;

    addMedicine();
    searchMedicine();
    
    return 0;
}
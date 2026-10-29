Medicine Reminder and Inventory Management System
A robust, terminal-based C++ application designed to manage pharmacy inventories, schedule patient medicine reminders, and track medical profiles. This project utilizes advanced data structures (Graphs, Min-Heaps, Hash Maps, Stacks, and Linked Lists) to ensure efficient data processing and safety checks like drug-conflict prevention.

How to Compile and Run
To run this program locally, you will need a C++ compiler (like GCC) installed on your system.
Compile the code: Open your terminal in the project folder and run the following command to compile all C++ files together:


g++ *.cpp -o med_reminder
Run the application:
On Windows: .\med_reminder.exe
On Mac/Linux: ./med_reminder

Instruction Manual (How to Use)
When you launch the application, you will be greeted with a color-coded main menu. Navigate the system by typing the number corresponding to your desired action and pressing Enter.
[ Inventory Management ]
1. Add Medicine: Add a new drug to the system by providing an ID, name, quantity, and expiry date.
2. Search Medicine: Manage your existing stock using the unique Medicine ID.
3. Update Medicine: Modify the quantity and expiry date of an existing medicine using its unique ID.
4. Delete Medicine: Remove a medicine from the inventory using its unique ID.
5. Display Inventory: View a list of all medicines currently stored in the system.
[ Reminder Management ]
6. Add Reminder: Schedule a time for a patient to take a medicine. Safety Feature: The system will automatically check if the new medicine conflicts with any currently scheduled medicines and block it if a dangerous interaction is found.
7. Remove Reminder: Remove all reminders associated with a selected medicine ID.
8. View Next Reminder: The system prioritizes reminders based on time. Use option 8 to see the very next reminder due.
9. View All Reminders: Display all scheduled reminders in chronological order.
[ Checkup Management ]
10. Add Next Checkup Date: Collects checkup details (date, time, doctor, hospital), validates the input, and saves the record.
11. View Next Checkup Date: Displays the earliest checkup scheduled for today or a future date.
[ Alerts ]
12. Display Alerts: Scans the inventory to warn you if any medicines are below a quantity of 5 or have expired.
[ History ]
13. View History: Displays a log of the most recent operations you've performed (like adding or deleting medicines).
14. Clear History: Removes all history entries and saves the empty history.

File Architecture (Role of Each File)
This project is modular, meaning different features are split into their own specific files for clean organization. Each module consists of a .h (Header) file for definitions and a .cpp (Source) file for the actual logic.
Core System
main.cpp: The central hub of the program. It ties all the other files together, displays the visual terminal menu, and routes the user's input to the correct functions.
design.h: Stores ANSI escape codes used to add styling and text colors to the terminal output.
Inventory Module (Hash Map & Linked List)
inventory.h / inventory.cpp: Manages the pharmacy stock. Uses a Hash Table for quick insertions and lookups. It utilizes a Linked List to handle collisions (when two medicines generate the same hash index).
Reminder Module (Min-Heap)
reminder.h / reminder.cpp: Handles scheduling. Uses a Min-Heap (Priority Queue) to ensure that the reminder with the earliest time is always at the top and the first to be displayed/removed.
Safety & Conflict Module (Graph)
conflict.h / conflict.cpp: The medical safety checker. Uses an Adjacency List Graph to map out dangerous drug interactions. Before a reminder is added, it traverses the graph to ensure the new drug doesn't fatally conflict with a currently scheduled drug.
Alerts Module (Linked List)
alert.h / alert.cpp: Scans for low stock and expired medicines. It stores these warnings dynamically in a Linked List so the user can review them.
History Module (Stack)
history.h / history.cpp: Tracks user actions. Uses a Stack (LIFO - Last In, First Out) to store operations, ensuring the most recent action is always viewed first.
Checkup Module
checkup.h / checkup.cpp: Store and retrieve upcoming medical checkups. Writes all supplied checkup records to the persistence file checkups.txt.

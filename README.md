Medicine Reminder and Inventory Management System

A robust, terminal-based C++ application designed to manage pharmacy inventories, schedule patient medicine reminders, and track medical profiles. This project utilizes advanced data structures (Graphs, Min-Heaps, Hash Maps, Stacks, and Linked Lists) to ensure efficient data processing and safety checks like drug-conflict prevention.
->How to Compile and Run:
To run this program locally, you will need a C++ compiler (like GCC) installed on your system.
1. Compile the code: Open your terminal in the project folder and run the following command to compile all C++ files together:
g++ *.cpp -o med_reminder

2. Run the application:
On Windows: .\med_reminder.exe
On Mac/Linux: ./med_reminder
->Instruction Manual (How to Use)
When you launch the application, you will be greeted with a color-coded main menu. Navigate the system by typing the number corresponding to your desired action and pressing Enter.

[ Inventory Management ]

1. Add Medicine: Add a new drug to the system by providing an ID, name, quantity, and expiry date.
2. Search, Update, Delete: Manage your existing stock using the unique Medicine ID. 
5. Display Inventory: View a list of all medicines currently stored in the system. (3 to 5)

[ Reminder Management ]

6. Add Reminder: Schedule a time for a patient to take a medicine. Safety Feature: The system will automatically check if the new medicine conflicts with any currently scheduled medicines and block it if a dangerous interaction is found.
7. View & Remove Reminders: The system prioritizes reminders based on time. Use option 8 to see the very next reminder due. (7 to 9)

[ Patient Management ]

10. Add Patient Profile: Register a new patient with their ID, name, age, and primary disease.
11. View Patients: Instantly look up a patient's medical profile using their ID. (11 to 12)

[ Disease & Treatment ]

13. Link Medicine to a Disease: Map a specific medicine as a treatment for a specific disease (e.g., Link "Paracetamol" to "Fever").
14. View Treatments: Type in a disease to see all linked medicines.
15. Suggest Medicine for Patient: Enter a Patient ID, and the system will automatically look up their diagnosed disease and suggest the appropriate linked medicines.

[ Alerts & Data ]

16. Alerts: Scans the inventory to warn you if any medicines are below a quantity of 5 or have expired. (16-17)
18. Backup Inventory: Saves your current inventory to your computer's hard drive (inventory_backup.txt) so it isn't lost when you close the program.
19. View History: Displays a log of the most recent operations you've performed (like adding or deleting medicines).

->File Architecture (Role of Each File)
This project is modular, meaning different features are split into their own specific files for clean organization. Each module consists of a .h (Header) file for definitions and a .cpp (Source) file for the actual logic.

Core System
main.cpp: The central hub of the program. It ties all the other files together, displays the visual terminal menu, and routes the user's input to the correct functions.
design.h: Stores ANSI escape codes used to add styling and text colors to the terminal output.

Inventory Module (Hash Map & Linked List)
inventory.h / inventory.cpp: Manages the pharmacy stock. Uses a Hash Table for quick insertions and lookups. It utilizes a Linked List to handle collisions (when two medicines generate the same hash index).

Reminder Module (Min-Heap)
reminder.h / reminder.cpp: Handles scheduling. Uses a Min-Heap (Priority Queue) to ensure that the reminder with the earliest time is always at the top and the first to be displayed/removed.

Patient Module (Hash Map)
patient.h / patient.cpp: Stores user profiles. Uses an std::unordered_map (Hash Map) for O(1) time complexity lookups, meaning you can find a patient instantly by their ID regardless of how many patients are registered.

Disease Module (Hash Map)
disease.h / disease.cpp: Maps illnesses to treatments. Uses a Hash Map containing string keys (the disease) and vector values (the list of medicines) to suggest appropriate treatments for patients.

Safety & Conflict Module (Graph)
conflict.h / conflict.cpp: The medical safety checker. Uses an Adjacency List Graph to map out dangerous drug interactions. Before a reminder is added, it traverses the graph to ensure the new drug doesn't fatally conflict with a currently scheduled drug.

Alerts Module (Linked List)
alert.h / alert.cpp: Scans for low stock and expired medicines. It stores these warnings dynamically in a Linked List so the user can review them.

History Module (Stack)
history.h / history.cpp: Tracks user actions. Uses a Stack (LIFO - Last In, First Out) to store operations, ensuring the most recent action is always viewed first.

Storage Module (File I/O)
storage.h / storage.cpp: Handles data persistence. Writes the current Hash Map inventory data to a physical text file (inventory_backup.txt) on the user's hard drive so data survives after the program exits.

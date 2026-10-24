

#include "reminder.h"
#include "conflict.h"

#include <iostream>
#include <utility>

// Define the shared reminder queue.
ReminderQueue reminderQueue;

// Constructor: the vector is initially empty.
ReminderQueue::ReminderQueue()
{
}

// Move a newly inserted reminder upward to restore min-heap order.
void ReminderQueue::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        // Stop if the parent has an earlier or equal reminder time.
        if (heap[parent].reminderTime <= heap[index].reminderTime)
        {
            break;
        }

        std::swap(heap[parent], heap[index]);
        index = parent;
    }
}

// Insert a reminder into the min-heap.
void ReminderQueue::addReminder(const Reminder &reminder)
{
    heap.push_back(reminder);
    heapifyUp(static_cast<int>(heap.size()) - 1);
}

// Move a node downward to restore min-heap order.
void ReminderQueue::heapifyDown(int index)
{
    int size = static_cast<int>(heap.size());

    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size &&
            heap[left].reminderTime < heap[smallest].reminderTime)
        {
            smallest = left;
        }

        if (right < size &&
            heap[right].reminderTime < heap[smallest].reminderTime)
        {
            smallest = right;
        }

        // Stop if the current node is already the smallest.
        if (smallest == index)
        {
            break;
        }

        std::swap(heap[index], heap[smallest]);
        index = smallest;
    }
}

// Remove the earliest reminder from the min-heap.
bool ReminderQueue::removeReminder()
{
    if (heap.empty())
    {
        return false;
    }

    // Replace the root with the last element.
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty())
    {
        heapifyDown(0);
    }

    return true;
}

// Return the earliest reminder without removing it.
Reminder *ReminderQueue::getNextReminder()
{
    if (heap.empty())
    {
        return nullptr;
    }

    return &heap[0];
}

// Display all reminders in the current heap arrangement.
void ReminderQueue::displayReminders()
{
    if (heap.empty())
    {
        std::cout << "No reminders available.\n";
        return;
    }

    std::cout << "\n===== Reminders =====\n";

    for (const Reminder &reminder : heap)
    {
        std::cout << "Medicine ID: " << reminder.medicineId << "\n";
        std::cout << "Medicine Name: " << reminder.medicineName << "\n";
        std::cout << "Reminder Time: " << reminder.reminderTime << "\n";
        std::cout << "-----------------------------\n";
    }
}

// Return the medicine names currently present in the reminder queue.
std::vector<std::string> ReminderQueue::getAllScheduledMedicines()
{
    std::vector<std::string> medicines;

    for (const Reminder &reminder : heap)
    {
        medicines.push_back(reminder.medicineName);
    }

    return medicines;
}

// Add a reminder after checking for known drug conflicts.
void addReminder()
{
    Reminder reminder;

    std::cout << "\nEnter Medicine ID: ";
    if (!(std::cin >> reminder.medicineId))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid medicine ID.\n";
        return;
    }

    std::cout << "Enter Medicine Name: ";
    std::getline(std::cin >> std::ws, reminder.medicineName);

    std::cout << "Enter Reminder Time (HH:MM, 24-hour format): ";
    std::cin >> reminder.reminderTime;

    // Compare the new medicine with medicines already scheduled.
    std::vector<std::string> currentMeds =
        reminderQueue.getAllScheduledMedicines();

    if (conflictGraph.hasConflict(reminder.medicineName, currentMeds))
    {
        std::cout << "Reminder aborted due to a potential drug interaction.\n";
        return;
    }

    reminderQueue.addReminder(reminder);
    std::cout << "Reminder added successfully.\n";
}

// Remove the earliest reminder.
void removeReminder()
{
    Reminder *next = reminderQueue.getNextReminder();

    if (next == nullptr)
    {
        std::cout << "No reminders available.\n";
        return;
    }

    // Print the details before removing the reminder.
    std::cout << "\nRemoving reminder for " << next->medicineName
              << " at " << next->reminderTime << ".\n";

    if (reminderQueue.removeReminder())
    {
        std::cout << "Reminder removed successfully.\n";
    }
}

// Display the earliest reminder without removing it.
void viewNextReminder()
{
    Reminder *next = reminderQueue.getNextReminder();

    if (next == nullptr)
    {
        std::cout << "No reminders available.\n";
        return;
    }

    std::cout << "\n===== Next Reminder =====\n";
    std::cout << "Medicine ID: " << next->medicineId << "\n";
    std::cout << "Medicine Name: " << next->medicineName << "\n";
    std::cout << "Reminder Time: " << next->reminderTime << "\n";
}

// Display all reminders.
void viewAllReminders()
{
    reminderQueue.displayReminders();
}

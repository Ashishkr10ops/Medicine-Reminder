#include "reminder.h"

#include <iostream>
#include <utility>

ReminderQueue reminderQueue;

// Constructor
// Initially, the heap is empty.
ReminderQueue::ReminderQueue()
{
}

// Restores the Min Heap property by moving
// a newly inserted element upward.
void ReminderQueue::heapifyUp(int index)
{

    while (index > 0)
    {

        // Find the parent of the current node.
        int parent = (index - 1) / 2;

        // If the parent is already smaller,
        // the Min Heap property is satisfied.
        if (heap[parent].reminderTime <=
            heap[index].reminderTime)
        {
            break;
        }

        // Otherwise, swap the current node with its parent.
        std::swap(heap[parent], heap[index]);

        index = parent;
    }
}

// Adds a new reminder to the Min Heap.
void ReminderQueue::addReminder(
    const Reminder &reminder)
{

    // Insert the new reminder at the end.
    heap.push_back(reminder);

    // Restore the Min Heap property.
    heapifyUp(heap.size() - 1);
}

// Restores the Min Heap property by moving
// an element downward.
void ReminderQueue::heapifyDown(int index)
{

    while (true)
    {

        // Calculate the positions of the children.
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int smallest = index;

        // Check whether the left child is smaller.
        if (left < heap.size() &&
            heap[left].reminderTime <
                heap[smallest].reminderTime)
        {

            smallest = left;
        }

        // Check whether the right child is smaller.
        if (right < heap.size() &&
            heap[right].reminderTime <
                heap[smallest].reminderTime)
        {

            smallest = right;
        }

        // If the current node is already the smallest,
        // the Min Heap property is satisfied.
        if (smallest == index)
        {
            break;
        }

        // Swap with the smaller child.
        std::swap(heap[index], heap[smallest]);

        index = smallest;
    }
}

// Removes the earliest reminder from the Min Heap.
bool ReminderQueue::removeReminder()
{

    if (heap.empty())
    {
        return false;
    }

    // Move the last element to the root.
    heap[0] = heap.back();

    // Remove the duplicate last element.
    heap.pop_back();

    // Restore the Min Heap property.
    if (!heap.empty())
    {
        heapifyDown(0);
    }

    return true;
}

// Returns the earliest reminder.
Reminder *ReminderQueue::getNextReminder()
{

    if (heap.empty())
    {
        return nullptr;
    }

    // The minimum element is always at the root.
    return &heap[0];
}

// Displays all reminders currently stored in the heap.
void ReminderQueue::displayReminders()
{

    if (heap.empty())
    {
        std::cout << "\nNo reminders available.\n";
        return;
    }

    std::cout << "\n===== Reminders =====\n";

    for (const Reminder &reminder : heap)
    {

        std::cout << "Medicine ID: "
                  << reminder.medicineId << "\n";

        std::cout << "Medicine: "
                  << reminder.medicineName << "\n";

        std::cout << "Reminder Time: "
                  << reminder.reminderTime << "\n";

        std::cout << "-----------------------------\n";
    }
}

// User-facing function to add a reminder.
void addReminder()
{

    Reminder reminder;

    std::cout << "\nEnter Medicine ID: ";
    std::cin >> reminder.medicineId;

    std::cout << "Enter Medicine Name: ";
    std::cin >> reminder.medicineName;

    std::cout << "Enter Reminder Time (HH:MM): ";
    std::cin >> reminder.reminderTime;

    reminderQueue.addReminder(reminder);

    std::cout << "Reminder added successfully.\n";
}

// Removes the earliest reminder.
void removeReminder()
{

    Reminder *reminder =
        reminderQueue.getNextReminder();

    if (reminder == nullptr)
    {
        std::cout << "\nNo reminders available.\n";
        return;
    }

    std::cout << "\nRemoving reminder for "
              << reminder->medicineName
              << " at "
              << reminder->reminderTime
              << ".\n";

    reminderQueue.removeReminder();

    std::cout << "Reminder removed successfully.\n";
}

// Displays the earliest reminder.
void viewNextReminder()
{

    Reminder *reminder =
        reminderQueue.getNextReminder();

    if (reminder == nullptr)
    {
        std::cout << "\nNo reminders available.\n";
        return;
    }

    std::cout << "\n===== Next Reminder =====\n";

    std::cout << "Medicine ID: "
              << reminder->medicineId << "\n";

    std::cout << "Medicine: "
              << reminder->medicineName << "\n";

    std::cout << "Reminder Time: "
              << reminder->reminderTime << "\n";
}

// Displays all reminders.
void viewAllReminders()
{
    reminderQueue.displayReminders();
}
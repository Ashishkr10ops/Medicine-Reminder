
#include "reminder.h"

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
    // Continue while the current node has a parent.
    while (index > 0)
    {
        // Calculate the parent index in the array-based heap.
        int parent = (index - 1) / 2;

        // Stop if the parent already has an earlier or equal time.
        if (heap[parent].reminderTime <= heap[index].reminderTime)
        {
            break;
        }

        // Swap the child with its parent when heap order is violated.
        std::swap(heap[parent], heap[index]);

        // Continue checking from the parent's previous position.
        index = parent;
    }
}

// Insert a reminder into the min-heap.
void ReminderQueue::addReminder(const Reminder &reminder)
{
    // Add the new reminder at the end of the vector.
    heap.push_back(reminder);

    // Restore heap order by moving the new element upward.
    heapifyUp(static_cast<int>(heap.size()) - 1);
}

// Move a node downward to restore min-heap order.
void ReminderQueue::heapifyDown(int index)
{
    int size = static_cast<int>(heap.size());

    while (true)
    {
        // Calculate the indices of the left and right children.
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        // Assume the current node has the earliest reminder.
        int smallest = index;

        // Check whether the left child has an earlier reminder.
        if (left < size && heap[left].reminderTime < heap[smallest].reminderTime)
        {
            smallest = left;
        }

        // Check whether the right child has an earlier reminder.
        if (right < size && heap[right].reminderTime < heap[smallest].reminderTime)
        {
            smallest = right;
        }

        // Stop if the current node is already the smallest.
        if (smallest == index)
        {
            break;
        }

        // Swap with the child having the earliest reminder.
        std::swap(heap[index], heap[smallest]);

        // Continue from the child's position.
        index = smallest;
    }
}

// Remove the earliest reminder from the min-heap.
bool ReminderQueue::removeReminder()
{
    // An empty heap has no reminder to remove.
    if (heap.empty())
    {
        return false;
    }

    // Replace the root with the last element.
    heap[0] = heap.back();

    // Remove the duplicate last element.
    heap.pop_back();

    // Restore heap order if reminders remain.
    if (!heap.empty())
    {
        heapifyDown(0);
    }

    return true;
}

// Return the earliest reminder without removing it.
Reminder *ReminderQueue::getNextReminder()
{
    // The root contains the earliest reminder.
    if (heap.empty())
    {
        return nullptr;
    }

    return &heap[0];
}

// Display reminders in the current heap arrangement.
void ReminderQueue::displayReminders()
{
    if (heap.empty())
    {
        std::cout << "No reminders available.\n";
        return;
    }

    // Heap order does not necessarily mean the entire list is sorted.
    for (const Reminder &reminder : heap)
    {
        std::cout << "Medicine ID: "
                  << reminder.medicineId << "\n";

        std::cout << "Medicine Name: "
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
    std::getline(std::cin >> std::ws, reminder.medicineName);

    // Enter time in zero-padded 24-hour format, such as 09:05.
    std::cout << "Enter Reminder Time (HH:MM): ";
    std::cin >> reminder.reminderTime;

    // Insert the reminder into the priority queue.
    reminderQueue.addReminder(reminder);

    std::cout << "Reminder added successfully.\n";
}

// User-facing function to remove the earliest reminder.
void removeReminder()
{
    // Get the reminder at the root before removing it.
    Reminder *next = reminderQueue.getNextReminder();

    if (next == nullptr)
    {
        std::cout << "No reminders available.\n";
        return;
    }

    // Display the reminder that will be removed.
    std::cout << "Removing reminder for "
              << next->medicineName
              << " at " << next->reminderTime << "\n";

    // Remove the root and restore min-heap order.
    reminderQueue.removeReminder();
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

    std::cout << "\nNext Reminder\n";

    std::cout << "Medicine ID: "
              << next->medicineId << "\n";

    std::cout << "Medicine Name: "
              << next->medicineName << "\n";

    std::cout << "Reminder Time: "
              << next->reminderTime << "\n";
}

// Display all reminders.
void viewAllReminders()
{
    reminderQueue.displayReminders();
}

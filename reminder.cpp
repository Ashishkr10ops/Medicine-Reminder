#include "reminder.h"
#include "conflict.h" 
#include <iostream>
#include <utility>

ReminderQueue reminderQueue;

ReminderQueue::ReminderQueue()
{
}

void ReminderQueue::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;
        if (heap[parent].reminderTime <= heap[index].reminderTime)
        {
            break;
        }
        std::swap(heap[parent], heap[index]);
        index = parent;
    }
}

void ReminderQueue::addReminder(const Reminder &reminder)
{
    heap.push_back(reminder);
    heapifyUp(heap.size() - 1);
}

void ReminderQueue::heapifyDown(int index)
{
    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap.size() && heap[left].reminderTime < heap[smallest].reminderTime)
        {
            smallest = left;
        }

        if (right < heap.size() && heap[right].reminderTime < heap[smallest].reminderTime)
        {
            smallest = right;
        }

        if (smallest == index)
        {
            break;
        }

        std::swap(heap[index], heap[smallest]);
        index = smallest;
    }
}

bool ReminderQueue::removeReminder()
{
    if (heap.empty())
    {
        return false;
    }
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty())
    {
        heapifyDown(0);
    }
    return true;
}

Reminder *ReminderQueue::getNextReminder()
{
    if (heap.empty())
    {
        return nullptr;
    }
    return &heap[0];
}

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
        std::cout << "Medicine ID: " << reminder.medicineId << "\n";
        std::cout << "Medicine: " << reminder.medicineName << "\n";
        std::cout << "Reminder Time: " << reminder.reminderTime << "\n";
        std::cout << "-----------------------------\n";
    }
}

// New addition to pull current medicines for conflict checking
std::vector<std::string> ReminderQueue::getAllScheduledMedicines() {
    std::vector<std::string> meds;
    for (const Reminder& r : heap) {
        meds.push_back(r.medicineName);
    }
    return meds;
}

void addReminder()
{
    Reminder reminder;

    std::cout << "\nEnter Medicine ID: ";
    std::cin >> reminder.medicineId;
    std::cout << "Enter Medicine Name: ";
    std::cin >> reminder.medicineName;
    std::cout << "Enter Reminder Time (HH:MM): ";
    std::cin >> reminder.reminderTime;

    // New addition: Check for drug conflicts before adding
    std::vector<std::string> currentMeds = reminderQueue.getAllScheduledMedicines();
    if (conflictGraph.hasConflict(reminder.medicineName, currentMeds)) {
        std::cout << "Reminder aborted due to dangerous drug interaction.\n";
        return;
    }

    reminderQueue.addReminder(reminder);
    std::cout << "Reminder added successfully.\n";
}

void removeReminder()
{
    Reminder *reminder = reminderQueue.getNextReminder();

    if (reminder == nullptr)
    {
        std::cout << "\nNo reminders available.\n";
        return;
    }

    std::cout << "\nRemoving reminder for " << reminder->medicineName
              << " at " << reminder->reminderTime << ".\n";

    reminderQueue.removeReminder();
    std::cout << "Reminder removed successfully.\n";
}

void viewNextReminder()
{
    Reminder *reminder = reminderQueue.getNextReminder();

    if (reminder == nullptr)
    {
        std::cout << "\nNo reminders available.\n";
        return;
    }

    std::cout << "\n===== Next Reminder =====\n";
    std::cout << "Medicine ID: " << reminder->medicineId << "\n";
    std::cout << "Medicine: " << reminder->medicineName << "\n";
    std::cout << "Reminder Time: " << reminder->reminderTime << "\n";
}

void viewAllReminders()
{
    reminderQueue.displayReminders();
}
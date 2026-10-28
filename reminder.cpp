

#include "reminder.h"
#include "conflict.h"

#include <iostream>
#include <utility>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>

// Define the shared reminder queue.
ReminderQueue reminderQueue;

// Constructor: the vector is initially empty.
ReminderQueue::ReminderQueue()
{
}

bool isValidReminderTime(const std::string &time)
{
    if (time.length() != 5 || time[2] != ':')
        return false;

    for (int i = 0; i < 5; i++)
    {
        if (i == 2)
            continue;

        if (!std::isdigit(static_cast<unsigned char>(time[i])))
            return false;
    }

    int hour = std::stoi(time.substr(0, 2));
    int minute = std::stoi(time.substr(3, 2));

    return hour >= 0 && hour <= 23 &&
           minute >= 0 && minute <= 59;
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

int ReminderQueue::removeRemindersForMedicine(int medicineId)
{
    int removed = 0;
    std::vector<Reminder> remaining;

    for (const Reminder &reminder : heap)
    {
        if (reminder.medicineId == medicineId)
            removed++;
        else
            remaining.push_back(reminder);
    }

    heap = remaining;

    // Restore min-heap order after removing reminders.
    for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; i--)
        heapifyDown(i);

    return removed;
}

void ReminderQueue::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);

    if (!file)
    {
        std::cout << "Error: Unable to save reminders to file.\n";
        return;
    }

    // Save each reminder as: ID|Name|Time
    for (const Reminder &reminder : heap)
    {
        file << reminder.medicineId << '|'
             << reminder.medicineName << '|'
             << reminder.reminderTime << '\n';
    }

    file.close();

    if (!file)
    {
        std::cout << "Error: Failed while writing reminders to file.\n";
        return;
    }

    std::cout << "Reminders saved successfully.\n";
}

void ReminderQueue::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);

    // A missing file is normal on the first run.
    if (!file)
    {
        std::ifstream checkFile(filename);

        if (checkFile.good())
        {
            std::cout << "Error: Unable to read reminders file.\n";
        }

        return;
    }

    std::string line;
    int loaded = 0;
    int skipped = 0;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string idText, medicineName, reminderTime;

        if (!std::getline(ss, idText, '|') ||
            !std::getline(ss, medicineName, '|') ||
            !std::getline(ss, reminderTime) ||
            medicineName.empty())
        {
            std::cout << "Skipping invalid reminder record.\n";
            skipped++;
            continue;
        }

        int medicineId;

        try
        {
            std::size_t pos = 0;
            medicineId = std::stoi(idText, &pos);

            if (pos != idText.size() ||
                medicineId <= 0 ||
                !isValidReminderTime(reminderTime))
            {
                std::cout << "Skipping invalid reminder record.\n";
                skipped++;
                continue;
            }
        }
        catch (const std::exception &)
        {
            std::cout << "Skipping invalid reminder record.\n";
            skipped++;
            continue;
        }

        Reminder reminder;
        reminder.medicineId = medicineId;
        reminder.medicineName = medicineName;
        reminder.reminderTime = reminderTime;

        heap.push_back(reminder);
        loaded++;
    }

    // Check for an actual read failure.
    if (file.bad())
    {
        std::cout << "Error: A problem occurred while reading reminders.\n";
    }

    // Restore min-heap order.
    for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; i--)
    {
        heapifyDown(i);
    }

    std::cout << loaded << " reminder(s) loaded.";

    if (skipped > 0)
    {
        std::cout << " " << skipped
                  << " invalid record(s) skipped.";
    }

    std::cout << '\n';
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

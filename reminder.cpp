
#include "reminder.h"
#include "conflict.h"
#include "inventory.h"

#include <iostream>
#include <utility>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <vector>

// Shared reminder queue used throughout the application.
ReminderQueue reminderQueue;

// Initialize an empty min-heap.
ReminderQueue::ReminderQueue()
{
}

// Validate a time in 24-hour HH:MM format.
bool isValidReminderTime(const std::string &time)
{
    if (time.length() != 5 || time[2] != ':')
        return false;

    // Every character except the colon must be a digit.
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

// Add a number of days to a date in YYYY-MM-DD format.
std::string addDaysToDate(const std::string &date, int days)
{
    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    // mktime normalizes the date, including month/year boundaries.
    std::tm dateValue = {};
    dateValue.tm_year = year - 1900;
    dateValue.tm_mon = month - 1;
    dateValue.tm_mday = day + days;
    dateValue.tm_hour = 12;

    std::mktime(&dateValue);

    std::ostringstream result;
    result << std::setfill('0')
           << std::setw(4) << dateValue.tm_year + 1900 << '-'
           << std::setw(2) << dateValue.tm_mon + 1 << '-'
           << std::setw(2) << dateValue.tm_mday;

    return result.str();
}

// Advance recurring reminders that are overdue and restore heap order.
void ReminderQueue::updateRecurringReminders()
{
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);

    if (localTime == nullptr)
    {
        std::cerr << "Error: Could not determine current date and time.\n";
        return;
    }

    // Build today's date and current time in sortable string formats.
    std::ostringstream dateStream;
    dateStream << std::setfill('0')
               << std::setw(4) << localTime->tm_year + 1900 << '-'
               << std::setw(2) << localTime->tm_mon + 1 << '-'
               << std::setw(2) << localTime->tm_mday;

    std::ostringstream timeStream;
    timeStream << std::setfill('0')
               << std::setw(2) << localTime->tm_hour << ':'
               << std::setw(2) << localTime->tm_min;

    std::string today = dateStream.str();
    std::string currentTime = timeStream.str();

    bool changed = false;

    for (Reminder &reminder : heap)
    {
        // Daily reminders advance by one day; interval reminders use
        // their configured number of days.
        int daysToAdd =
            (reminder.reminderType == "DAILY")
                ? 1
                : reminder.intervalDays;

        if (daysToAdd <= 0)
        {
            continue;
        }

        // Keep advancing until the next occurrence is in the future.
        while (reminder.reminderDate < today ||
               (reminder.reminderDate == today &&
                reminder.reminderTime < currentTime))
        {
            reminder.reminderDate =
                addDaysToDate(reminder.reminderDate, daysToAdd);

            changed = true;
        }
    }

    // Dates may have changed, so rebuild the min-heap bottom-up.
    for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; i--)
    {
        heapifyDown(i);
    }

    // Persist changes only when at least one reminder was advanced.
    if (changed && !saveToFile("reminders.txt"))
    {
        std::cerr << "Warning: Recurring reminders were updated in memory "
                     "but could not be saved.\n";
    }
}

// Restore min-heap order after insertion.
// Time complexity: O(log n).
void ReminderQueue::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        // Stop when the parent already comes before the child.
        if (!comesBefore(heap[index], heap[parent]))
        {
            break;
        }

        std::swap(heap[parent], heap[index]);
        index = parent;
    }
}

// Insert a reminder into the min-heap in O(log n) time.
void ReminderQueue::addReminder(const Reminder &reminder)
{
    heap.push_back(reminder);
    heapifyUp(static_cast<int>(heap.size()) - 1);
}

// Restore min-heap order by moving a node toward the leaves.
// Time complexity: O(log n).
void ReminderQueue::heapifyDown(int index)
{
    int size = static_cast<int>(heap.size());

    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        // Find the earliest reminder among the node and its children.
        if (left < size &&
            comesBefore(heap[left], heap[smallest]))
        {
            smallest = left;
        }

        if (right < size &&
            comesBefore(heap[right], heap[smallest]))
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

// Remove the earliest reminder from the root of the min-heap.
bool ReminderQueue::removeReminder()
{
    if (heap.empty())
    {
        return false;
    }

    // Move the last element to the root, then restore heap order.
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

// Display reminders chronologically without changing the original heap.
void ReminderQueue::displayReminders()
{
    if (heap.empty())
    {
        std::cout << "No reminders available.\n";
        return;
    }

    // Sorting a copy preserves the min-heap structure of the queue.
    std::vector<Reminder> sortedReminders = heap;

    std::sort(sortedReminders.begin(), sortedReminders.end(),
              [](const Reminder &a, const Reminder &b)
              {
                  if (a.reminderDate != b.reminderDate)
                  {
                      return a.reminderDate < b.reminderDate;
                  }

                  return a.reminderTime < b.reminderTime;
              });

    std::cout << "\n===== Reminders (Chronological Order) =====\n";

    for (const Reminder &reminder : sortedReminders)
    {
        std::cout << "Medicine ID: " << reminder.medicineId << '\n';
        std::cout << "Medicine Name: " << reminder.medicineName << '\n';

        if (reminder.reminderType == "DAILY")
        {
            std::cout << "Frequency: Daily\n";
        }
        else
        {
            std::cout << "Frequency: Every "
                      << reminder.intervalDays << " day(s)\n";
        }

        std::cout << "Next Reminder Date: "
                  << reminder.reminderDate << '\n';
        std::cout << "Reminder Time: "
                  << reminder.reminderTime << '\n';
        std::cout << "-----------------------------\n";
    }
}

// Remove every reminder associated with a medicine ID.
int ReminderQueue::removeRemindersForMedicine(int medicineId)
{
    int removed = 0;
    std::vector<Reminder> remaining;

    // Keep reminders belonging to other medicines.
    for (const Reminder &reminder : heap)
    {
        if (reminder.medicineId == medicineId)
        {
            removed++;
        }
        else
        {
            remaining.push_back(reminder);
        }
    }

    heap = remaining;

    // Rebuild heap order after removing matching elements.
    for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; i--)
    {
        heapifyDown(i);
    }

    return removed;
}

// Return the names of medicines that currently have scheduled reminders.
std::vector<std::string> ReminderQueue::getAllScheduledMedicines()
{
    std::vector<std::string> medicines;

    for (const Reminder &reminder : heap)
    {
        medicines.push_back(reminder.medicineName);
    }

    return medicines;
}

// Save reminders using the format:
// ID|Name|Type|IntervalDays|Date|Time
bool ReminderQueue::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open "
                  << filename << " for saving reminders.\n";
        return false;
    }

    for (const Reminder &reminder : heap)
    {
        file << reminder.medicineId << '|'
             << reminder.medicineName << '|'
             << reminder.reminderType << '|'
             << reminder.intervalDays << '|'
             << reminder.reminderDate << '|'
             << reminder.reminderTime << '\n';

        if (!file)
        {
            std::cerr << "Error: Failed while writing reminders.\n";
            file.close();
            return false;
        }
    }

    file.close();

    if (file.fail())
    {
        std::cerr << "Error: Failed while closing reminders file.\n";
        return false;
    }

    return true;
}

// Load saved reminders, validate their fields, and rebuild the min-heap.
void ReminderQueue::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);

    // A missing file is normal on the first run.
    if (!file)
    {
        return;
    }

    std::string line;
    int skipped = 0;
    bool namesCorrected = false;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);

        std::string idText, medicineName, reminderType;
        std::string intervalText, reminderDate, reminderTime;

        // Split each record using the saved delimiter.
        if (!std::getline(ss, idText, '|') ||
            !std::getline(ss, medicineName, '|') ||
            !std::getline(ss, reminderType, '|') ||
            !std::getline(ss, intervalText, '|') ||
            !std::getline(ss, reminderDate, '|') ||
            !std::getline(ss, reminderTime) ||
            medicineName.empty())
        {
            std::cerr << "Warning: Skipping invalid reminder record.\n";
            skipped++;
            continue;
        }

        try
        {
            // Reject partially numeric IDs and invalid values.
            std::size_t pos = 0;
            int medicineId = std::stoi(idText, &pos);

            if (pos != idText.size() || medicineId <= 0)
            {
                throw std::invalid_argument("Invalid medicine ID");
            }

            pos = 0;
            int intervalDays = std::stoi(intervalText, &pos);

            if (pos != intervalText.size() || intervalDays < 0)
            {
                throw std::invalid_argument("Invalid interval");
            }

            // Daily reminders use interval 0; interval reminders need
            // a positive number of days.
            bool validType =
                (reminderType == "DAILY" && intervalDays == 0) ||
                (reminderType == "INTERVAL" && intervalDays >= 1);

            if (!validType ||
                !isValidReminderTime(reminderTime) ||
                !isValidDateFormat(reminderDate))
            {
                std::cerr << "Warning: Skipping invalid reminder record.\n";
                skipped++;
                continue;
            }

            // Keep the saved medicine name synchronized with inventory.
            Medicine *medicine =
                inventoryTable.searchMedicine(medicineId);

            if (medicine != nullptr)
            {
                if (medicineName != medicine->name)
                {
                    std::cout << "Correcting reminder name for Medicine ID "
                              << medicineId << ": "
                              << medicineName << " -> "
                              << medicine->name << '\n';

                    medicineName = medicine->name;
                    namesCorrected = true;
                }
            }
            else
            {
                // Retain orphaned reminders so the user can review or remove them.
                std::cerr << "Warning: Medicine ID " << medicineId
                          << " is not present in inventory. "
                          << "Keeping the saved reminder for review.\n";
            }

            Reminder reminder;
            reminder.medicineId = medicineId;
            reminder.medicineName = medicineName;
            reminder.reminderType = reminderType;
            reminder.intervalDays = intervalDays;
            reminder.reminderDate = reminderDate;
            reminder.reminderTime = reminderTime;

            // Insert the validated record; heap order is rebuilt below.
            heap.push_back(reminder);
        }
        catch (const std::exception &)
        {
            std::cerr << "Warning: Skipping invalid reminder record.\n";
            skipped++;
        }
    }

    if (file.bad())
    {
        std::cerr << "Error: A problem occurred while reading reminders.\n";
    }

    // Build a valid min-heap in O(n) time.
    for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; i--)
    {
        heapifyDown(i);
    }

    if (skipped > 0)
    {
        std::cerr << "Warning: " << skipped
                  << " invalid reminder record(s) skipped.\n";
    }

    // Persist corrected names so the file matches the inventory.
    if (namesCorrected && !saveToFile(filename))
    {
        std::cerr << "Warning: Corrected reminder names could not be saved.\n";
    }
}

// Create a reminder after validating the medicine and checking conflicts.
void addReminder()
{
    Reminder reminder;

    std::cout << "\nEnter Medicine ID: ";

    if (!(std::cin >> reminder.medicineId) ||
        reminder.medicineId <= 0)
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid medicine ID.\n";
        return;
    }

    // A reminder can only be created for a medicine in inventory.
    Medicine *medicine =
        inventoryTable.searchMedicine(reminder.medicineId);

    if (medicine == nullptr)
    {
        std::cout << "Medicine not found in inventory.\n";
        return;
    }

    // Use the inventory name to avoid inconsistent medicine names.
    reminder.medicineName = medicine->name;

    std::cout << "Medicine Name: "
              << reminder.medicineName << '\n';

    int choice;

    std::cout << "\nSelect Reminder Frequency:\n";
    std::cout << "1. Daily\n";
    std::cout << "2. Every N Days\n";
    std::cout << "Enter choice: ";

    if (!(std::cin >> choice) ||
        (choice != 1 && choice != 2))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid choice.\n";
        return;
    }

    if (choice == 1)
    {
        reminder.reminderType = "DAILY";
        reminder.intervalDays = 0;
    }
    else
    {
        reminder.reminderType = "INTERVAL";

        std::cout << "Enter interval in days: ";

        if (!(std::cin >> reminder.intervalDays) ||
            reminder.intervalDays < 1)
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Interval must be at least 1 day.\n";
            return;
        }
    }

    std::cout << "Enter starting date (YYYY-MM-DD): ";
    std::cin >> reminder.reminderDate;

    std::cout << "Enter reminder time (HH:MM, 24-hour format): ";
    std::cin >> reminder.reminderTime;

    if (!isValidDateFormat(reminder.reminderDate) ||
        !isValidReminderTime(reminder.reminderTime))
    {
        std::cout << "Invalid date or time format.\n";
        return;
    }

    // Check for known conflicts with medicines already scheduled.
    std::vector<std::string> currentMeds =
        reminderQueue.getAllScheduledMedicines();

    if (conflictGraph.hasConflict(
            reminder.medicineName, currentMeds))
    {
        std::cout
            << "Reminder aborted due to a potential drug interaction.\n";
        return;
    }

    reminderQueue.addReminder(reminder);

    if (reminderQueue.saveToFile("reminders.txt"))
    {
        std::cout << "Reminder added and saved successfully.\n";
    }
    else
    {
        std::cout << "Warning: Reminder was added to the current session, "
                     "but could not be saved to file.\n";
    }
}

// Remove all reminders associated with a selected medicine.
void removeReminder()
{
    int medicineId;

    std::cout << "\nEnter Medicine ID to remove its reminders: ";

    if (!(std::cin >> medicineId) || medicineId <= 0)
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid medicine ID.\n";
        return;
    }

    int removed = reminderQueue.removeRemindersForMedicine(medicineId);

    if (removed == 0)
    {
        std::cout << "No reminders found for Medicine ID "
                  << medicineId << ".\n";
        return;
    }

    if (!reminderQueue.saveToFile("reminders.txt"))
    {
        std::cerr << "Warning: Reminders were removed in memory "
                     "but could not be saved to file.\n";
    }

    std::cout << removed << " reminder(s) removed for Medicine ID "
              << medicineId << ".\n";
}

// Display the root of the min-heap without removing it.
void viewNextReminder()
{
    Reminder *next = reminderQueue.getNextReminder();

    if (next == nullptr)
    {
        std::cout << "No reminders available.\n";
        return;
    }

    std::cout << "\n===== Next Reminder =====\n";
    std::cout << "Medicine ID: " << next->medicineId << '\n';
    std::cout << "Medicine Name: " << next->medicineName << '\n';
    std::cout << "Reminder Date: " << next->reminderDate << '\n';
    std::cout << "Reminder Time: " << next->reminderTime << '\n';

    if (next->reminderType == "DAILY")
    {
        std::cout << "Frequency: Daily\n";
    }
    else
    {
        std::cout << "Frequency: Every "
                  << next->intervalDays << " day(s)\n";
    }
}

// Define chronological priority: earlier dates first, then earlier times.
bool ReminderQueue::comesBefore(const Reminder &a, const Reminder &b)
{
    if (a.reminderDate != b.reminderDate)
    {
        return a.reminderDate < b.reminderDate;
    }

    return a.reminderTime < b.reminderTime;
}

// User-facing function to display all scheduled reminders.
void viewAllReminders()
{
    reminderQueue.displayReminders();
}

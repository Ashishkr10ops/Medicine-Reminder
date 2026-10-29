
#ifndef REMINDER_H
#define REMINDER_H

#include <string>
#include <vector>

// Stores the medicine details and schedule for one reminder.
struct Reminder
{
    int medicineId;
    std::string medicineName;

    // Recurrence type: "DAILY" or "INTERVAL".
    std::string reminderType;

    // Number of days between reminders; used for INTERVAL reminders.
    int intervalDays;

    // Date and time of the next scheduled occurrence.
    std::string reminderDate; // YYYY-MM-DD
    std::string reminderTime; // HH:MM
};

// Manages reminders using a min-heap.
// The earliest date and time have the highest priority.
class ReminderQueue
{
private:
    // Vector storing the heap in array representation.
    std::vector<Reminder> heap;

    // Returns true if reminder a should have higher priority than reminder b.
    bool comesBefore(const Reminder &a, const Reminder &b);

    // Restores heap order after inserting a new element.
    void heapifyUp(int index);

    // Restores heap order after removing or replacing an element.
    void heapifyDown(int index);

public:
    // Creates an initially empty reminder queue.
    ReminderQueue();

    // Inserts a reminder and maintains min-heap order.
    void addReminder(const Reminder &reminder);

    // Removes the earliest reminder; returns false if the heap is empty.
    bool removeReminder();

    // Returns a pointer to the earliest reminder without removing it.
    // Returns nullptr if no reminders are available.
    Reminder *getNextReminder();

    // Displays all reminders in chronological order.
    void displayReminders();

    // Returns medicine names associated with scheduled reminders.
    std::vector<std::string> getAllScheduledMedicines();

    // Removes all reminders belonging to the specified medicine ID.
    // Returns the number of reminders removed.
    int removeRemindersForMedicine(int medicineId);

    // Saves reminders to a file; returns false if saving fails.
    bool saveToFile(const std::string &filename);

    // Loads and validates reminders from a file.
    void loadFromFile(const std::string &filename);

    // Advances overdue recurring reminders and restores heap order.
    void updateRecurringReminders();
};

// Shared reminder queue defined in reminder.cpp.
extern ReminderQueue reminderQueue;

// User-facing functions for managing and viewing reminders.
void addReminder();
void removeReminder();
void viewNextReminder();
void viewAllReminders();

// Validates a time string in 24-hour HH:MM format.
bool isValidReminderTime(const std::string &time);

#endif

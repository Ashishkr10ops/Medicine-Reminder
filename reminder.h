
#ifndef REMINDER_H
#define REMINDER_H

#include <string>
#include <vector>

// Stores the details required for a medicine reminder.
struct Reminder
{
    int medicineId;
    std::string medicineName;
    std::string reminderTime;
};

// Implements a min-heap to prioritize the earliest reminder.
class ReminderQueue
{
private:
    // Stores reminders in heap order.
    std::vector<Reminder> heap;

    // Restores heap order after inserting a new reminder.
    void heapifyUp(int index);

    // Restores heap order after removing the top reminder.
    void heapifyDown(int index);

public:
    // Creates an initially empty reminder queue.
    ReminderQueue();

    // Inserts a reminder and maintains min-heap order.
    void addReminder(const Reminder &reminder);

    // Removes the earliest reminder; returns false if empty.
    bool removeReminder();

    // Returns the earliest reminder without removing it.
    Reminder *getNextReminder();

    // Displays all reminders in their current heap arrangement.
    void displayReminders();
};

// Shared reminder queue defined in reminder.cpp.
extern ReminderQueue reminderQueue;

// User-facing functions called from the main menu.
void addReminder();
void removeReminder();
void viewNextReminder();
void viewAllReminders();

#endif

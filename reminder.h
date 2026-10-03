#ifndef REMINDER_H
#define REMINDER_H

#include <string>
#include <vector>

// Stores information about one reminder
struct Reminder
{
    int medicineId;
    std::string medicineName;
    std::string reminderTime;
};

// Min Heap used to manage reminders.
// The reminder with the earliest time stays at the root.
class ReminderQueue
{
private:
    std::vector<Reminder> heap;

    // Restores heap property after insertion/removal
    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    ReminderQueue();

    // Min Heap operations
    void addReminder(const Reminder &reminder);
    bool removeReminder();
    Reminder *getNextReminder();
    void displayReminders();
};

// Global reminder queue
extern ReminderQueue reminderQueue;

// User-facing functions
void addReminder();
void removeReminder();
void viewNextReminder();
void viewAllReminders();

#endif
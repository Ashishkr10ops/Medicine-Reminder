#ifndef REMINDER_H
#define REMINDER_H

#include <string>
#include <vector>

struct Reminder
{
    int medicineId;
    std::string medicineName;
    std::string reminderTime;
};

class ReminderQueue
{
private:
    std::vector<Reminder> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    ReminderQueue();

    void addReminder(const Reminder &reminder);
    bool removeReminder();
    Reminder *getNextReminder();
    void displayReminders();
    std::vector<std::string> getAllScheduledMedicines(); // New addition
};

extern ReminderQueue reminderQueue;

void addReminder();
void removeReminder();
void viewNextReminder();
void viewAllReminders();

#endif
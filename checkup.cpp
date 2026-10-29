
#include "checkup.h"
#include "inventory.h"
#include "reminder.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <ctime>

namespace
{
    // File used to persist checkup records between program runs.
    const std::string CHECKUP_FILE = "checkups.txt";

    // Reads checkup records from the file and validates their date and time.
    std::vector<Checkup> loadCheckups()
    {
        std::vector<Checkup> checkups;
        std::ifstream file(CHECKUP_FILE);

        if (!file.is_open())
        {
            std::cerr << "Error: Could not open "
                      << CHECKUP_FILE << ".\n";
            return checkups;
        }

        std::string line;

        // Each record uses the format: date|time|doctor|hospital.
        while (std::getline(file, line))
        {
            if (line.empty())
                continue;

            std::stringstream ss(line);
            Checkup checkup;

            // Extract the required date, time, and doctor fields.
            if (!std::getline(ss, checkup.date, '|') ||
                !std::getline(ss, checkup.time, '|') ||
                !std::getline(ss, checkup.doctor, '|'))
            {
                std::cerr << "Warning: Malformed checkup record: "
                          << line << '\n';
                continue;
            }

            // The hospital field is optional and may be empty.
            std::getline(ss, checkup.hospital);

            // Reuse existing validation functions for date and time.
            bool validDate = isValidDateFormat(checkup.date);
            bool validTime = isValidReminderTime(checkup.time);

            if (validDate && validTime)
            {
                checkups.push_back(checkup);
            }
            else
            {
                std::cerr << "Warning: Invalid date or time in record: "
                          << line << '\n';
            }
        }

        return checkups;
    }

    // Writes all supplied checkup records to the persistence file.
    bool saveCheckups(const std::vector<Checkup> &checkups)
    {
        std::ofstream file(CHECKUP_FILE);

        if (!file.is_open())
        {
            std::cerr << "Error: Unable to save checkups to "
                      << CHECKUP_FILE << ".\n";
            return false;
        }

        // Store each checkup as a pipe-separated record.
        for (const Checkup &checkup : checkups)
        {
            file << checkup.date << '|'
                 << checkup.time << '|'
                 << checkup.doctor << '|'
                 << checkup.hospital << '\n';
        }

        file.close();

        // Check whether writing and closing the file succeeded.
        if (!file)
        {
            std::cerr << "Error: Failed to finish saving checkups.\n";
            return false;
        }

        return true;
    }
}

// Collects and saves a new checkup record.
void addNextCheckupDate()
{
    Checkup checkup;

    std::cout << "\nEnter checkup date (YYYY-MM-DD): ";

    // Reject invalid input and clear the input stream before returning.
    if (!(std::cin >> checkup.date))
    {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid date input.\n";
        return;
    }

    if (!isValidDateFormat(checkup.date))
    {
        std::cout << "Invalid date. Use YYYY-MM-DD.\n";
        return;
    }

    std::cout << "Enter checkup time (HH:MM): ";

    if (!(std::cin >> checkup.time))
    {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid time input.\n";
        return;
    }

    if (!isValidReminderTime(checkup.time))
    {
        std::cout << "Invalid time. Use HH:MM in 24-hour format.\n";
        return;
    }

    // Clear the remaining newline before reading complete text fields.
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter doctor name (optional): ";
    std::getline(std::cin, checkup.doctor);

    std::cout << "Enter hospital/clinic (optional): ";
    std::getline(std::cin, checkup.hospital);

    // The pipe character is reserved as the file's field delimiter.
    if (checkup.doctor.find('|') != std::string::npos ||
        checkup.hospital.find('|') != std::string::npos)
    {
        std::cout << "The character '|' is not allowed in these fields.\n";
        return;
    }

    // Load existing records so the new checkup does not replace them.
    std::vector<Checkup> checkups = loadCheckups();
    checkups.push_back(checkup);

    if (saveCheckups(checkups))
    {
        std::cout << "Checkup date saved successfully.\n";
    }
}

// Displays the earliest checkup scheduled for today or a future date.
void viewNextCheckupDate()
{
    std::vector<Checkup> checkups = loadCheckups();

    if (checkups.empty())
    {
        std::cout << "No valid checkup dates found.\n";
        return;
    }

    // Sort by date first, then by time for checkups on the same date.
    // YYYY-MM-DD and HH:MM strings can be compared lexicographically.
    std::sort(checkups.begin(), checkups.end(),
              [](const Checkup &a, const Checkup &b)
              {
                  if (a.date != b.date)
                      return a.date < b.date;

                  return a.time < b.time;
              });

    // Obtain the current local date from the system clock.
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);

    if (localTime == nullptr)
    {
        std::cerr << "Error: Could not determine the current date.\n";
        return;
    }

    char todayBuffer[11];
    std::strftime(todayBuffer, sizeof(todayBuffer), "%Y-%m-%d",
                  localTime);

    std::string today(todayBuffer);

    // Find the first checkup whose date is today or later.
    for (const Checkup &checkup : checkups)
    {
        if (checkup.date >= today)
        {
            std::cout << "\n===== Next Checkup =====\n";
            std::cout << "Date: " << checkup.date << '\n';
            std::cout << "Time: " << checkup.time << '\n';
            std::cout << "Doctor: "
                      << (checkup.doctor.empty()
                              ? "Not specified"
                              : checkup.doctor)
                      << '\n';
            std::cout << "Hospital: "
                      << (checkup.hospital.empty()
                              ? "Not specified"
                              : checkup.hospital)
                      << '\n';

            return;
        }
    }

    std::cout << "No upcoming checkups found.\n";
}

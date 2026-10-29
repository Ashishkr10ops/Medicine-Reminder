
#ifndef CHECKUP_H
#define CHECKUP_H

#include <string>

// Stores the details of a scheduled medical checkup.
struct Checkup
{
    std::string date;     // Checkup date in YYYY-MM-DD format.
    std::string time;     // Checkup time in HH:MM (24-hour) format.
    std::string doctor;   // Doctor's name; optional.
    std::string hospital; // Hospital or clinic name; optional.
};

// Collects checkup details, validates the input, and saves the record.
void addNextCheckupDate();

// Displays the earliest checkup scheduled for today or a future date.
void viewNextCheckupDate();

#endif

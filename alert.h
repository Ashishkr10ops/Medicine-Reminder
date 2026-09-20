#ifndef ALERT_H
// If it is not defined, the compiler proceeds with the code below it.
#define ALERT_H

// Functions declaration

void checkLowStock();
void checkExpiry();
void generateAlert();

#endif
// If this header hasn't been included, include its contents. Otherwise, skip it.

// The #ifndef, #define, and#endif are called include guards.
// They prevent the same header from being included multiple times.
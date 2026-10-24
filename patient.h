#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <unordered_map>

// Stores information about a patient
struct PatientProfile {
    int id;
    std::string name;
    int age;
    std::string disease;
};

// Manages all patient records using a Hash Map for O(1) lookups
class PatientManager {
private:
    std::unordered_map<int, PatientProfile> patientMap;

public:
    PatientManager();
    ~PatientManager();

    void addPatient();
    void viewPatient();
    void displayAllPatients();
    
    // Allows the disease module to find a patient by ID
    PatientProfile* getPatient(int id);
};

extern PatientManager patientManager;

#endif
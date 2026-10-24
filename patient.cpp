#include "patient.h"
#include <iostream>

PatientManager patientManager;

PatientManager::PatientManager() {}
PatientManager::~PatientManager() {}

void PatientManager::addPatient() {
    PatientProfile p;
    std::cout << "\nEnter Patient ID: ";
    std::cin >> p.id;
    std::cout << "Enter Patient Name: ";
    std::cin >> p.name;
    std::cout << "Enter Patient Age: ";
    std::cin >> p.age;
    std::cout << "Enter Primary Disease: ";
    std::cin >> p.disease;

    patientMap[p.id] = p;
    std::cout << "Patient profile created successfully.\n";
}

void PatientManager::viewPatient() {
    int id;
    std::cout << "\nEnter Patient ID to search: ";
    std::cin >> id;

    if (patientMap.find(id) != patientMap.end()) {
        PatientProfile p = patientMap[id];
        std::cout << "\n===== Patient Profile =====\n";
        std::cout << "ID: " << p.id << "\n";
        std::cout << "Name: " << p.name << "\n";
        std::cout << "Age: " << p.age << "\n";
        std::cout << "Disease: " << p.disease << "\n";
    } else {
        std::cout << "Patient not found.\n";
    }
}

void PatientManager::displayAllPatients() {
    if (patientMap.empty()) {
        std::cout << "\nNo patients registered.\n";
        return;
    }

    std::cout << "\n===== All Patients =====\n";
    for (const auto& pair : patientMap) {
        std::cout << "ID: " << pair.second.id 
                  << " | Name: " << pair.second.name
                  << " | Age: " << pair.second.age 
                  << " | Disease: " << pair.second.disease << "\n";
    }
}

PatientProfile* PatientManager::getPatient(int id) {
    if (patientMap.find(id) != patientMap.end()) {
        return &patientMap[id];
    }
    return nullptr;
}
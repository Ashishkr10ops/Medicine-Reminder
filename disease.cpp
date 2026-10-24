#include "disease.h"
#include "patient.h"
#include <iostream>

DiseaseManager diseaseManager;

DiseaseManager::DiseaseManager() {}

void DiseaseManager::addTreatment() {
    std::string disease, medicine;
    std::cout << "\nEnter Disease Name: ";
    std::cin >> disease;
    std::cout << "Enter Medicine used to treat " << disease << ": ";
    std::cin >> medicine;

    diseaseTreatments[disease].push_back(medicine);
    std::cout << "Treatment linked successfully.\n";
}

void DiseaseManager::viewTreatments() {
    std::string disease;
    std::cout << "\nEnter Disease Name to check treatments: ";
    std::cin >> disease;

    if (diseaseTreatments.find(disease) != diseaseTreatments.end()) {
        std::cout << "\nRecommended medicines for " << disease << ":\n";
        for (const std::string& med : diseaseTreatments[disease]) {
            std::cout << "- " << med << "\n";
        }
    } else {
        std::cout << "No treatments found for this disease.\n";
    }
}

void DiseaseManager::suggestForPatient() {
    int patientId;
    std::cout << "\nEnter Patient ID to suggest medicines: ";
    std::cin >> patientId;

    PatientProfile* p = patientManager.getPatient(patientId);
    if (p == nullptr) {
        std::cout << "Patient not found.\n";
        return;
    }

    std::cout << "\nPatient " << p->name << " is diagnosed with " << p->disease << ".\n";
    
    if (diseaseTreatments.find(p->disease) != diseaseTreatments.end()) {
        std::cout << "Suggested Medicines based on diagnosis:\n";
        for (const std::string& med : diseaseTreatments[p->disease]) {
            std::cout << "- " << med << "\n";
        }
    } else {
        std::cout << "No known treatments recorded for " << p->disease << " in the system.\n";
    }
}
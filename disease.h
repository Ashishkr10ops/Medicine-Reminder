#ifndef DISEASE_H
#define DISEASE_H

#include <string>
#include <vector>
#include <unordered_map>

class DiseaseManager {
private:
    // Maps a Disease (string) to a list of Medicines (vector of strings)
    std::unordered_map<std::string, std::vector<std::string>> diseaseTreatments;

public:
    DiseaseManager();
    void addTreatment();
    void viewTreatments();
    void suggestForPatient();
};

extern DiseaseManager diseaseManager;

#endif
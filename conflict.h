#ifndef CONFLICT_H
#define CONFLICT_H

#include <string>
#include <unordered_map>
#include <vector>

class ConflictGraph {
private:
    std::unordered_map<std::string, std::vector<std::string>> adjList;

public:
    ConflictGraph();
    void addConflict(const std::string& med1, const std::string& med2);
    bool hasConflict(const std::string& newMed, const std::vector<std::string>& currentMeds);
};

extern ConflictGraph conflictGraph;
void initializeConflicts();

#endif
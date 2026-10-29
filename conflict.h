
#ifndef CONFLICT_H
#define CONFLICT_H

#include <string>
#include <unordered_map>
#include <vector>

// Represents medicine-conflict relationships using an adjacency list.
class ConflictGraph
{
private:
    // Maps each medicine name to the list of medicines that conflict with it.
    // The unordered_map provides average O(1) lookup by medicine name.
    std::unordered_map<std::string, std::vector<std::string>> adjList;

public:
    // Initializes an empty conflict graph.
    ConflictGraph();

    // Adds a conflict relationship between two medicines.
    // Each relationship is stored in both directions.
    void addConflict(const std::string &med1, const std::string &med2);

    // Checks whether newMed conflicts with any medicine in currentMeds.
    // Returns true if a known conflict is found; otherwise, returns false.
    bool hasConflict(
        const std::string &newMed,
        const std::vector<std::string> &currentMeds);
};

// Shared graph instance used throughout the application.
extern ConflictGraph conflictGraph;

// Adds the example medicine-conflict relationships to the graph.
void initializeConflicts();

#endif


#include "conflict.h"
#include "design.h"

#include <iostream>
#include <algorithm>
#include <cctype>

// Shared graph containing known medicine-conflict relationships.
ConflictGraph conflictGraph;

// Initializes an empty conflict graph.
ConflictGraph::ConflictGraph() {}

// Converts a medicine name to lowercase for case-insensitive comparison.
std::string toLowerCase(const std::string &name)
{
    std::string result = name;

    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char ch)
                   {
                       return static_cast<char>(std::tolower(ch));
                   });

    return result;
}

// Adds an undirected edge between two medicines.
// Both directions are stored in the adjacency list.
void ConflictGraph::addConflict(
    const std::string &med1,
    const std::string &med2)
{
    // Normalize names so comparisons are independent of letter case.
    std::string first = toLowerCase(med1);
    std::string second = toLowerCase(med2);

    adjList[first].push_back(second);
    adjList[second].push_back(first);
}

// Checks whether the new medicine conflicts with any current medicine.
// Returns true and displays a warning when a known conflict is found.
bool ConflictGraph::hasConflict(
    const std::string &newMed,
    const std::vector<std::string> &currentMeds)
{
    std::string normalizedNewMed = toLowerCase(newMed);

    // Find all medicines associated with the new medicine in the graph.
    auto it = adjList.find(normalizedNewMed);

    if (it == adjList.end())
    {
        // No known conflicts are recorded for this medicine.
        return false;
    }

    // Compare each current medicine against the stored conflict list.
    for (const std::string &current : currentMeds)
    {
        std::string normalizedCurrent = toLowerCase(current);

        for (const std::string &conflict : it->second)
        {
            if (normalizedCurrent == conflict)
            {
                std::cout << RED << BOLD
                          << "\n[CRITICAL WARNING]: "
                          << newMed << " may interact with "
                          << current << "!\n"
                          << RESET;

                return true;
            }
        }
    }

    // No matching conflict was found among the current medicines.
    return false;
}

// Initializes the example conflict pairs used by the application.
void initializeConflicts()
{
    conflictGraph.addConflict("Ibuprofen", "Aspirin");
    conflictGraph.addConflict("Warfarin", "Aspirin");
    conflictGraph.addConflict("Penicillin", "Methotrexate");
}

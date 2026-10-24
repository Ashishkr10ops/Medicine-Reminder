#include "conflict.h"
#include "design.h"
#include <iostream>

ConflictGraph conflictGraph;

ConflictGraph::ConflictGraph() {}

void ConflictGraph::addConflict(const std::string& med1, const std::string& med2) {
    adjList[med1].push_back(med2);
    adjList[med2].push_back(med1); 
}

bool ConflictGraph::hasConflict(const std::string& newMed, const std::vector<std::string>& currentMeds) {
    if (adjList.find(newMed) == adjList.end()) return false;

    for (const std::string& current : currentMeds) {
        for (const std::string& conflict : adjList[newMed]) {
            if (current == conflict) {
                std::cout << RED << BOLD << "\n[CRITICAL WARNING]: " << newMed << " severely conflicts with " << current << "!\n" << RESET;
                return true;
            }
        }
    }
    return false;
}

void initializeConflicts() {
    conflictGraph.addConflict("Ibuprofen", "Aspirin");
    conflictGraph.addConflict("Warfarin", "Aspirin");
    conflictGraph.addConflict("Penicillin", "Methotrexate");
}
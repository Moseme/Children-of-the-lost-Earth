// SHARED (all three students): main menu option 10, System Analysis.
// Shows the complexity of the system's main operations and MEASURES them on generated data of
// growing size, so the Big-O claims in the report are backed by real numbers.
//
// The experiments build their own temporary data (the live resources, requests and locations
// are never touched). Comparisons are counted exactly, so those numbers are the same on every
// computer; times in milliseconds depend on the machine.
#ifndef ANALYSISMANAGER_H
#define ANALYSISMANAGER_H
#include "ResourceManager.h"
#include "RequestManager.h"
#include "CampusManager.h"

class AnalysisManager {
private:
    ResourceManager& resources;
    RequestManager& requests;
    CampusManager& campus;

    AnalysisManager(const AnalysisManager&);               // copying forbidden
    AnalysisManager& operator=(const AnalysisManager&);

    void showComplexitySummary() const;
    void searchExperiment() const;
    void hashExperiment() const;
    void sortExperiment() const;
    void speedExperiment() const;
    void showSnapshot() const;

public:
    AnalysisManager(ResourceManager& rm, RequestManager& reqs, CampusManager& campusManager);
    void menu();
};

#endif

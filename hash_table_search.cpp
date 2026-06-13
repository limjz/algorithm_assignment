// *********************************************************
// Program: hash_table_search.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244PS | LIM JUN ZHAO | LIM.JUN.ZHAO@student.mmu.edu.my | 0126010726
// Member_2: 243UC247DT | THASSVEEN   |                                  | 
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

// Aligned with group Record struct
struct Record {
    long long number;
    string word;
};

// Global Hash Table parameters
const int TABLE_SIZE = 10007;

// Using a 2D Vector (Vector of Vectors) to represent Hash Table with Simple Chaining
// This acts exactly like the "3D" array logic style for bucket tracking without pointer mess
vector<vector<Record>> hashTable(TABLE_SIZE);

// Basic Hash Function
int hashFunction(long long key) {
    return key % TABLE_SIZE;
}

// Read CSV and insert into Hash Table
bool loadDataset(string filename) {
    ifstream file(filename);
    if (!file) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        int commaPos = line.find(',');
        Record r;
        r.number = stoll(line.substr(0, commaPos));
        r.word = line.substr(commaPos + 1);

        // Simple chaining: append directly to the vector bucket index
        int index = hashFunction(r.number);
        hashTable[index].push_back(r);
    }
    file.close();
    return true;
}

// Search function that tracks metrics and logs steps
bool searchHashTable(long long targetKey, Record& result, int& steps, double& elapsedTime) {
    int index = hashFunction(targetKey);
    steps = 0;

    auto start = chrono::high_resolution_clock::now();

    // Traverse the simple linked list/vector chain sequentially
    for (int i = 0; i < hashTable[index].size(); i++) {
        steps++;
        if (hashTable[index][i].number == targetKey) {
            result = hashTable[index][i];
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duration = end - start;
            elapsedTime = duration.count(); // Time in milliseconds
            return true;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    elapsedTime = duration.count();
    return false;
}

int main() {
    string datasetFilename;
    cout << "Enter dataset filename (example: dataset_1000): ";
    cin >> datasetFilename;

    // Cross-OS Path handling matching radix_sort.cpp
    string inputFile = "CSV_dataset\\" + datasetFilename + ".csv";
    ifstream checkFile(inputFile);
    if (!checkFile) {
        inputFile = "CSV_dataset/" + datasetFilename + ".csv"; // Mac Fallback
    }
    else {
        checkFile.close();
    }

    cout << "Loading " << inputFile << " ..." << endl;
    if (!loadDataset(inputFile)) {
        cout << "Error: Cannot open or read file " << inputFile << endl;
        return 1;
    }
    cout << "Dataset loaded successfully into Hash Table." << endl << endl;

    // Perform Search Operations to capture Best, Avg, Worst cases
    long long targetKey;
    cout << "Enter key to search: ";
    cin >> targetKey;

    Record result;
    int steps = 0;
    double elapsedTime = 0.0;

    bool found = searchHashTable(targetKey, result, steps, elapsedTime);

    if (found) {
        cout << "Key Found: " << result.word << endl;
        cout << "Steps taken: " << steps << endl;
        cout << "Execution Time: " << elapsedTime << " ms" << endl;
    } else {
        cout << "Key not found in dataset." << endl;
        cout << "Steps taken: " << steps << endl;
    }

    // Creating the Output metrics log file matching project standards
    string outputFolder = "CSV_output\\";
    string outputFile = outputFolder + "hash_search_analytics_" + datasetFilename + ".txt";
    ofstream logFile(outputFile);
    
    if (!logFile) {
        // Fallback for Mac paths
        outputFile = "CSV_output/hash_search_analytics_" + datasetFilename + ".txt";
        logFile.open(outputFile);
    }

    if (logFile) {
        logFile << "=========================================\n";
        logFile << "HASH TABLE SEARCH PERFORMANCE METRICS\n";
        logFile << "=========================================\n";
        logFile << "Dataset Evaluated: " << datasetFilename << "\n";
        logFile << "Target Key Searched: " << targetKey << "\n";
        logFile << "Search Result: " << (found ? "FOUND" : "NOT FOUND") << "\n";
        logFile << "Total Chain Comparisons (Steps): " << steps << "\n";
        logFile << "Measured Execution Time: " << elapsedTime << " ms\n";
        logFile << "=========================================\n";
        logFile.close();
        cout << "\nAnalytics metrics saved to: " << outputFile << endl;
    }

    return 0;
}
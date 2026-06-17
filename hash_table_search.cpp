// *********************************************************
// Program: hash_table_search.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Class: TC4L | T13L
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;

struct Record {
    long long number;
    string word;
};

const int TABLE_SIZE = 10007;
vector<vector<Record>> hashTable(TABLE_SIZE);
vector<long long> allKeys; // To store keys for running N searches

int hashFunction(long long key) {
    return key % TABLE_SIZE;
}

bool loadDataset(string filename) {
    ifstream file(filename);
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        int commaPos = line.find(',');
        if (commaPos == string::npos) continue;

        Record r;
        r.number = stoll(line.substr(0, commaPos));
        r.word = line.substr(commaPos + 1);

        int index = hashFunction(r.number);
        hashTable[index].push_back(r);
        allKeys.push_back(r.number); // Save key for batch testing
    }
    file.close();
    return true;
}

// Search function returning execution time in seconds
double searchTimeOnly(long long targetKey) {
    int index = hashFunction(targetKey);
    
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < hashTable[index].size(); i++) {
        if (hashTable[index][i].number == targetKey) {
            auto end = chrono::high_resolution_clock::now();
            return chrono::duration<double>(end - start).count();
        }
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double>(end - start).count();
}

int main() {
    // Hardcoded dataset target matching Jun Zhao's structure reference
    string datasetBase = "dataset_1000"; 
    string inputFile = "CSV_dataset/" + datasetBase + ".csv";

    if (!loadDataset(inputFile)) {
        inputFile = "CSV_dataset\\" + datasetBase + ".csv";
        if (!loadDataset(inputFile)) {
            cout << "Error: Cannot open " << datasetBase << ".csv" << endl;
            return 1;
        }
    }

    double totalExecutionTime = 0.0;
    double bestCase = 999999.0;
    double worstCase = -1.0;
    int n = allKeys.size();

    // Perform N searches over the entire dataset keyspace
    for (int i = 0; i < n; i++) {
        double t = searchTimeOnly(allKeys[i]);
        totalExecutionTime += t;
        if (t < bestCase) bestCase = t;
        if (t > worstCase) worstCase = t;
    }

    double averageCase = totalExecutionTime / n;

    // Required Filename Output: hash_table_search_dataset_1000.txt
    string outputFilename = "hash_table_search_" + datasetBase + ".txt";
    ofstream outFile(outputFilename);
    if (outFile.is_open()) {
        outFile << "Best case time: " << bestCase << " seconds\n";
        outFile << "Average case time: " << averageCase << " seconds\n";
        outFile << "Worst case time: " << worstCase << " seconds\n";
        outFile.close();
        cout << "Metrics successfully written to " << outputFilename << endl;
    }

    return 0;
}
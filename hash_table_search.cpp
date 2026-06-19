// *********************************************************
// Program: hash_table_search.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244PS | LIM JUN ZHAO | LIM.JUN.ZHAO@student.mmu.edu.my | 0126010726
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************
// Task Distribution
// Member_1: Radix Sort and Hash Table Search (radix_sort.cpp, radix_sort_step.cpp, hash_table_search.cpp, hash_table_search_step.cpp)
// Member_2: Implement dataset generator.
// Member_3:
// Member_4:
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip> // for fixed and setprecision()

using namespace std;

struct Record {
    long long number;
    string word;
};

// number of bucket in hash table
// const int TABLE_SIZE = 10007;
const int TABLE_SIZE = 5000011;

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

// performing one search function, returning execution time in seconds
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
    string datasetBase = "dataset_50000000"; 
    string inputFile = "CSV_dataset/" + datasetBase + ".csv";

    if (!loadDataset(inputFile)) {
        inputFile = "CSV_dataset\\" + datasetBase + ".csv";
        if (!loadDataset(inputFile)) {
            cout << "Error: Cannot open " << datasetBase << ".csv" << endl;
            return 1;
        }
    }

    double totalExecutionTime = 0.0;
    double bestCase = 999999.0; //super high value for comparison
    double worstCase = -1.0; // -ve for comparison
    int n = allKeys.size();

    // Perform N searches over the entire dataset keyspace to get best, average, worst case times
    for (int i = 0; i < n; i++) {
        double t = searchTimeOnly(allKeys[i]);
        totalExecutionTime += t;
        if (t < bestCase) bestCase = t;
        if (t > worstCase) worstCase = t;
    }

    double averageCase = totalExecutionTime / n;

    // Required Filename Output: hash_table_search_dataset_1000.txt
    string outputFilename = "CSV_output\\hash_table_search_" + datasetBase + ".txt";
    ofstream outFile(outputFilename);
    if (outFile.is_open()) { 
        outFile << fixed << setprecision(9); // Set precision for seconds
        outFile << "Best case time: " << bestCase << " seconds\n";
        outFile << "Average case time: " << averageCase << " seconds\n";
        outFile << "Worst case time: " << worstCase << " seconds\n";
        outFile.close();
        cout << "Metrics successfully written to " << outputFilename << endl;
    }

    cout << "Best case time:    " << bestCase    << " seconds" << endl;
    cout << "Average case time: " << averageCase << " seconds" << endl;
    cout << "Worst case time:   " << worstCase   << " seconds" << endl;
    cout << endl;

    return 0;
}
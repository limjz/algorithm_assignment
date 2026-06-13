// *********************************************************
// Program: hash_table_search_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Record {
    long long number;
    string word;
};

const int TABLE_SIZE = 10007;
vector<vector<Record>> hashTable(TABLE_SIZE);

int hashFunction(long long key) {
    return key % TABLE_SIZE;
}

int main() {
    string datasetFilename;
    cout << "Enter dataset filename for Step-by-Step Logging: ";
    cin >> datasetFilename;

    string inputFile = "CSV_dataset/" + datasetFilename + ".csv";
    ifstream file(inputFile);
    if (!file) {
        inputFile = "CSV_dataset\\" + datasetFilename + ".csv";
        file.open(inputFile);
    }

    if (!file) {
        cout << "Error opening file." << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        int commaPos = line.find(',');
        Record r;
        r.number = stoll(line.substr(0, commaPos));
        r.word = line.substr(commaPos + 1);
        hashTable[hashFunction(r.number)].push_back(r);
    }
    file.close();

    long long targetKey;
    cout << "Enter target key to trace: ";
    cin >> targetKey;

    int index = hashFunction(targetKey);
    
    string outputFile = "CSV_output/hash_search_step_" + datasetFilename + ".txt";
    ofstream log(outputFile);
    if (!log) {
        outputFile = "CSV_output\\hash_search_step_" + datasetFilename + ".txt";
        log.open(outputFile);
    }

    log << "Tracing Search Path for Key: " << targetKey << "\n";
    log << "Hashed Target Bucket Index: " << index << "\n";
    log << "--------------------------------------------------\n";

    bool found = false;
    for (int i = 0; i < hashTable[index].size(); i++) {
        log << "Step " << (i + 1) << " -> Checking Index Slot [" << i << "] | Key: " << hashTable[index][i].number;
        if (hashTable[index][i].number == targetKey) {
            log << " [MATCH FOUND!]\n";
            log << "Result Payload: " << hashTable[index][i].word << "\n";
            found = true;
            break;
        }
        log << " [NO MATCH]\n";
    }

    if (!found) {
        log << "--------------------------------------------------\n";
        log << "Search Complete. Target Key does not exist in chain.\n";
    }

    log.close();
    cout << "Step-by-step verification trace successfully logged in CSV_output!" << endl;
    return 0;
}
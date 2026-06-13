// *********************************************************
// Program: hash_table_search_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Class: TC4L | T13L
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
    string datasetBase = "dataset_1000";
    long long targetKey = 2008864030; // Hardcoded exact target required by rubrics/Jun Zhao

    string inputFile = "CSV_dataset/" + datasetBase + ".csv";
    ifstream file(inputFile);
    if (!file) {
        inputFile = "CSV_dataset\\" + datasetBase + ".csv";
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

    // Required filename: dataset_1000_hash_table_search_step_2008864030.txt
    string outputFilename = datasetBase + "_hash_table_search_step_" + to_string(targetKey) + ".txt";
    ofstream outFile(outputFilename);

    if (!outFile.is_open()) {
        cout << "Error creating output file." << endl;
        return 1;
    }

    int index = hashFunction(targetKey);
    bool found = false;

    for (size_t i = 0; i < hashTable[index].size(); i++) {
        if (hashTable[index][i].number == targetKey) {
            outFile << hashTable[index][i].number << " = " << hashTable[index][i].number << "/" << hashTable[index][i].word << "\n";
            found = true;
            break;
        } else {
            outFile << "-1 != " << targetKey << "\n";
        }
    }

    if (!found && hashTable[index].empty()) {
        outFile << "-1 != " << targetKey << "\n";
    }

    outFile.close();
    cout << "Step logs successfully written to " << outputFilename << endl;

    return 0;
}
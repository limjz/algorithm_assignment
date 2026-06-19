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

//number of bucket in hash table - using prime number reduces collisions
// const int TABLE_SIZE = 10007;
const int TABLE_SIZE = 5000011;

vector<vector<Record>> hashTable(TABLE_SIZE);

int hashFunction(long long key) {
    return key % TABLE_SIZE;
}

// insert all the generate data in to the hash table vector
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
        r.word   = line.substr(commaPos + 1);

        int index = hashFunction(r.number);
        hashTable[index].push_back(r);
    }

    file.close();
    return true;
}


void searchAndLog(long long targetKey, ofstream& outFile){

    int index = hashFunction(targetKey);
    bool found = false;
    int stepCount = 0;
    string foundWord; // to store the word if found

    outFile << "Target Search Parameter: " << targetKey << "\n";
    outFile << "Hashed Bucket Index: " << index << "\n";

    for (size_t i = 0; i < hashTable[index].size(); i++) {

        stepCount++;
        long long currentKey = hashTable[index][i].number;

        if (currentKey == targetKey) {
            
            outFile << "Step " << stepCount << ": Comparing with "
                    << currentKey << " -> MATCH FOUND\n";
            found = true;
            foundWord = hashTable[index][i].word; // store the word
            break;
        } else {
            outFile << "Step " << stepCount << ": Comparing with "
                    << currentKey << " -> NO MATCH\n";
           
        }
    }

    if (found) {
        outFile << targetKey << " = " << targetKey << "/" << foundWord << "\n";
    }
    else {
        outFile << "-1 != " << targetKey << "\n";
    }

};




int main() {

    // CHANGE THESE PARAMETERS 
    string datasetBase = "dataset_1000000";
    long long foundKey = 1000787734; 
    long long notFoundKey = 123456789; 


    string inputFile = "CSV_dataset/" + datasetBase + ".csv";

    if (!loadDataset(inputFile)) {
        inputFile = "CSV_dataset\\" + datasetBase + ".csv";
        if (!loadDataset(inputFile)) {
            cout << "Error opening file." << endl;
            return 1;
        }
    }


    // Target FOUND 
    // Output file name: dataset_1000_hash_table_search_step_2008864030.txt
    string foundOutputFilename = "CSV_output\\" + datasetBase + "_hash_table_search_step_" + to_string(foundKey) + ".txt";
    ofstream foundFile(foundOutputFilename);

    if (!foundFile.is_open()) {
        cout << "Error creating output file " << foundOutputFilename << endl;
        return 1;
    }

    searchAndLog(foundKey, foundFile);
    foundFile.close();

    cout << "Found target search results written to " << foundOutputFilename << endl;


    // Target NOT FOUND
    // Output file name: dataset_1000_hash_table_search_step_123456789.txt

    string notFoundOutputFilename = "CSV_output\\" + datasetBase + "_hash_table_search_step_" + to_string(notFoundKey) + ".txt";
    ofstream notFoundFile(notFoundOutputFilename);
    if (!notFoundFile){
        cout << "Error creating output file " << notFoundOutputFilename << endl;
        return 1;
    }

    searchAndLog(notFoundKey, notFoundFile);
    notFoundFile.close();

    cout << "Not-found target search results written to " << notFoundOutputFilename << endl;

    return 0;
}
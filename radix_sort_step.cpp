// *********************************************************
// Program: radix_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC3L
// Tutorial Class: T12L
// Trimester: 2610
// Member_1: 242UC244PS | LIM JUN ZHAO | LIM.JUN.ZHAO@student.mmu.edu.my | 0126010726
// Member_2: 242UC19643 | TOO JUN CHEN | TOO.JUN.CHEN@student.mmu.edu.my | 0123456789
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************
// Task Distribution
// Member_1: Radix Sort (radix_sort.cpp, radix_sort_step.cpp)
// Member_2: Implement dataset generator.
// Member_3:
// Member_4:
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


int getDigit(long long number, int position) {
    long long divisor = 1;
    // build divisor: position 10 = 1, position 9 = 10, position 8 = 100...
    for (int i = 0; i < (10 - position); i++) {
        divisor = divisor * 10;
    }
    return (number / divisor) % 10;
}

void printStep(vector<Record> arr, ofstream& outFile, string label) {
    string line = "[";

    for (int i = 0; i < arr.size(); i++) {
        line += to_string(arr[i].number) + "/" + arr[i].word;

        // Add comma and space after every element except the last one
        if (i < arr.size() - 1) {
            line += ", ";
        }
    }

    line += "] " + label;

    // print to screen AND output file simultaneously
    cout << line << endl;
    outFile << line << "\n";
}


void sortByDigit(vector<Record>& arr, int position) {
    int n = arr.size();

    // count_bucket is an array of 10 vectors
    vector<Record> count_bucket[10];

    // Place each record element in to correct bucket
    // based on its digit at the current position
    for (int i = 0; i < n; i++) {
        int digit = getDigit(arr[i].number, position);
        count_bucket[digit].push_back(arr[i]);
    }

    // Read all buckets back into arr[] in order from bucket 0 to 9 => sorted first digits
    int index = 0;
    for (int i = 0; i < 10; i++) {
        // go through every record inside this bucket
        for (int j = 0; j < count_bucket[i].size(); j++) {
            arr[index] = count_bucket[i][j];
            index++;
        }
    }
    
}

// read from CSV file
vector<Record> readCSV(string filename, int startRow, int endRow) {
    vector<Record> data;
    ifstream file(filename);

    if (!file) {
        cout << "Error: Cannot open file " << filename << endl;
        return data;
    }

    string line;
    int currentRow = 1;  // track which row we are currently reading

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (currentRow >= startRow && currentRow <= endRow) {
            int commaPos = line.find(',');
            Record r;
            r.number = stoll(line.substr(0, commaPos));  // text before comma → number
            r.word   = line.substr(commaPos + 1);         // text after comma  → word
            data.push_back(r);
        }

        currentRow++;

        // stop at the set endRow 
        if (currentRow > endRow) break;
    }

    file.close();
    return data;
}


int main() {

    /* ========= CHANGE INPUT VALUES HERE ============= */
    string datasetName = "dataset_7000000.csv";
    int startRow = 1;
    int endRow = stoi(datasetName.substr(datasetName.find("dataset_") + 8, datasetName.find(".csv") - datasetName.find("dataset_") - 8));
    //int endRow = 7;


    string inputFile = "CSV_dataset\\" + datasetName;

    // Check if file exists
    ifstream checkFile(inputFile);
    if (!checkFile) {
        cout << endl;
        cout << "  Error: \"" << datasetName << "\" not found in CSV_dataset folder!" << endl;
        cout << "  Please check the filename and try again." << endl << endl;
        return 1;
    }
    checkFile.close();

 

    // error checking 
    if (startRow < 1 ) {
        cout << endl;
        cout << "  Error: Start row must be > 1 " << endl << endl;
        return 1;  
    }
    if (endRow < startRow) {
        cout << endl;
        cout << "  Error: End row must be >= Start row " << startRow << " !!" << endl << endl;
        return 1;  
    }


    cout << endl;
    cout << "Loading rows " << startRow << " to " << endRow
     << " from " << inputFile << " ..." << endl;


    // Load only the rows we need from the CSV
    vector<Record> data = readCSV(inputFile, startRow, endRow);

    if (data.empty()) {
        cout << "Error: No data loaded." << endl;
        return 1;
    }

    int start_baseName = inputFile.find("CSV_dataset\\") + 12; // position after "CSV_dataset\"
    int end_baseName = inputFile.find(".csv"); // position of ".csv"

    string baseName   = inputFile.substr(start_baseName, end_baseName - start_baseName); // extract the filename without .csv
    string outputFile = "CSV_output\\" + baseName + "_radix_sorted_step_"
                      + to_string(startRow) + "_"
                      + to_string(endRow) + ".txt";

    // Open output txt file for writing
    ofstream outFile(outputFile);
    if (!outFile) {
        cout << "Error: Cannot create output file. " << outputFile << endl;
        return 1;
    }

    cout << "=== Radix Sort Step by Step ===" << endl;
    cout << "Rows: " << startRow << " to " << endRow << endl << endl;

    // print out original unsorted array first
    printStep(data, outFile, "original");

    // sort and print all steps
    for (int d = 10; d >= 1; d--) {
        sortByDigit(data, d);
        printStep(data, outFile, "d=" + to_string(d));
    }

    outFile.close();
    cout << "\nStep file saved as: " << outputFile << endl;

    return 0;
}

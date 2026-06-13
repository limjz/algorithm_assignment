// *********************************************************
// Program: heap_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244PS | LIM JUN ZHAO | LIM.JUN.ZHAO@student.mmu.edu.my | 0126010726
// Member_2: 242UC244SJ | KHOO SHEN ZHI | khoo.shen.zhi@student.mmu.edu.my | 01136735033
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************
// Task Distribution
// Member_1: Radix Sort & File Utilities
// Member_2: Heap Sort Algorithm Implementation 
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

// Custom manual swap helper function (banned from using standard libraries)
void swapRecords(Record& a, Record& b) {
    Record temp = a;
    a = b;
    b = temp;
}

// Reusable function to print array snapshot to screen AND output file simultaneously
void printStep(vector<Record> arr, ofstream& outFile, string label) {
    string line = "[";

    for (size_t i = 0; i < arr.size(); i++) {
        line += to_string(arr[i].number) + "/" + arr[i].word;

        // Add comma and space after every element except the last one
        if (i < arr.size() - 1) {
            line += ", ";
        }
    }

    line += "] " + label;

    // Print to screen AND output file simultaneously
    cout << line << endl;
    outFile << line << "\n";
}

// Maintains the Max-Heap structural property for a subtree rooted at index 'i'
void maxHeapify(vector<Record>& arr, int n, int i) {
    int largest = i;       
    int left = 2 * i + 1;  
    int right = 2 * i + 2; 

    if (left < n && arr[left].number > arr[largest].number) {
        largest = left;
    }

    if (right < n && arr[right].number > arr[largest].number) {
        largest = right;
    }

    if (largest != i) {
        swapRecords(arr[i], arr[largest]);
        maxHeapify(arr, n, largest);
    }
}

// Core Step-by-Step Heap Sort algorithm tracking structural modifications
void heapSortStep(vector<Record>& arr, ofstream& outFile) {
    int n = arr.size();

    // Initial Max-Heap Construction
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }
    // Print layout state right after initial binary tree layout is stabilized
    printStep(arr, outFile, "Build-Max-Heap");

    // Extract elements from the heap sequentially
    int passCounter = 1;
    for (int i = n - 1; i > 0; i--) {
        // Swap largest value (root index 0) down to the sorted boundary
        swapRecords(arr[0], arr[i]);

        // Restore Max-Heap structure on the remaining active tree nodes
        maxHeapify(arr, i, 0);

        // Print structural configuration change for this extraction cycle
        printStep(arr, outFile, "Pass " + to_string(passCounter));
        passCounter++;
    }
}

// Read from CSV file within specified line range
vector<Record> readCSV(string filename, int startRow, int endRow) {
    vector<Record> data;
    ifstream file(filename);

    if (!file) {
        cout << "Error: Cannot open file " << filename << endl;
        return data;
    }

    string line;
    int currentRow = 1;  

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (currentRow >= startRow && currentRow <= endRow) {
            int commaPos = line.find(',');
            Record r;
            r.number = stoll(line.substr(0, commaPos));  
            r.word   = line.substr(commaPos + 1);         
            data.push_back(r);
        }

        currentRow++;
        if (currentRow > endRow) break;
    }

    file.close();
    return data;
}

int main() {

    /* ========= CHANGE INPUT VALUES HERE ============= */
    string datasetName = "dataset_100000.csv";
    int startRow = 1;
    
    // Automatically parse endRow from the filename to match teammate's pattern
    // NOTE: Keep an eye on this value! If datasetName is dataset_7000000.csv, 
    // it will try to load 7,000,000 lines into your step file, printing millions of lines.
    // For small test runs, you can manually override endRow to 7 or 10 as shown below.
    int endRow = stoi(datasetName.substr(datasetName.find("dataset_") + 8, datasetName.find(".csv") - datasetName.find("dataset_") - 8));
    // int endRow = 7; // <- Uncomment this line when doing quick step validation tests!

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

    // Data validation checks
    if (startRow < 1) {
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

    // Load sub-range data
    vector<Record> data = readCSV(inputFile, startRow, endRow);

    if (data.empty()) {
        cout << "Error: No data loaded." << endl;
        return 1;
    }

    int start_baseName = inputFile.find("CSV_dataset\\") + 12; 
    int end_baseName = inputFile.find(".csv"); 

    string baseName   = inputFile.substr(start_baseName, end_baseName - start_baseName); 
    
    string outputFile = "CSV_output\\" + baseName + "_heap_sorted_step_"
                      + to_string(startRow) + "_"
                      + to_string(endRow) + ".txt";

    // Open output txt file for writing
    ofstream outFile(outputFile);
    if (!outFile) {
        cout << "Error: Cannot create output file. " << outputFile << endl;
        return 1;
    }

    cout << "=== Heap Sort Step by Step ===" << endl;
    cout << "Rows: " << startRow << " to " << endRow << endl << endl;

    // 1. Trace base data layout
    printStep(data, outFile, "original");

    // 2. Perform heap sort and record its internal tree conversions
    heapSortStep(data, outFile);

    outFile.close();
    cout << "\nStep file saved as: " << outputFile << endl;

    return 0;
}
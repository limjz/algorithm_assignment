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
void printStep(const vector<Record>& arr, ofstream& outFile, string label) {
    string line = "[";

    for (size_t i = 0; i < arr.size(); i++) {
        line += to_string(arr[i].number) + "/" + arr[i].word;

        // Add comma and space after every element except the last one
        if (i < arr.size() - 1) {
            line += ", ";
        }
    }

    line += "] " + label;

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
    // Print the structure right after initial building completes
    printStep(arr, outFile, "Build-Max-Heap");

    // Extract elements from the heap loop
    int stepCounter = 1;
    for (int i = n - 1; i > 0; i--) {
        // Move current root (maximum element) to the sorting end boundary
        swapRecords(arr[0], arr[i]);

        // Fix the heap property on the remaining active elements
        maxHeapify(arr, i, 0);

        // Print structural footprint of the entire vector after this extraction pass
        printStep(arr, outFile, "Pass " + to_string(stepCounter));
        stepCounter++;
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

// Data protection: Only allow valid integers
int getIntInput(string message) {
    int number;
    while (true) {
        cout << message;
        cin >> number;

        if (cin.fail()) {
            cin.clear();            
            cin.ignore(1000, '\n'); 
            cout << endl;
            cout << "  Error: Please enter a number only!" << endl << endl;
            continue;  
        }
        return number;
    }
}

// Dynamic data scanner
int countRows(string filename) { 
    ifstream file(filename);
    string line; 
    int count = 0;
    while (getline(file, line)) {
        if (line.empty()) continue; 
        count++;
    }
    file.close();
    return count;
}

int main(int argc, char* argv[]) {
    string datasetName;
    string inputFile;
    int startRow;
    int endRow;

    while (true) {
        cout << "Enter dataset filename" << endl;
        cout << "(example: dataset_1000): ";
        cin >> datasetName;

        inputFile = "CSV_dataset\\" + datasetName + ".csv"; 

        ifstream checkFile(inputFile);
        if (!checkFile) {
            cout << endl;
            cout << "  Error: \"" << datasetName << "\" not found in CSV_dataset folder!" << endl;
            cout << "  Please check the filename and try again." << endl << endl;
            continue;  
        }
        checkFile.close();
        break;
    }

    int totalRows = countRows(inputFile);
   
    while (true) {
        startRow = getIntInput("Enter start row number (example: 1): ");
        endRow   = getIntInput("Enter end row number   (example: 10): "); // Recommended small range for step test

        if (startRow < 1 || startRow > totalRows) {
            cout << endl;
            cout << "  Error: Start row must be between 1 and " << totalRows << endl << endl;
            continue;  
        }
        if (endRow < startRow || endRow > totalRows) {
            cout << endl;
            cout << "  Error: End row must be between " << startRow << " and " << totalRows << endl << endl;
            continue;  
        }
        break;
    }

    cout << endl;
    cout << "Loading rows " << startRow << " to " << endRow;
    cout << " from " << inputFile << " ..." << endl;

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

    ofstream outFile(outputFile);
    if (!outFile) {
        cout << "Error: Cannot create output file." << endl;
        return 1;
    }

    cout << "=== Heap Sort Step by Step ===" << endl;
    cout << "Rows: " << startRow << " to " << endRow << endl << endl;

    //Print out the baseline sequence layout
    printStep(data, outFile, "original");

    //Perform heap sort tracing mutations inside
    heapSortStep(data, outFile);

    outFile.close();
    cout << "\nStep file saved as: " << outputFile << endl;

    return 0;
}
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
#include <chrono>   
using namespace std;

struct Record {
    long long number;
    string word;
};

// Custom manual swap help function
void swapRecords(Record& a, Record& b) {
    Record temp = a;
    a = b;
    b = temp;
}

// Maintains the Max-Heap structural property for a subtree rooted at index 'i'
void maxHeapify(vector<Record>& arr, int n, int i) {
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // Left child index
    int right = 2 * i + 2; // Right child index

    // If left child is larger than root
    if (left < n && arr[left].number > arr[largest].number) {
        largest = left;
    }

    // If right child is larger than current largest
    if (right < n && arr[right].number > arr[largest].number) {
        largest = right;
    }

    // If largest is not root, swap and recursively heapify down
    if (largest != i) {
        swapRecords(arr[i], arr[largest]);
        maxHeapify(arr, n, largest);
    }
}

// Core Heap Sort Algorithm
void heapSort(vector<Record>& arr) {
    int n = arr.size();

    //Rearrange array into a Max-Heap structure
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }

    //One by one extract elements from the heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to the end boundary
        swapRecords(arr[0], arr[i]);

        // Call max heapify on the reduced heap area
        maxHeapify(arr, i, 0);
    }
}

// read all and place into vector<Record>data
vector<Record> readCSV(string filename) {
    vector<Record> data;
    ifstream file(filename);

    if (!file) {
        cout << "Error: Cannot open file " << filename << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Find the comma that separates number and word
        int commaPos = line.find(',');

        Record r;
        r.number = stoll(line.substr(0, commaPos));   // text before comma -> number
        r.word   = line.substr(commaPos + 1);          // text after comma  -> word
        data.push_back(r);
    }

    file.close();
    return data;
}
// Write to CSV file 
bool writeCSV(vector<Record> arr, string filename) {
    ofstream file(filename);

    if (!file) {
        cout << "Error: Cannot create output file." << endl;
        return false;
    }

    for (int i = 0; i < arr.size(); i++) {
        file << arr[i].number << "," << arr[i].word << "\n";
    }

    file.close();
    return true;
}

void writeTime (string filename, double elapsedTime, int inputSize) {
    ofstream timeFile (filename, ios::app); // open in append mode
    
    if (!timeFile) {
        cout << "Error: Cannot write running time to file." << endl;
        return;
    }
    timeFile << "\n";
    timeFile << "# Running time: " << elapsedTime << " seconds\n";
    timeFile << "# Input size: "   << inputSize << " records\n";
    timeFile.close();
}

int main() {
    /* ========= CHANGE INPUT VALUES HERE ============= */
    string datasetName = "dataset_10000.csv";
    
    string inputFile = "CSV_dataset\\" + datasetName;  // construct full path to input file

    // error checking, if file not found, show error and exit
    ifstream checkFile(inputFile);
    if (!checkFile) { // File not found   
        cout << endl;
        cout << "  Error: \"" << datasetName << "\" not found in CSV_dataset folder!" << endl;
        cout << "  Please check the filename and try again." << endl << endl;
        return 1;  // exit the program
    }
    checkFile.close();

    // File exists — load it into the Vector<Record> 
    cout << "Loading " << inputFile << " ..." << endl;
    vector<Record> data = readCSV(inputFile);

    if (data.empty()) {
        cout << "  Error: File is empty or cannot be read." << endl;
        cout << "  Please try a different file." << endl;
        cout << endl;
        return 1;
    }

    cout << "Loaded " << data.size() << " records from " << inputFile << endl;

    // Timer Start (Isolates pure algorithm performance)
    auto start = chrono::high_resolution_clock::now();

    heapSort(data);

    // Timer End after sorting is done
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;
    cout << "Running time: " << elapsed.count() << " seconds" << endl;

    // Save sorted output to new file in folder CSV_output (Matches teammate's naming style)
    string baseName    = datasetName.substr(0, datasetName.find(".csv"));
    string outputFile  = "CSV_output\\" + baseName + ".heap_sorted_" + datasetName;

    bool writeSuccess = writeCSV(data, outputFile);

    if (!writeSuccess) {
        cout << "Error: Failed to write sorted output to file." << endl;
        return 1;
    }

    writeTime(outputFile, elapsed.count(), data.size()); // output the running time at bottom of csv file

    cout << "Sorted output saved as: " << outputFile << endl;

    return 0;
}
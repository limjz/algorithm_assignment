// *********************************************************
// Program: radix_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
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
#include <chrono>   
using namespace std;

struct Record {
    long long number;
    string word;
};


int getDigit(long long number, int position) {

    // Shift the number to right by divide 10^(10 - position)
    // use mod 10 to capture the last digit

    long long divisor = 1;
    for (int i = 0; i < (10 - position); i++) {
        divisor = divisor * 10;
    }

    return (number / divisor) % 10;
}

// sort the array by ONE digit position using counting sort, and later loop back for 10 digits
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

// loop from ones to billions to sort it 
void radixSort(vector<Record>& arr) {

    // d = digit position, from 10 (ones) down to 1 (billions)
    for (int d = 10; d >= 1; d--) {
        sortByDigit(arr, d);
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

// write to CSV file
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
    string datasetName = "dataset_100000000.csv";
    

    string inputFile = "CSV_dataset\\" +  datasetName;  // construct full path to input file

    // error checking, if file not found, show error and ask again
    ifstream checkFile(inputFile);
    
    if (!checkFile) { // File not found   
        cout << endl;
        cout << "  Error: \"" << datasetName << "\" not found in CSV_dataset folder!" << endl;
        cout << "  Please check the filename and try again." << endl << endl;
        return 1;  // exit the program
    }
    checkFile.close();

    // File exists — load it in to the Vector<Record> 
    cout << "Loading " << inputFile << " ..." << endl;
    vector<Record> data = readCSV(inputFile);

    if (data.empty()) {
        cout << "  Error: File is empty or cannot be read." << endl;
        cout << "  Please try a different file." << endl;
        cout << endl;
        return 1;
    }


    cout << "Loaded " << data.size() << " records from " << inputFile << endl;

    // Timer Start
    auto start = chrono::high_resolution_clock::now();

    radixSort(data);

    // Timer End after sorting is done
    auto end = chrono::high_resolution_clock::now();


    chrono::duration<double> elapsed = end - start;
    cout << "Running time: " << elapsed.count() << " seconds" << endl;

    // Save sorted output to new file in folder CSV_output
    string baseName    = datasetName.substr(0, datasetName.find(".csv"));
    string outputFile  = "CSV_output\\" + baseName + ".radix_sorted_" + datasetName;

    bool writeSuccess = writeCSV(data, outputFile);

    if (!writeSuccess) {
        cout << "Error: Failed to write sorted output to file." << endl;
        return 1;
    }

    writeTime(outputFile, elapsed.count(), data.size()); //output the running time at bottom of csv file

    cout << "Sorted output saved as: " << outputFile << endl;

    return 0;
}

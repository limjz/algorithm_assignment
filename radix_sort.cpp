// *********************************************************
// Program: radix_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244PS | LIM JUN ZHAO | LIM.JUN.ZHAO@student.mmu.edu.my | 0126010726
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
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
    vector<Record> sorted_array(n);    // temp array output[] 

    // count_bucket[] = 10 buckets, each for one digit numbers  
    int count_bucket[10] = {0}; // default all buckets = 0

    // count how many numbers fall into each bucket
    // eg:  [0, 0, 0, 2, 0, 1, 0, 2, 2, 1]
    // means: 2 numbers end with 3, 1 ends with 5, etc.
    for (int i = 0; i < n; i++) {
        int digit = getDigit(arr[i].number, position); // position 10 = ones, position 1 = billions
       count_bucket[digit]++;
    }

    // Accumulate the digits in the buckets, so that count_bucket[i] now contains the ENDING index for each bucket
    for (int i = 1; i < 10; i++) {
       count_bucket[i] = count_bucket[i] + count_bucket[i - 1];
    }

    // Place numbers into correct position in sorted_array[]
    // go BACKWARDS from i = n-1 down to 0 to keep sort STABLE
    for (int i = n - 1; i >= 0; i--) {
        int digit = getDigit(arr[i].number, position);
        int pos = count_bucket[digit] - 1;  // find correct index
        sorted_array[pos] = arr[i];        // place it there
       count_bucket[digit]--;              // move the pointer left
    }

    // sub back the sorted output to arr[]
    for (int i = 0; i < n; i++) {
        arr[i] = sorted_array[i];
    }
}

// ----------------------------------------------------------
// Full Radix Sort
// Process digit by digit, starting from rightmost (position 10)
// going all the way to leftmost (position 1)
// ----------------------------------------------------------
void radixSort(vector<Record>& arr) {
    // d = digit position, from 10 (ones) down to 1 (billions)
    for (int d = 10; d >= 1; d--) {
        sortByDigit(arr, d);
    }
}

// ----------------------------------------------------------
// Read CSV file and store all records into a vector
// ----------------------------------------------------------
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

// ----------------------------------------------------------
// Write all sorted records into a new CSV file
// ----------------------------------------------------------
void writeCSV(vector<Record> arr, string filename) {
    ofstream file(filename);
    for (int i = 0; i < arr.size(); i++) {
        file << arr[i].number << "," << arr[i].word << "\n";
    }
    file.close();
}

// ----------------------------------------------------------
// MAIN
// ----------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage  : ./radix_sort <dataset_filename.csv>" << endl;
        cout << "Example: ./radix_sort dataset_1000.csv" << endl;
        return 1;
    }

    string inputFile = argv[1];

    // Read all records from CSV into memory
    vector<Record> data = readCSV(inputFile);

    if (data.empty()) {
        cout << "Error: No data loaded from " << inputFile << endl;
        return 1;
    }

    cout << "Loaded " << data.size() << " records from " << inputFile << endl;

    // -------------------------------------------------------
    // START TIMER — only time the sort, NOT file reading/writing
    // -------------------------------------------------------
    auto start = chrono::high_resolution_clock::now();

    radixSort(data);

    auto end = chrono::high_resolution_clock::now();
    // -------------------------------------------------------
    // END TIMER
    // -------------------------------------------------------

    chrono::duration<double> elapsed = end - start;
    cout << "Running time: " << elapsed.count() << " seconds" << endl;

    // Save sorted output to new file
    string outputFile = "radix_sorted_" + inputFile;
    writeCSV(data, outputFile);
    cout << "Sorted output saved as: " << outputFile << endl;

    return 0;
}

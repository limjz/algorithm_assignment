// *********************************************************
// Program: dataset_generator.cpp
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
#include <cstdlib>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

long long bigRand() {
    long long value = (long long)rand() * (RAND_MAX + 1LL) + rand();
    return value;
}

string randomWord() {
    string word = "aaaaa";

    for (int i = 0; i < 5; i++) {
        word[i] = 'a' + (rand() % 26);
    }

    return word;
}

int main() {
    // Seed: group leader ID 242UC244PS -> 2421324469
    srand(2421324469U);

    long long n;

    n = 50000000;   // Change this line for each run

    if (n <= 0 || n > 9000000000LL) {
        cout << "Error: Dataset size must be from 1 to 9000000000." << endl;
        return 1;
    }

    string filename = "CSV_dataset\\dataset_" + to_string(n) + ".csv";
    ofstream outFile(filename);

    if (!outFile) {
        cout << "Error: Cannot create file " << filename << endl;
        cout << "Make sure the CSV_dataset folder exists." << endl;
        return 1;
    }

    // Vector replaces manual pointer allocation.
    vector<long long> numbers(n);

    auto generateStart = chrono::system_clock::now();

    // Generate unique 10-digit integers.
    for (long long i = 0; i < n; i++) {
        numbers[i] = 1000000000LL + i;
    }

    // Manual Fisher-Yates shuffle.
    for (long long i = n - 1; i > 0; i--) {
        long long randomIndex = bigRand() % (i + 1);

        long long temp = numbers[i];
        numbers[i] = numbers[randomIndex];
        numbers[randomIndex] = temp;
    }

    auto generateEnd = chrono::system_clock::now();
    chrono::duration<double> generateDuration = generateEnd - generateStart;
    auto saveStart = chrono::system_clock::now();

    // Buffer output to reduce file-writing overhead.
    string outputBuffer = "";
    outputBuffer.reserve(1024 * 1024);

    for (long long i = 0; i < n; i++) {
        outputBuffer += to_string(numbers[i]);
        outputBuffer += ",";
        outputBuffer += randomWord();
        outputBuffer += "\n";

        if (outputBuffer.size() >= 1024 * 1024) {
            outFile << outputBuffer;
            outputBuffer.clear();
        }

        if ((i + 1) % 10000000 == 0) {
            cout << "Progress: " << (i + 1) << " / " << n << endl;
        }
    }

    if (outputBuffer != "") {
        outFile << outputBuffer;
    }

    outFile.close();

    auto saveEnd = chrono::system_clock::now();
    chrono::duration<double> saveDuration = saveEnd - saveStart;

    cout << "Done!" << endl;
    cout << "Records generated: " << n << endl;
    cout << "Output saved as:   " << filename << endl;
    cout << "Generate and shuffle time: " << generateDuration.count()
<< " seconds" << endl;
    cout << "CSV saving time:           " << saveDuration.count()
         << " seconds" << endl;
    cout << "Total time:                "
         << generateDuration.count() + saveDuration.count()
         << " seconds" << endl;

    return 0;
}

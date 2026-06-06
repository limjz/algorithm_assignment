// *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************

#include <iostream>
#include <fstream>
#include <cstdlib>   
#include <ctime>     
#include <string>
#include <set>       // for set<long long> to track used numbers
using namespace std;


string randomWord() {
    string word = "";
    for (int i = 0; i < 5; i++) {

        char letter = 'a' + (rand() % 26);
        word += letter;
    }
    return word;
}


long long randomNumber() {
    // rand() only gives up to 32767 on some systems
    // build a big number using two rand() calls
    long long num = (long long)rand() * rand() % 9000000000LL + 1000000000LL;
    return num;
}

int main() {
    // Seed: student ID 242UC244PS -> 2421324469
    // We use unsigned int version for srand
    srand(2421324469U);

    int n;
    cout << "Enter number of records to generate: ";
    cin >> n;

    string outputFolder = "CSV_dataset\\";
   
    string filename = outputFolder + "dataset_" + to_string(n) + ".csv";
    ofstream outFile(filename);

    if (!outFile) {
        cout << "Error: Cannot create file " << filename << endl;
        return 1;
    }

    // Use a set to track which numbers have been used
    // set automatically checks for duplicates
    set<long long> usedNumbers;

    int count = 0;
    cout << "Generating " << n << " unique records..." << endl;

    while (count < n) {
        long long num = randomNumber();

        // Check if this number was already generated, if yes, skip and try again
        if (usedNumbers.count(num) > 0) {
            continue;  // duplicate found, skip
        }

        // Number is unique, add to set and write to file
        usedNumbers.insert(num);
        string word = randomWord();

        outFile << num << "," << word << "\n";
        count++;

        // Show progress every 100,000 records
        if (count % 100000 == 0) {
            cout << "  Progress: " << count << " / " << n << endl;
        }
    }

    outFile.close();
    cout << "Done! Saved as: " << filename << endl;

    return 0;
}

// *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: YOUR_LECTURE_CLASS
// Tutorial Class: YOUR_TUTORIAL_CLASS
// Trimester: 2610
// Member_1: 242UC244PS | YOUR_NAME | YOUR_EMAIL | YOUR_PHONE
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************
// Task Distribution
// Member_1: Implement dataset generator.
// Member_2:
// Member_3:
// Member_4:
// *********************************************************

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

using namespace std;

namespace {

// Integer range required by assignment:
// 1,000,000,000 to 9,999,999,999.
constexpr uint64_t MIN_INTEGER = 1'000'000'000ULL;
constexpr uint64_t MAX_INTEGER = 9'999'999'999ULL;
constexpr uint64_t INTEGER_RANGE =
    MAX_INTEGER - MIN_INTEGER + 1ULL;

// Full-cycle pseudo-random sequence constants.
// No integer repeats before all 9 billion values are used.
constexpr uint64_t LCG_MULTIPLIER = 1'664'521ULL;
constexpr uint64_t LCG_INCREMENT = 1'013'904'223ULL;

// Generate one random five-letter lowercase string.
string generateFiveLetterString(mt19937_64& rng) {
    uniform_int_distribution<int> letterDistribution(0, 25);

    string text(5, 'a');

    for (char& character : text) {
        character =
            static_cast<char>('a' + letterDistribution(rng));
    }

    return text;
}

// Read and validate dataset size from command line.
uint64_t readDatasetSize(const char* argument) {
    size_t processedCharacters = 0;

    const string input(argument);

    const uint64_t datasetSize =
        stoull(input, &processedCharacters);

    if (processedCharacters != input.size()) {
        throw invalid_argument(
            "Dataset size must contain digits only."
        );
    }

    if (datasetSize == 0 || datasetSize > INTEGER_RANGE) {
        throw out_of_range(
            "Dataset size must be between 1 and 9000000000."
        );
    }

    return datasetSize;
}

} // namespace

int main(int argc, char* argv[]) {
    // Student ID: 242UC244PS
    // Converted seed: 2421324469
    // This line initializes a 64-bit pseudo-random number generator using the group leader student ID as the seed.
    mt19937_64 rng(2421324469ULL);

    if (argc != 2) {
        cerr << "Usage: "
                  << argv[0]
                  << " <dataset_size>\n";

        cerr << "Example: "
                  << argv[0]
                  << " 1000\n";

        return 1;
    }

    try {
        const uint64_t datasetSize =
            readDatasetSize(argv[1]);

        // Create folder if it does not exist.
        filesystem::create_directories("CSV_dataset");

        const string outputPath =
            "CSV_dataset/dataset_" +
            to_string(datasetSize) +
            ".csv";

        ofstream outputFile(outputPath);

        if (!outputFile) {
            cerr
                << "Error: Unable to create "
                << outputPath
                << '\n';

            return 1;
        }

        const auto startTime =
            chrono::steady_clock::now();

        // First value is derived from seeded generator.
        uint64_t currentState =
            rng() % INTEGER_RANGE;

        for (uint64_t row = 0;
             row < datasetSize;
             ++row) {

            const uint64_t uniqueInteger =
                MIN_INTEGER + currentState;

            outputFile
                << uniqueInteger
                << ','
                << generateFiveLetterString(rng)
                << '\n';

            // Generate next unique pseudo-random integer.
            currentState =
                (
                    LCG_MULTIPLIER * currentState +
                    LCG_INCREMENT
                ) % INTEGER_RANGE;
        }

        outputFile.flush();

        if (!outputFile) {
            throw runtime_error(
                "Writing failed. Check available disk space."
            );
        }

        outputFile.close();

        const auto endTime =
            chrono::steady_clock::now();

        const chrono::duration<double> elapsedTime =
            endTime - startTime;

        cout << "Dataset generated successfully.\n";
        cout << "Rows generated: "
                  << datasetSize
                  << '\n';

        cout << "Output file: "
                  << outputPath
                  << '\n';

        cout << "Generation time: "
                  << elapsedTime.count()
                  << " seconds\n";
    }
    catch (const exception& error) {
        cerr
            << "Error: "
            << error.what()
            << '\n';

        return 1;
    }

    return 0;
}
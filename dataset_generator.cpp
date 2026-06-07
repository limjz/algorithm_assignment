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

namespace {

// Integer range required by assignment:
// 1,000,000,000 to 9,999,999,999.
constexpr std::uint64_t MIN_INTEGER = 1'000'000'000ULL;
constexpr std::uint64_t MAX_INTEGER = 9'999'999'999ULL;
constexpr std::uint64_t INTEGER_RANGE =
    MAX_INTEGER - MIN_INTEGER + 1ULL;

// Full-cycle pseudo-random sequence constants.
// No integer repeats before all 9 billion values are used.
constexpr std::uint64_t LCG_MULTIPLIER = 1'664'521ULL;
constexpr std::uint64_t LCG_INCREMENT = 1'013'904'223ULL;

// Generate one random five-letter lowercase string.
std::string generateFiveLetterString(std::mt19937_64& rng) {
    std::uniform_int_distribution<int> letterDistribution(0, 25);

    std::string text(5, 'a');

    for (char& character : text) {
        character =
            static_cast<char>('a' + letterDistribution(rng));
    }

    return text;
}

// Read and validate dataset size from command line.
std::uint64_t readDatasetSize(const char* argument) {
    std::size_t processedCharacters = 0;

    const std::string input(argument);

    const std::uint64_t datasetSize =
        std::stoull(input, &processedCharacters);

    if (processedCharacters != input.size()) {
        throw std::invalid_argument(
            "Dataset size must contain digits only."
        );
    }

    if (datasetSize == 0 || datasetSize > INTEGER_RANGE) {
        throw std::out_of_range(
            "Dataset size must be between 1 and 9000000000."
        );
    }

    return datasetSize;
}

} // namespace

int main(int argc, char* argv[]) {
    // Student ID: 242UC244PS
    // Converted seed: 2421324469
    std::mt19937_64 rng(2421324469ULL);

    if (argc != 2) {
        std::cerr << "Usage: "
                  << argv[0]
                  << " <dataset_size>\n";

        std::cerr << "Example: "
                  << argv[0]
                  << " 1000\n";

        return 1;
    }

    try {
        const std::uint64_t datasetSize =
            readDatasetSize(argv[1]);

        // Create folder if it does not exist.
        std::filesystem::create_directories("CSV_dataset");

        const std::string outputPath =
            "CSV_dataset/dataset_" +
            std::to_string(datasetSize) +
            ".csv";

        std::ofstream outputFile(outputPath);

        if (!outputFile) {
            std::cerr
                << "Error: Unable to create "
                << outputPath
                << '\n';

            return 1;
        }

        const auto startTime =
            std::chrono::steady_clock::now();

        // First value is derived from seeded generator.
        std::uint64_t currentState =
            rng() % INTEGER_RANGE;

        for (std::uint64_t row = 0;
             row < datasetSize;
             ++row) {

            const std::uint64_t uniqueInteger =
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
            throw std::runtime_error(
                "Writing failed. Check available disk space."
            );
        }

        outputFile.close();

        const auto endTime =
            std::chrono::steady_clock::now();

        const std::chrono::duration<double> elapsedTime =
            endTime - startTime;

        std::cout << "Dataset generated successfully.\n";
        std::cout << "Rows generated: "
                  << datasetSize
                  << '\n';

        std::cout << "Output file: "
                  << outputPath
                  << '\n';

        std::cout << "Generation time: "
                  << elapsedTime.count()
                  << " seconds\n";
    }
    catch (const std::exception& error) {
        std::cerr
            << "Error: "
            << error.what()
            << '\n';

        return 1;
    }

    return 0;
}
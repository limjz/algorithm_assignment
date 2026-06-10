// *********************************************************
// Program: hash_table_search.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244PS | LIM JUN ZHAO | LIM.JUN.ZHAO@student.mmu.edu.my | 0126010726
// Member_2: 243UC247DT | THASSVEEN   |                                  | 
// *********************************************************
// Task Distribution
// Member_1: Implement dataset generator & sorting algorithms.
// Member_2: Implement Hash Table Search (AVL Tree Chain) & Analysis.
// *********************************************************

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;

// Aligned perfectly with Jun Zhao's Record struct mapping
struct Record {
    long long number;
    string word;
};

// Node structure for an AVL Balanced Tree chain bucket (as required by assignment spec)
struct AVLNode {
    Record data;
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;
    int height = 1;
};

class HashTableAVL {
private:
    vector<AVLNode*> table;
    long long tableSize;

    // Hash function mapping keys cleanly to addressable buckets
    long long hashFunction(long long key) {
        return key % tableSize;
    }

    int getHeight(AVLNode* node) {
        return node == nullptr ? 0 : node->height;
    }

    int getBalanceFactor(AVLNode* node) {
        return node == nullptr ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    // Standard AVL Tree Left Rotation mechanics
    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    // Standard AVL Tree Right Rotation mechanics
    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    // AVL Tree Balanced Insertion algorithm
    AVLNode* insertNode(AVLNode* node, Record record) {
        if (node == nullptr) {
            return new AVLNode{record, nullptr, nullptr, 1};
        }

        if (record.number < node->data.number) {
            node->left = insertNode(node->left, record);
        } else if (record.number > node->data.number) {
            node->right = insertNode(node->right, record);
        } else {
            return node; // Duplicates handling edge-case
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalanceFactor(node);

        // Left Left Case
        if (balance > 1 && record.number < node->left->data.number) {
            return rotateRight(node);
        }
        // Right Right Case
        if (balance < -1 && record.number > node->right->data.number) {
            return rotateLeft(node);
        }
        // Left Right Case
        if (balance > 1 && record.number > node->left->data.number) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // Right Left Case
        if (balance < -1 && record.number < node->right->data.number) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void destroyTree(AVLNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    HashTableAVL(long long size) {
        this->tableSize = size;
        table.resize(tableSize, nullptr);
    }

    void insert(Record record) {
        long long index = hashFunction(record.number);
        table[index] = insertNode(table[index], record);
    }

    bool searchTreeLog(AVLNode* root, long long targetKey, ofstream& logFile, int& steps) {
        if (root == nullptr) {
            logFile << " -> Node empty. Target NOT FOUND.\n";
            return false;
        }
        steps++;
        logFile << " -> Step " << steps << ": Comparing with Node Key [" << root->data.number << "]\n";

        if (root->data.number == targetKey) {
            logFile << "Target FOUND: " << root->data.word << "\n";
            return true;
        }
        if (targetKey < root->data.number) {
            return searchTreeLog(root->left, targetKey, logFile, steps);
        } else {
            return searchTreeLog(root->right, targetKey, logFile, steps);
        }
    }

    void executeSearchStep(long long targetKey, const string& outputFilePath) {
        ofstream logFile(outputFilePath);
        long long index = hashFunction(targetKey);
        
        logFile << "Target Search Parameter: " << targetKey << "\n";
        logFile << "Hashed Address Bucket Index: " << index << "\n";
        logFile << "Traversing Balanced Search Path:\n";

        int totalSteps = 0;
        bool found = searchTreeLog(table[index], targetKey, logFile, totalSteps);
        logFile << "Total comparison checks executed: " << totalSteps << "\n";
        logFile.close();
    }

    ~HashTableAVL() {
        for (long long i = 0; i < tableSize; ++i) {
            destroyTree(table[i]);
        }
    }
};

int main() {
    string datasetFilename;
    cout << "=== Hash Table Module: AVL Tree Chain Architecture Engaged ===\n";
    cout << "Enter dataset filename to test loading (e.g., dataset_1000): ";
    cin >> datasetFilename;

    // Dual-compatibility slash formatting for cross-OS evaluation support
    string inputFile = "CSV_dataset/" + datasetFilename + ".csv"; 
    ifstream file(inputFile);
    
    // Fallback path handler check for Windows systems run matching
    if (!file) {
        inputFile = "CSV_dataset\\" + datasetFilename + ".csv";
        file.open(inputFile);
    }

    if (!file) {
        cout << "Error: Cannot open file " << inputFile << endl;
        return 1;
    }

    // Dynamic sizing table construction based on standard assignment recommendations
    HashTableAVL hashMap(10007); 
    string line;
    long long recordCount = 0;

    // Using matching parsing framework derived from Radix Sort configuration
    while (getline(file, line)) {
        if (line.empty()) continue;
        int commaPos = line.find(',');
        
        Record r;
        r.number = stoll(line.substr(0, commaPos));
        r.word = line.substr(commaPos + 1);
        
        hashMap.insert(r);
        recordCount++;
    }
    file.close();

    cout << "Successfully populated Hash Table with " << recordCount << " records from " << inputFile << "!\n";
    return 0;
}
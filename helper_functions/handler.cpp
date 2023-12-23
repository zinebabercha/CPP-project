#include <iostream>
#include <fstream>
#include <string>
#include <functional>

using namespace std;

bool safeFileOperation(const string& filePath, function<void(ifstream&)> fileOperation) {
    ifstream file(filePath, ios::binary);
    if (!file) {
        cerr << "Error: Unable to open file " << filePath << endl;
        return false;
    }

    try {
        fileOperation(file);
    } catch (const ios_base::failure& e) {
        cerr << "File operation error: " << e.what() << endl;
        return false;
    }

    return true;
}

void exampleFileOperation(ifstream& file) {
    string line;
    while (getline(file, line)) {
        cout << line << endl; // Print each line of the file
    }
}

int main() {
    string filePath = "fileHash.txt"; // Replace with your file path

    bool success = safeFileOperation(filePath, exampleFileOperation);
    if (success) {
        cout << "File operation successful." << endl;
    } else {
        cout << "File operation failed." << endl;
    }

    return 0;
}

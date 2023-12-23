#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void copyFile(const string& sourcePath, const string& destPath) {
    ifstream src(sourcePath, ios::binary);
    ofstream dest(destPath, ios::binary);

    if (!src) {
        cerr << "Cannot open source file: " << sourcePath << endl;
        return;
    }
    if (!dest) {
        cerr << "Cannot open destination file: " << destPath << endl;
        return;
    }

    dest << src.rdbuf();
}

int main() {
    // Example usage
    string source = "source.txt"; // Replace with actual source file path
    string destination = "destination.txt"; // Replace with actual destination file path

    copyFile(source, destination);
    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>

using namespace std;

size_t hashFileContents(const string& filePath) {
    ifstream fileStream(filePath, ios::binary);
    if (!fileStream) {
        cerr << "Cannot open file: " << filePath << endl;
        return 0;
    }

    stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();

    hash<string> hasher;
    return hasher(buffer.str());
}

int main() {
    string filePath = "fileHash.txt"; // Replace with your file path
    size_t fileHash = hashFileContents(filePath);

    cout << "Hash of the file contents: " << fileHash << endl;
    return 0;
}

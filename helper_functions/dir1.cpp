
/*
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

vector<string> listFilesInDirectory(const string& path) {
    vector<string> files;
    
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_regular_file(entry.status())) {
                files.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return files;
}

int main() {
    string directoryPath = "C:\\Users\\Pc\\Downloads\\cpp"; // Your directory path
    vector<string> files = listFilesInDirectory(directoryPath);

    for (const auto& file : files) {
        cout << file << endl;
    }

    return 0;
}
*/
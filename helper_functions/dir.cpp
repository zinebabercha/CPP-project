#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

vector<string> listFilesInDirectory(const string& path) {
    vector<string> files;
    string command = "dir /b /a-d \"" + path + "\" > temp.txt"; // /a-d lists files only
    system(command.c_str());

    ifstream file("temp.txt");
    string line;
    while (getline(file, line)) {
        files.push_back(line);
    }
    file.close();
    system("del temp.txt"); // Clean up temporary file

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

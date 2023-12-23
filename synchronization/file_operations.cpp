#include "file_operations.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
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
bool areFilesSame(const string& file1, const string& file2) {
    ifstream f1(file1, ifstream::binary | ifstream::ate);
    ifstream f2(file2, ifstream::binary | ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; // File open failed
    }

    if (f1.tellg() != f2.tellg()) {
        return false; // Size mismatch
    }

    // Reset and compare contents
    f1.seekg(0, ifstream::beg);
    f2.seekg(0, ifstream::beg);
    return equal(istreambuf_iterator<char>(f1),
                 istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(f2));
}

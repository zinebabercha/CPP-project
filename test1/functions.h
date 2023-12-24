#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#endif // FUNCTIONS_H

#include <string>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <ostream>
#include <vector>

using namespace std;

// Overload '<<' operator for unordered_map<string, string>
std::ostream &operator<<(std::ostream &os, const std::unordered_map<std::string, std::string> &map)
{
    for (const auto &pair : map)
    {
        os << pair.first << ": " << pair.second << std::endl;
    }
    return os;
}

// Overload '>>' operator for unordered_map<string, string>
std::istream &operator>>(std::istream &is, std::unordered_map<std::string, std::string> &map)
{
    std::string key, value;
    while (is >> key >> value)
    {
        map[key] = value;
    }
    return is;
}

size_t hashFileContents(const string &filePath)
{
    ifstream fileStream(filePath, ios::binary);
    if (!fileStream)
    {
        cerr << "Cannot open file: " << filePath << endl;
        return 0;
    }

    stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();

    hash<string> hasher;
    return hasher(buffer.str());
}

vector<string> listFilesInDirectory(const string &path)
{
    vector<string> files;
    string command = "dir /b /a-d \"" + path + "\" > temp.txt"; // /a-d lists files only
    system(command.c_str());

    ifstream file("temp.txt");
    string line;
    while (getline(file, line))
    {
        files.push_back(line);
    }
    file.close();
    system("del temp.txt"); // Clean up temporary file

    return files;
}

bool areFilesSame(const string &file1, const string &file2)
{
    ifstream f1(file1, ifstream::binary | ifstream::ate);
    ifstream f2(file2, ifstream::binary | ifstream::ate);

    if (f1.fail() || f2.fail())
    {
        return false; // File open failed
    }

    if (f1.tellg() != f2.tellg())
    {
        return false; // Size mismatch
    }

    // Reset and compare contents
    f1.seekg(0, ifstream::beg);
    f2.seekg(0, ifstream::beg);
    return equal(istreambuf_iterator<char>(f1),
                 istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(f2));
}



void copyFile(const string &sourcePath, const string &destPath)
{
    ifstream src(sourcePath, ios::binary);
    ofstream dest(destPath, ios::binary);

    if (!src)
    {
        cerr << "Cannot open source file: " << sourcePath << endl;
        return;
    }
    if (!dest)
    {
        cerr << "Cannot open destination file: " << destPath << endl;
        return;
    }

    dest << src.rdbuf();
}




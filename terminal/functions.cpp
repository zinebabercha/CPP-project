#include <string>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <ostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

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

std::string getLastFolderName(const std::string &path)
{
    size_t lastSlashPos = path.find_last_of('\\');
    if (lastSlashPos != std::string::npos)
    {
        size_t secondLastSlashPos = path.find_last_of('\\', lastSlashPos - 1);
        if (secondLastSlashPos != std::string::npos)
        {
            return path.substr(secondLastSlashPos + 1, lastSlashPos - secondLastSlashPos - 1);
        }
    }
    return "";
}

void copy_directory(const std::string &source, const std::string &destination)
{
    DIR *dir = opendir(source.c_str());
    if (dir == nullptr)
    {
        std::cerr << "Cannot open directory: " << source << std::endl;
        return;
    }

    mkdir(destination.c_str());

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            std::string source_path = source + "/" + entry->d_name;
            std::string destination_path = destination + "/" + entry->d_name;

            struct stat info;
            if (stat(source_path.c_str(), &info) != 0)
            {
                std::cerr << "Cannot access path: " << source_path << std::endl;
                continue;
            }

            if (S_ISDIR(info.st_mode))
            {
                copy_directory(source_path, destination_path);
            }
            else
            {
                std::ifstream src(source_path, std::ios::binary);
                std::ofstream dst(destination_path, std::ios::binary);

                dst << src.rdbuf();

                src.close();
                dst.close();
            }
        }
    }

    closedir(dir);
}

int getGreatestFolderVersion(const std::string &path)
{
    int greatestVersion = -1;
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr)
    {
        std::cerr << "Cannot open directory: " << path << std::endl;
        return 0;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        struct stat info;
        if (stat(path.c_str(), &info) != 0)
        {
            std::cerr << "Cannot access path: " << path << std::endl;
            continue;
        }

        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && S_ISDIR(info.st_mode))
        {
            std::string folderName = entry->d_name;
            int version = std::stoi(folderName);
            if (version > greatestVersion)
            {
                greatestVersion = version;
            }
        }
    }

    closedir(dir);
    return greatestVersion;
}
#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <ostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include<QString>
#include<sstream>

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

// QVector<QString> listFilesInDirectory(const string &path)
// {
//     QVector<QString> files;
//     string command = "dir /b /a-d \"" + path + "\" > temp.txt"; // /a-d lists files only
//     system(command.c_str());

//     ifstream file("temp.txt");
//     string line;
//     while (getline(file, line))
//     {
//         files.push_back(QString::fromStdString(line));
//     }
//     file.close();
//     system("del temp.txt"); // Clean up temporary file

//     return files;
// }

std::string getLastFolderName(const std::string &path)
{
    size_t lastSlashPos = path.find_last_of('/');
    if (lastSlashPos != std::string::npos)
    {
        size_t secondLastSlashPos = path.find_last_of('/', lastSlashPos - 1);
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

std::vector<QString> listFilesInDirectory(const QString& path) {
    QDir dir(path);
    std::vector<QString> files;
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);
    for (const QFileInfo& fileInfo : fileList) {
        files.push_back(fileInfo.fileName());
    }
    return files;
}

void copyFile(const QString& sourcePath, const QString& destPath) {
    QFile srcFile(sourcePath);
    QFile destFile(destPath);

    if (!srcFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open source file:" << sourcePath;
        return;
    }
    if (!destFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open destination file:" << destPath;
        return;
    }

    QByteArray fileData = srcFile.readAll();
    destFile.write(fileData);

    srcFile.close();
    destFile.close();
}

bool areFilesSame(const QString& file1, const QString& file2) {
    QFile f1(file1);
    QFile f2(file2);

    if (!f1.open(QIODevice::ReadOnly) || !f2.open(QIODevice::ReadOnly)) {
        return false; // File open failed
    }

    QByteArray f1Data = f1.readAll();
    QByteArray f2Data = f2.readAll();

    return f1Data == f2Data;
}

void synchronizeDirectories(const QString& sourceDir, const QString& destDir) {
    auto sourceFiles = listFilesInDirectory(sourceDir);
    auto destFiles = listFilesInDirectory(destDir);

    for (const auto& fileName : sourceFiles) {
        QString sourceFilePath = sourceDir + "/" + fileName;
        QString destFilePath = destDir + "/" + fileName;

        if (std::find(destFiles.begin(), destFiles.end(), fileName) != destFiles.end()) {
            if (!areFilesSame(sourceFilePath, destFilePath)) {
                qDebug() << "Updating file:" << fileName;
                copyFile(sourceFilePath, destFilePath);
            }
        } else {
            qDebug() << "Copying new file:" << fileName;
            copyFile(sourceFilePath, destFilePath);
        }
    }
}

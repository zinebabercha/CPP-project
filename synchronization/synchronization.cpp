#include "synchronization.h"
#include "file_operations.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>

using namespace std;


void synchronizeDirectories(const string& sourceDir, const string& destDir) {
    auto sourceFiles = listFilesInDirectory(sourceDir);
    auto destFiles = listFilesInDirectory(destDir);

    for (const auto& fileName : sourceFiles) {
        string sourceFilePath = sourceDir + "\\" + fileName;
        string destFilePath = destDir + "\\" + fileName;

        if (find(destFiles.begin(), destFiles.end(), fileName) != destFiles.end()) {
            if (!areFilesSame(sourceFilePath, destFilePath)) {
                cout << "Updating file: " << fileName << endl;
                copyFile(sourceFilePath, destFilePath);
            }
        } else {
            cout << "Copying new file: " << fileName << endl;
            copyFile(sourceFilePath, destFilePath);
        }
    }
}
    // Optionally handle deletion of files in destination that are not in source


int main() {
    string sourceDir = "C:\\Users\\Pc\\Downloads\\group4_submission_week4"; // Replace with actual source directory path
    string destDir = "C:\\Users\\Pc\\Downloads\\Software engineering\\Software-Engineering"; // Replace with actual destination directory path

    synchronizeDirectories(sourceDir, destDir);
    return 0;
    }
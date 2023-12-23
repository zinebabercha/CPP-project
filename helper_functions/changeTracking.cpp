/*
#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_map>

using namespace std;
using namespace std::filesystem;

class FileChangeTracker {
    unordered_map<string, file_time_type> lastModified;

public:
    void updateDirectory(const string& path) {
        for (const auto& entry : directory_iterator(path)) {
            string filePath = entry.path().string();
            auto lastWriteTime = last_write_time(entry);

            if (lastModified.find(filePath) == lastModified.end()) {
                // New file added
                cout << "Added: " << filePath << endl;
            } else if (lastModified[filePath] != lastWriteTime) {
                // File modified
                cout << "Modified: " << filePath << endl;
            }

            lastModified[filePath] = lastWriteTime;
        }

        // Check for deleted files
        for (auto it = lastModified.begin(); it != lastModified.end(); ) {
            if (!exists(it->first)) {
                cout << "Deleted: " << it->first << endl;
                it = lastModified.erase(it);
            } else {
                ++it;
            }
        }
    }
};

int main() {
    FileChangeTracker tracker;
    tracker.updateDirectory("C:\\Users\\Pc\\Downloads\\cpp"); // Replace with actual directory path
    // Run this periodically or trigger based on some event
}
*/
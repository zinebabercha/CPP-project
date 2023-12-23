#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

// Assume trackFileVersion is implemented as previously shown
void trackFileVersion(const string& workingDirectory, const string& filePath) {
    cout << "Tracking version of file: " << filePath << endl;
    fs::path file_path(workingDirectory + "/" + filePath);
    fs::path repo_dir(workingDirectory);
    fs::path version_path = repo_dir / ".vcs" / "versions" / filePath;

    // Create version directory if it doesn't exist
    if (!fs::exists(version_path.parent_path())) {
        fs::create_directories(version_path.parent_path());
    }

    // Copy the current file to the version directory with a timestamp
    auto timestamp = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string versioned_file_name = to_string(timestamp) + "_" + filePath;
    fs::copy(file_path, version_path.parent_path() / versioned_file_name);}

void rollbackToPreviousVersion(const string& workingDirectory, const string& filePath, const string& version) {
    cout << "Rolling back to previous version of file: " << filePath << endl;
    fs::path repo_dir(workingDirectory);
    fs::path versioned_file_path = repo_dir / ".vcs" / "versions" / filePath / version;

    if (fs::exists(versioned_file_path)) {
        fs::copy(versioned_file_path, repo_dir / filePath, fs::copy_options::overwrite_existing);
        cout << "Rolled back to version: " << version << endl;
    } else {
        cerr << "Version not found: " << version << endl;
    }
}

int main() {
    string workingDirectory = "./TestRepo";
    string testFileName = "testfile.txt";

    // Create test file and track its version
    ofstream testFile(workingDirectory + "/" + testFileName);
    testFile << "This is the original content." << endl;
    testFile.close();
    trackFileVersion(workingDirectory, testFileName);

    // Modify the file and track the new version
    testFile.open(workingDirectory + "/" + testFileName);
    testFile << "This is the modified content." << endl;
    testFile.close();
    trackFileVersion(workingDirectory, testFileName);

    // Rollback to the original version
    // Note: Replace "timestamp_testfile.txt" with the actual versioned file name
    rollbackToPreviousVersion(workingDirectory, testFileName, "timestamp_testfile.txt");

    return 0;
}

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

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
    fs::copy(file_path, version_path.parent_path() / versioned_file_name);
}

int main() {
    string workingDirectory = "./TestRepo";
    string testFileName = "testfile.txt";

    // Ensure the working directory and .vcs/versions directory exist
    fs::create_directories(workingDirectory + "/.vcs/versions");

    // Create a test file
    ofstream testFile(workingDirectory + "/" + testFileName);
    testFile << "This is a test file." << endl;
    testFile.close();

    // Track the file version
    trackFileVersion(workingDirectory, testFileName);

    cout << "Test file version tracked. Check the versions directory." << endl;
    return 0;
}
/*int main() {
    string repoPath = "./TestRepo";
    RepoManager repoManager(repoPath);
    repoManager.initRepository();

    Controller controller(repoPath, repoPath); // Assuming archive directory is the same
    controller.trackFileVersion("testfile.txt");

    cout << "Test completed. Check the versions directory for the file." << endl;
    return 0;
}
*/
/*// In Controller class
class Controller {
    // Existing code...

    void trackFileVersion(const string& filePath) {
        // Logic to maintain a file history log
        cout << "Tracking version of file: " << filePath << endl;
        // Implementation needed
    }

    void rollbackToPreviousVersion(const string& filePath) {
        // Logic to revert a file to a previous version
        cout << "Rolling back to previous version of file: " << filePath << endl;
        // Implementation needed
    }

    // ... (rest of the Controller class)
};

// In main function
int main() {
    string repoPath = "path/to/repo";
    string archivePath = "path/to/archive";
    Controller controller(repoPath, archivePath);

    // Initialize repository
    controller.initRepository();

    // Add file and commit changes
    controller.addFile("example.txt");
    controller.commitChanges("Initial commit");

    // Synchronize directories (if needed)
    // synchronizeDirectories(sourceDir, destDir);

    // Rollback to a previous version (if needed)
    // controller.rollbackToPreviousVersion("example.txt");

    // Other operations...
}
*/
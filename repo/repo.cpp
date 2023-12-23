
/*
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

class VersionControlSystem {
public:
    void initRepository(const string& repoPath) {
        fs::path repo_dir(repoPath);

        if (!fs::exists(repo_dir)) {
            fs::create_directories(repo_dir / ".vcs"); // ".vcs" is the hidden folder for version control
            // Additional setup like creating logs, tracking files, etc.
            cout << "Repository initialized at " << repoPath << endl;
        } else {
            cout << "Repository already exists at " << repoPath << endl;
        }
    }

    // ... (Implementations of other version control functions)
};

int main() {
    VersionControlSystem vcs;
    string repoPath = "C:\\Users\\Pc\\Downloads\\Software engineering\\Software-Engineering";
    vcs.initRepository(repoPath);

    // Further version control operations
}
*/
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

using namespace std;
namespace fs = std::filesystem;

class RepoManager {
private:
    string repoPath;

public:
    RepoManager(const string& path) : repoPath(path) {}

    void initRepository() {
        fs::path repo_dir(repoPath);

        if (!fs::exists(repo_dir)) {
            fs::create_directories(repo_dir / ".vcs"); // ".vcs" for version control metadata
            cout << "Repository initialized at " << repoPath << endl;
        } else {
            cout << "Repository already exists at " << repoPath << endl;
        }
    }

    string getRepoPath() const {
        return repoPath;
    }
};
class VersionControlSystem {
private:
    RepoManager& repoManager;
    unordered_map<string, string> fileHashes; // To track file hashes
    vector<string> commitLog; // To store commit messages

    string getHash(const string& filePath) {
        // Placeholder implementation for hashing file contents
        return to_string(hash<string>{}(filePath));
    }

public:
    VersionControlSystem(RepoManager& manager) : repoManager(manager) {}

    void addFile(const string& filePath) {
        string fullPath = repoManager.getRepoPath() + "/" + filePath;
        fileHashes[filePath] = getHash(fullPath);
        cout << "File added for tracking: " << filePath << endl;
    }

    void commitChanges(const string& message) {
        cout << "Committing changes: " << message << endl;
        for (const auto& [file, hash] : fileHashes) {
            // Logic to save the file's current state as part of the commit
            cout << "Committed " << file << " (hash: " << hash << ")" << endl;
        }
        commitLog.push_back(message);
        fileHashes.clear();
    }

    void log() const {
        cout << "Commit history:" << endl;
        for (const auto& msg : commitLog) {
            cout << msg << endl;
        }
    }

    void status() const {
        // Placeholder for showing the current status
        cout << "Current repository status (placeholder)" << endl;
    }
};
int main() {
    string repoPath = "./MyRepo"; // This creates the repo in the current working directory
    RepoManager repoManager(repoPath);
    repoManager.initRepository();

    VersionControlSystem vcs(repoManager);
    vcs.addFile("rag.txt"); // Make sure this file exists in your repository
    vcs.commitChanges("Initial commit");
    vcs.log();
    vcs.status();

    // Further version control operations
}

//what abt compression
//compare
//track changes
/* public:
    void addFile(const string& filePath) {
        trackFileVersion(filePath);  // Track the file version
        // Additional logic for adding a file
    }

    void commitChanges(const string& message) {
        // Logic to save the current state as a new version
        // Include metadata like timestamp and message
    }

    void rollbackToPreviousVersion(const string& filePath) {
        // Logic to revert a file to a previous version
    }

private:
    void trackFileVersion(const string& filePath) {
        // Logic to track file version
    }
}; */



/*Step 1: Track File States
Modify your VersionControlSystem to keep a record of file states at each commit. This might involve storing file hashes or even the content of files at each commit.

Step 2: Enhance the Commit Function
Update the commitChanges function to actually record the state of each file being committed. This could be as simple as storing the file's hash or as complex as copying the file's content to a special directory within the .vcs folder.

Step 3: Compare File States Across Commits
Implement a function to compare the state of a file across different commits. This function can be used to see what changed in a file from one commit to another.

Step 4: Display Changes in the Log
Enhance the log function to not only show commit messages but also display what files were changed in each commit, and possibly how they were changed.*/



//C:/Users/YourName/Documents/MyRepo
//ignore repo init
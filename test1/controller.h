#ifndef CONTROLLER_H
#define CONTROLLER_H

#endif // CONTROLLER_H


#include <string>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <ostream>
#include <vector>
#include <algorithm>
#include "functions.h"

using namespace std;
namespace fs = std::filesystem;

class Controller
{
private:
    string workingDirectory;
    string repoName;
    int LastVer;
    bool isNewRepo;
    string LastVerPath;
    string username;
    unordered_map<string, string> fileHashes;
    vector<string> commitLog;

public:
    static string archiveDirectory;

    static std::string getGreatestFolderName(const std::string &directoryPath)
    {
        std::string greatestFolderName;
        std::filesystem::path directory(directoryPath);

        if (std::filesystem::is_directory(directory))
        {
            for (const auto &entry : std::filesystem::directory_iterator(directory))
            {
                if (std::filesystem::is_directory(entry))
                {
                    std::string folderName = entry.path().filename().string();
                    if (std::stoi(folderName) > std::stoi(greatestFolderName))
                    {
                        greatestFolderName = folderName;
                    }
                }
            }
        }

        return greatestFolderName;
    }

    static fs::path getLastRepoVersionPath(string repoName, string username)
    {
        fs::path archive_dir(archiveDirectory);
        string lastVersion = getGreatestFolderName(archiveDirectory + "/" + username + "/" + repoName);
        if (lastVersion == "")
            throw("No version found");
        fs::path lastVersionPath = archive_dir / username / repoName / lastVersion;
        return lastVersionPath;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////::

    Controller(){}; // Default constructor

    bool repoExists(string repoName)
    {
        fs::path repo_dir(archiveDirectory + "/" + username + "/" + repoName);
        return fs::exists(repo_dir);
    }

    void start(bool newAccount, string username, string password, string workingDirectory, bool isNewRepo)
    {
        // login/register
        if (newAccount)
        {
            registerUser(username, password);
        }
        login(username, password);

        // search for repo / create new repo
        this->workingDirectory = workingDirectory;
        fs::path repo_dir(workingDirectory);
        repoName = repo_dir.parent_path().filename().string();

        if (!fs::exists(archiveDirectory + "/" + username + "/" + repoName))
        {
            if (isNewRepo)
            {
                this->isNewRepo = true;
                initRepository();
            }
            else
            {
                throw("Repository does not exist");
                return;
            }
        }

        useRepo(workingDirectory, isNewRepo);
    }

    bool login(string username, string password)
    {
        fs::path user_dir(archiveDirectory + "/" + username);
        if (!fs::exists(user_dir))
        {
            throw("User does not exist");
            return false;
        }
        fs::path password_path(user_dir / "password.txt");
        if (fs::exists(password_path))
        {
            ifstream password_file(password_path);
            if (password_file.is_open())
            {
                string passwordFromFile;
                password_file >> passwordFromFile;
                password_file.close();
                if (passwordFromFile != password)
                {
                    throw("Wrong password");
                    return false;
                }
                this->username = username;
                return true;
            }
            else
            {
                cerr << "Cannot open file: " << password_path << endl;
            }
        }
        else
        {
            throw("User does not exist");
            return false;
        }
    }

    bool registerUser(string username, string password)
    {
        fs::path user_dir(archiveDirectory + "/" + username);
        if (fs::exists(user_dir))
        {
            throw("User already exists");
            return false;
        }
        fs::create_directories(user_dir);
        fs::path password_path(user_dir / "password.txt");
        ofstream password_file(password_path);
        if (password_file.is_open())
        {
            password_file << password;
            password_file.close();
            return true;
        }
        else
        {
            cerr << "Cannot open file: " << password_path << endl;
            return false;
        }
    }

    bool useRepo(string repoPath, bool isNewRepo)
    {
        if (isNewRepo)
        {
            LastVer = -1;
            LastVerPath = "";
        }
        else
        {
            LastVer = stoi(getGreatestFolderName(archiveDirectory + "/" + username + "/" + repoPath));
            LastVerPath = getLastRepoVersionPath(repoPath, username).string();
        }
        fs::path archiveDirRepo(LastVerPath);
        if (fs::exists(archiveDirRepo))
        {
            workingDirectory = repoPath;
            fs::path repo_dir(workingDirectory);
            fs::path file_hashes_path = archiveDirRepo / ".vcs" / "file_hashes.txt";
            if (fs::exists(file_hashes_path))
            {
                ifstream file_hashes_file(file_hashes_path);
                if (file_hashes_file.is_open())
                {
                    file_hashes_file >> fileHashes;
                    file_hashes_file.close();
                }
                else
                {
                    cerr << "Cannot open file: " << file_hashes_path << endl;
                }
            }
            return true;
        }
        else if (isNewRepo)
        {
            return true;
        }
        else
        {
            cout << "Repository does not exist" << endl;
            return false;
        }
    }

    void initRepository()
    {
        fs::path repo_dir(workingDirectory);
        fs::path archive_dir(archiveDirectory);

        if (fs::exists(archive_dir / username / repo_dir.parent_path().filename()))
        {
            throw("Repository already exists");
            return;
        }

        fs::create_directories(repo_dir / ".vcs");
        fs::create_directories(archive_dir / username / repo_dir.parent_path().filename());
        cout << "Repository initialized at " << workingDirectory << endl;
    }

    void addFile(const string &fileName)
    {
        fs::path file_path(workingDirectory + "/" + fileName);
        fs::path repo_dir(workingDirectory);
        fs::path archive_dir(archiveDirectory);

        if (!fs::exists(file_path))
        {
            throw("File does not exist");
            return;
        }

        fileHashes[fileName] = to_string(hashFileContents(workingDirectory + "/" + fileName));
        log("File added for tracking: " + fileName);
        cout << "File added to archive" << endl;
    }

    void commitChanges(const string &message)
    {
        cout << "Committing changes: " << message << endl;
        for (const auto &[file, hash] : fileHashes)
        {
            // Logic to save the file's current state as part of the commit
            cout << "Committed " << file << " (hash: " << hash << ")" << endl;
        }

        // Put the map of filehashes in the filehashes file in .vcs
        fs::path repo_dir(workingDirectory);

        fs::path file_hashes_path = repo_dir / ".vcs" / "file_hashes.txt";
        ofstream file_hashes_file(file_hashes_path);
        if (file_hashes_file.is_open())
        {
            file_hashes_file << fileHashes;
            file_hashes_file.close();
        }
        else
        {
            cerr << "Cannot open file: " << file_hashes_path << endl;
        }
        fileHashes.clear();

        // Add deleted and added files to commit log
        vector<string> newFiles = listFilesInDirectory(workingDirectory);
        vector<string> oldFiles = listFilesInDirectory(LastVerPath);
        vector<string> deletedFiles;
        vector<string> addedFiles;
        sort(newFiles.begin(), newFiles.end());
        sort(oldFiles.begin(), oldFiles.end());
        set_difference(oldFiles.begin(), oldFiles.end(), newFiles.begin(), newFiles.end(), back_inserter(deletedFiles));
        set_difference(newFiles.begin(), newFiles.end(), oldFiles.begin(), oldFiles.end(), back_inserter(addedFiles));
        for (const auto &file : deletedFiles)
        {
            commitLog.push_back("Deleted " + file);
        }
        for (const auto &file : addedFiles)
        {
            commitLog.push_back("Added " + file);
        }

        // Add commit log to commit log file
        fs::path commit_log_path = repo_dir / ".vcs" / "commit_log.txt";
        ofstream commit_log_file(commit_log_path, ios::app);
        if (commit_log_file.is_open())
        {
            commit_log_file << message << endl;
            for (const auto &log : commitLog)
            {
                commit_log_file << log << endl;
            }
            commit_log_file.close();
        }
        else
        {
            cerr << "Cannot open file: " << commit_log_path << endl;
        }
        commitLog.clear();

        // Copy whole repo to archive
        fs::path archive_dir(archiveDirectory);
        string newVersion = to_string(LastVer + 1);
        fs::path newVersionPath = archive_dir / username / repo_dir.parent_path().filename() / newVersion;
        fs::create_directories(newVersionPath);
        fs::copy(repo_dir, newVersionPath, fs::copy_options::recursive);

        // Update LastVer and LastVerPath
        LastVer = stoi(newVersion);
        LastVerPath = newVersionPath.string();
    }

    ////////////////////////////////////////////////////////////////////
    void log(const string &message)
    {
        ofstream logFile;
        logFile.open(workingDirectory + "/.vcs/log.txt", ios::app);
        if (!logFile)
        {
            throw("Log file is not open. Message: " + message);
            return;
        }

        logFile << getCurrentTime() << " - " << message << endl;
    }

    string getCurrentTime()
    {
        auto now = time(nullptr);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return buffer;
    }
};


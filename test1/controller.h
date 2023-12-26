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
#include <sys/stat.h>

#include "functions.h"

using namespace std;
namespace fs = std::filesystem;

class Controller
{
private:
    string archiveDirectory;
    string workingDirectory;
    string repoName;
    int LastVer;
    bool isNewRepo;
    string LastVerPath;
    string username;
    unordered_map<string, string> fileHashes;
    vector<string> commitLog;

public:
    Controller()
    {
        this->archiveDirectory = "D:\\OneDrive\\Bureau\\Archive";
    }


    string getLastRepoVersionPath()
    {
        int lastVersion = getGreatestFolderVersion(archiveDirectory + "\\" + username + "\\" + repoName);
        if (lastVersion == -1)
            throw("No version found");
        string lastVersionPath = archiveDirectory + "\\" + username + "\\" + repoName + "\\" + to_string(lastVersion);
        return lastVersionPath;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////::

    bool repoExists(string repoName)
    {
        return fs::exists(archiveDirectory + "\\" + username + "\\" + repoName);
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
        repoName = getLastFolderName(workingDirectory + "\\");

        if (!fs::exists(archiveDirectory + "\\" + username + "\\" + repoName))
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
        string user_dir = archiveDirectory + "\\" + username;
        if (!fs::exists(user_dir))
        {
            throw("User does not exist");
            return false;
        }
        string password_path = user_dir + "\\" + "password.txt";
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
        return false;
    }

    bool registerUser(string username, string password)
    {

        string user_dir = archiveDirectory + "\\" + username;
        if (fs::exists(user_dir))
        {
            throw("User already exists");
            return false;
        }
        mkdir(user_dir.c_str());
        string password_path = user_dir + "\\" + "password.txt";
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

    vector<string> listUserRepos()
    {
        string user_dir = archiveDirectory + "\\" + username;
        vector<string> repos;
        if (fs::exists(user_dir))
        {
            for (const auto &entry : fs::directory_iterator(user_dir))
            {
                if (fs::is_directory(entry))
                {
                    repos.push_back(entry.path().filename().string());
                }
            }
        }
        else
        {
            throw("User does not exist");
        }
        return repos;
    }

    bool useRepo(string repoPath, bool isNewRepo)
    {
        workingDirectory = repoPath;
        cout << "Using repo: " << workingDirectory << endl;
        repoName = getLastFolderName(workingDirectory + "\\");
        if (isNewRepo)
        {
            initRepository();
            LastVer = -1;
            LastVerPath = "";
        }
        else
        {
            LastVer = getGreatestFolderVersion(archiveDirectory + "\\" + username + "\\" + repoName);
            if (LastVer == -1)
                throw("No version found");
            LastVerPath = getLastRepoVersionPath();
        }
        if (!isNewRepo && fs::exists(LastVerPath))
        {
            string file_hashes_path = LastVerPath + "\\" + ".vcs" + "\\" + "file_hashes.txt";
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
        if (fs::exists(archiveDirectory + "\\" + username + "\\" + repoName))
        {
            cout << "not here" << endl;
            cout << archiveDirectory + "\\" + username + "\\" + repoName << endl;
            throw(std::runtime_error("Repository already exists"));
            return;
        }
        cout << "here" << endl;
        mkdir((workingDirectory + "\\.vcs").c_str());
        mkdir((archiveDirectory + "\\" + username + "\\" + repoName).c_str());
        cout << "Repository initialized at " << workingDirectory << endl;
    }

    void addFile(const string &fileName)
    {
        try
        {
            string file_path = workingDirectory + "\\" + fileName;
            cout << file_path << endl;
            if (!fs::exists(file_path))
            {
                throw std::runtime_error("File does not exist");
                return;
            }

            fileHashes[fileName] = to_string(hashFileContents(workingDirectory + "\\" + fileName));
            log("File added for tracking: " + fileName);
            cout << "File added to archive" << endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
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

        string file_hashes_path = workingDirectory + "\\.vcs\\file_hashes.txt";
        ofstream file_hashes_file(file_hashes_path, ios::out);
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
        string commit_log_path = workingDirectory + "\\" + ".vcs" + "\\" + "commit_log.txt";
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
        string newVersion = to_string(LastVer + 1);
        cout << repoName << endl;
        cout << username << endl;
        string newVersionPath = archiveDirectory + "\\" + username + "\\" + repoName + "\\" + newVersion;
        copy_directory(workingDirectory, newVersionPath);

        // Update LastVer and LastVerPath
        LastVer = stoi(newVersion);
        LastVerPath = newVersionPath;
    }

    ////////////////////////////////////////////////////////////////////
    void log(const string &message)
    {
        ofstream logFile;
        logFile.open(workingDirectory + "\\.vcs\\log.txt", ios::app);
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

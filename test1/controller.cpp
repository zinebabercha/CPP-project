#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <functions.cpp>

class Controller {
private:
    QString archiveDirectory;
    QString workingDirectory;
    int lastVer;
    bool isNewRepo;
    QString lastVerPath;
    QMap<QString, QString> fileHashes;
    std::vector<QString> commitLog;

public:
    QString repoName;
    QString username;

    Controller() {
        this->archiveDirectory = "D:/OneDrive/Bureau/Archive";
    }

    QString getLastRepoVersionPath() {
        int lastVersion = getGreatestFolderVersion((archiveDirectory + "/" + username + "/" + repoName).toStdString());
        if (lastVersion == -1) {
            throw std::runtime_error("No version found");
        }
        QString lastVersionPath = archiveDirectory + "/" + username + "/" + repoName + "/" + QString::number(lastVersion);
        return lastVersionPath;
    }

    bool repoExists(const QString& repoName) {
        QDir dir(archiveDirectory + "/" + username + "/" + repoName);
        return dir.exists();
    }

    bool login(const QString &username, const QString &password) {
        QDir userDir(archiveDirectory + "/" + username);
        if (!userDir.exists()) {
            throw std::runtime_error("User does not exist");
        }

        QFile passwordFile(userDir.filePath("password.txt"));
        if (passwordFile.exists()) {
            if (passwordFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&passwordFile);
                QString passwordFromFile = in.readLine();
                if (passwordFromFile != password) {
                    throw std::runtime_error("Wrong password");
                }
                this->username = username;
                return true;
            } else {
                throw std::runtime_error("Cannot open password file");
            }
        } else {
            throw std::runtime_error("User does not exist");
        }
    }

    bool registerUser(const QString& username, const QString& password) {
        QDir userDir(archiveDirectory + "/" + username);
        if (userDir.exists()) {
            throw std::runtime_error("User already exists");
            return false;
        }

        if (!userDir.mkpath(".")) { // Create user directory
            throw std::runtime_error("Cannot create user directory");
            return false;
        }

        QFile passwordFile(userDir.filePath("password.txt"));
        if (passwordFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&passwordFile);
            out << password;
            passwordFile.close();
            return true;
        } else {
            throw std::runtime_error("Cannot open file: " + passwordFile.fileName().toStdString());
            return false;
        }
    }

    std::vector<QString> listUserRepos() {
        QDir userDir(archiveDirectory + "/" + username);
        if (!userDir.exists()) {
            throw std::runtime_error("User does not exist");
        }

        std::vector<QString> repos;
        QFileInfoList entries = userDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo& entry : entries) {
            if (entry.isDir()) {
                repos.push_back(entry.fileName());
            }
        }
        return repos;
    }

    bool useRepo(const QString& repoPath, bool isNewRepo) {
        workingDirectory = repoPath;
        qDebug() << "Using repo: " << workingDirectory;
        repoName = getLastFolderName(workingDirectory);

        if (isNewRepo) {
            initRepository();
            lastVer = -1;
            lastVerPath.clear();
        } else {
            lastVer = getGreatestFolderVersion(archiveDirectory + "/" + username + "/" + repoName);
            if (lastVer == -1) {
                throw std::runtime_error("No version found");
            }
            lastVerPath = getLastRepoVersionPath();
        }

        if (!isNewRepo) {
            QDir versionDir(lastVerPath);
            QFile fileHashesFile(versionDir.filePath(".vcs/file_hashes.txt"));
            if (fileHashesFile.exists()) {
                if (fileHashesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&fileHashesFile);
                    QString line;
                    while (in.readLineInto(&line)) {
                        auto parts = line.split(':');
                        if (parts.size() == 2) {
                            fileHashes.insert(parts[0], parts[1]);
                        }
                    }
                    fileHashesFile.close();
                    return true;
                } else {
                    qDebug() << "Cannot open file: " << fileHashesFile.fileName();
                }
            }
        } else {
            return true;
        }
        qDebug() << "Repository does not exist";
        return false;
    }

    void initRepository() {
        QDir userRepoDir(archiveDirectory + "/" + username + "/" + repoName);
        if (userRepoDir.exists()) {
            qDebug() << "Repository already exists";
            throw std::runtime_error("Repository already exists");
        }

        QDir workingDir(workingDirectory);
        if (!workingDir.mkpath(".vcs")) {
            throw std::runtime_error("Cannot create .vcs directory");
        }
        if (!userRepoDir.mkpath(".")) {
            throw std::runtime_error("Cannot create user repository directory");
        }
        qDebug() << "Repository initialized at " << workingDirectory;
    }

    void addFile(const QString& fileName) {
        try {
            QString filePath = workingDirectory + "/" + fileName;
            qDebug() << filePath;
            QFile file(filePath);
            if (!file.exists()) {
                throw std::runtime_error("File does not exist");
            }

            fileHashes[fileName] = QString::number(hashFileContents(filePath));
            log("File added for tracking: " + fileName);
            qDebug() << "File added to archive";
        } catch (const std::exception& e) {
            qDebug() << e.what();
        }
    }

    void commitChanges(const QString& message) {
        qDebug() << "Committing changes: " << message;
        for (const auto& [file, hash] : fileHashes.toStdMap()) {
            qDebug() << "Committed" << file << "(hash:" << hash << ")";
        }

        // Write file hashes to .vcs/file_hashes.txt
        QFile fileHashesFile(workingDirectory + "/.vcs/file_hashes.txt");
        if (fileHashesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&fileHashesFile);
            for (const auto& [file, hash] : fileHashes.toStdMap()) {
                out << file << ":" << hash << "\n";
            }
            fileHashesFile.close();
        } else {
            qDebug() << "Cannot open file:" << fileHashesFile.fileName();
        }
        fileHashes.clear();

        // Add deleted and added files to commit log
        QVector<QString> newFiles = listFilesInDirectory(workingDirectory);
        QVector<QString> oldFiles = listFilesInDirectory(lastVerPath);
        QVector<QString> deletedFiles;
        QVector<QString> addedFiles;
        std::sort(newFiles.begin(), newFiles.end());
        std::sort(oldFiles.begin(), oldFiles.end());
        std::set_difference(oldFiles.begin(), oldFiles.end(), newFiles.begin(), newFiles.end(), std::back_inserter(deletedFiles));
        std::set_difference(newFiles.begin(), newFiles.end(), oldFiles.begin(), oldFiles.end(), std::back_inserter(addedFiles));
        for (const auto& file : deletedFiles) {
            commitLog.push_back("Deleted " + file);
        }
        for (const auto& file : addedFiles) {
            commitLog.push_back("Added " + file);
        }

        // Write commit log to .vcs/commit_log.txt
        QFile commitLogFile(workingDirectory + "/.vcs/commit_log.txt");
        if (commitLogFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&commitLogFile);
            out << message << "\n";
            for (const auto& log : commitLog) {
                out << log << "\n";
            }
            commitLogFile.close();
        } else {
            qDebug() << "Cannot open file:" << commitLogFile.fileName();
        }
        commitLog.clear();

        // Copy whole repo to archive
        int newVersion = lastVer + 1;
        QString newVersionPath = archiveDirectory + "/" + username + "/" + repoName + "/" + QString::number(newVersion);
        QDir().mkpath(newVersionPath);
        copyDirectory(workingDirectory, newVersionPath);

        // Update lastVer and lastVerPath
        lastVer = newVersion;
        lastVerPath = newVersionPath;
    }

    void commitChanges(const QString& message) {
        qDebug() << "Committing changes: " << message;
        for (const auto& [file, hash] : fileHashes.toStdMap()) {
            qDebug() << "Committed" << file << "(hash:" << hash << ")";
        }

        // Write file hashes to .vcs/file_hashes.txt
        QFile fileHashesFile(workingDirectory + "/.vcs/file_hashes.txt");
        if (fileHashesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&fileHashesFile);
            for (const auto& [file, hash] : fileHashes.toStdMap()) {
                out << file << ":" << hash << "\n";
            }
            fileHashesFile.close();
        } else {
            qDebug() << "Cannot open file:" << fileHashesFile.fileName();
        }
        fileHashes.clear();

        // Add deleted and added files to commit log
        QVector<QString> newFiles = listFilesInDirectory(workingDirectory);
        QVector<QString> oldFiles = listFilesInDirectory(lastVerPath);
        QVector<QString> deletedFiles;
        QVector<QString> addedFiles;
        std::sort(newFiles.begin(), newFiles.end());
        std::sort(oldFiles.begin(), oldFiles.end());
        std::set_difference(oldFiles.begin(), oldFiles.end(), newFiles.begin(), newFiles.end(), std::back_inserter(deletedFiles));
        std::set_difference(newFiles.begin(), newFiles.end(), oldFiles.begin(), oldFiles.end(), std::back_inserter(addedFiles));
        for (const auto& file : deletedFiles) {
            commitLog.push_back("Deleted " + file);
        }
        for (const auto& file : addedFiles) {
            commitLog.push_back("Added " + file);
        }

        // Write commit log to .vcs/commit_log.txt
        QFile commitLogFile(workingDirectory + "/.vcs/commit_log.txt");
        if (commitLogFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&commitLogFile);
            out << message << "\n";
            for (const auto& log : commitLog) {
                out << log << "\n";
            }
            commitLogFile.close();
        } else {
            qDebug() << "Cannot open file:" << commitLogFile.fileName();
        }
        commitLog.clear();

        // Copy whole repo to archive
        int newVersion = lastVer + 1;
        QString newVersionPath = archiveDirectory + "/" + username + "/" + repoName + "/" + QString::number(newVersion);
        QDir().mkpath(newVersionPath);
        copyDirectory(workingDirectory, newVersionPath);

        // Update lastVer and lastVerPath
        lastVer = newVersion;
        lastVerPath = newVersionPath;
    }

    void log(const QString& message) {
        QFile logFile(workingDirectory + "/.vcs/log.txt");
        if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
            throw std::runtime_error("Log file is not open. Message: " + message.toStdString());
            return;
        }

        QTextStream out(&logFile);
        out << getCurrentTime() << " - " << message << "\n";
        logFile.close();
    }

    QString getCurrentTime() {
        QDateTime now = QDateTime::currentDateTime();
        return now.toString("yyyy-MM-dd HH:mm:ss");
    }

};


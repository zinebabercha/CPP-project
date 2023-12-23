#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;

class Logger {
    ofstream logFile;
    string logFileName;

public:
    Logger(const string& fileName) : logFileName(fileName) {
        logFile.open(logFileName, ios::app); // Open in append mode
        if (!logFile) {
            cerr << "Unable to open log file: " << logFileName << endl;
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const string& message) {
        if (!logFile) {
            cerr << "Log file is not open. Message: " << message << endl;
            return;
        }

        logFile << getCurrentTime() << " - " << message << endl;
    }

private:
    string getCurrentTime() {
        auto now = time(nullptr);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return buffer;
    }
};

int main() {
    Logger logger("sync_log.txt");

    logger.log("File synchronization started.");
    // Perform file synchronization operations...
    logger.log("File synchronization completed.");

    return 0;
}

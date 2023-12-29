#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>

class Controller : public QObject {
    Q_OBJECT
private:
    QString archiveDirectory;
    QString workingDirectory;
    int lastVer;
    bool isNewRepo;
    QString lastVerPath;
    QMap<QString, QString> fileHashes;
    std::vector<QString> commitLog;

    QMutex fileAddMutex;     // Mutex for synchronization
    QWaitCondition fileAddCondition;  // Wait condition for queue

public:
    QString repoName;
    QString username;

    Controller();

    QString getLastRepoVersionPath();

    bool repoExists(const QString& repoName);

    void initRepository();

    void log(const QString& message);

    QString getCurrentTime();

    bool useRepo(const QString& repoPath, bool isNewRepo, bool cloneRepo);
    void commitChanges(const QString& message);
    bool addFile(const QString& fileName);
    bool addFileParallel(const QString& fileName);
    bool registerUser(const QString& username, const QString& password);
    bool login(const QString &username, const QString &password);
    std::vector<QString> listUserRepos();

    void pull();
    QVector<QString> getLogLines();
    void clone();
    void emptyInfo();
    void logout();
    bool canCommit();
};


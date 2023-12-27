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

public:
    QString repoName;
    QString username;

    Controller();

    QString getLastRepoVersionPath();

    bool repoExists(const QString& repoName);

    void initRepository();

    void log(const QString& message);

    QString getCurrentTime();

    bool useRepo(const QString& repoPath, bool isNewRepo);
    void commitChanges(const QString& message);
    bool addFile(const QString& fileName);
    bool registerUser(const QString& username, const QString& password);
    bool login(const QString &username, const QString &password);
    std::vector<QString> listUserRepos();


};


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <controller.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showLoginPage();
    void showRegisterPage();
    void showUserReposPage(std::vector<QString>);
    void showRepoActionsPage();


private:
    Ui::MainWindow *ui;
    Controller* controller;
    QStringListModel *listModel;
    void handleLogin();
    void handleRegister();
    void handleLogout();
    void handleBrowseRepos();
    void handleInitializeRepo();
    void handleBackToRepos();
    void handleCheckRepository();
    void handleAddFiles();
    void handleCommitChanges();
    void handleAddSelectedFiles();



};
#endif // MAINWINDOW_H

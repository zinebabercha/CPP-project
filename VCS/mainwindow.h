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
    void showUserReposPage();
    void showRepoActionsPage();
    void showLogWindow();

private:
    Ui::MainWindow *ui;
    Controller* controller;
    QStringListModel *listModel;
    void handleLogin();
    void handleRegister();
    void handleLogout();
    void handleUseRepo();
    void handleAddFiles();
    void handleCommitChanges();
    // void handleAddFilesParallel();
    void handlePull();
    void handleChangeRepo();


};
#endif // MAINWINDOW_H

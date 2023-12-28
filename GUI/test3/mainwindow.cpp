#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtConcurrent/QtConcurrent>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDialog>
#include <iostream>
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = new Controller();
    listModel = new QStringListModel(this);
    ui->listView->setModel(listModel);
    ui->radioButton->setChecked(true);
    showLoginPage();
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::showRegisterPage);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::showLoginPage);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::handleRegister);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::handleUseRepo);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::handleAddFiles);
    // connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::handleAddFilesParallel);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::handlePull);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::showLogWindow);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::handleChangeRepo);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::handleLogout);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::handleCommitChanges);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLoginPage() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showRegisterPage() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showUserReposPage(){
    std::vector<QString> repos = controller->listUserRepos();
    listModel->removeRows(0, listModel->rowCount());
    for(QString repo: repos){
        listModel->insertRow(listModel->rowCount());
        QModelIndex index = listModel->index(listModel->rowCount() - 1);
        listModel->setData(index, repo);
    }
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::showRepoActionsPage(){
    ui->label_12->setText(controller->repoName);
    qDebug() << "here2\n";
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::handleLogin() {
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    // Assuming you have an instance of your Controller named 'controller'
    try {
        if (controller->login(username, password)) {
            // Successful login, you can perform actions or switch to another interface
            qDebug() << "Login successful!";

            ui->label_16->setText(controller->username);
            showUserReposPage();
        } else {
            // Handle unsuccessful login
            qDebug() << "Login failed. Check your credentials.";
            // Display an error message to the user
        }
    } catch (const std::exception &e) {
        // Handle exceptions thrown by the Controller
        qDebug() << "Error during login: " << e.what();
        // Display an error message to the user
    }

}

void MainWindow::handleRegister() {
    // Assuming you have an instance of your Controller named 'controller'
    QString newUsername = ui->lineEdit_3->text();
    QString newPassword = ui->lineEdit_4->text();

    try {
        if (controller->registerUser(newUsername, newPassword)) {
            // Successful registration
            qDebug() << "Registration successful!";

            showLoginPage();
        } else {
            // Handle unsuccessful registration
            qDebug() << "Registration failed. User may already exist.";
            // Display an error message to the user
        }
    } catch (const std::exception &e) {
        // Handle exceptions thrown by the Controller during registration
        qDebug() << "Error during registration: " << e.what();
        // Display an error message to the user
    }
}

void MainWindow::handleUseRepo(){
    controller->useRepo(ui->lineEdit_5->text(), ui->radioButton->isChecked(), ui->radioButton_3->isChecked());
    std::cout << "here\n";
    showRepoActionsPage();
}


void MainWindow::handleAddFiles(){
    QString inputText = ui->textEdit->toPlainText();

    // Split the text into lines
    QStringList lines = inputText.split('\n', QString::SkipEmptyParts);
    ui->textEdit->clear();

    // Iterate over each line
    for (const QString& line : lines) {
        // Process each line (replace this with your logic)
        if(!controller->addFile(line)){
            ui->textEdit->setText(ui->textEdit->toPlainText() + "\nFile: " + line + "not changed!");
        }
        qDebug() << "Line:" << line;
    }
}


// void MainWindow::handleAddFilesParallel() {
//     QString inputText = ui->textEdit->toPlainText();

//     // Split the text into lines
//     QList<QString> lines = inputText.split('\n', QString::SkipEmptyParts);
//     ui->textEdit->clear();

//     // Use QtConcurrent::mapped to run addFile in parallel
//     QFuture<bool> results = QtConcurrent::mapped(lines, controller->addFile);

//     // Iterate over each result
//     int index = 0;
//     for (const bool& result : results) {
//         const QString& line = lines[index++];

//         // Process each result (replace this with your logic)
//         if (!result) {
//             ui->textEdit->setText(ui->textEdit->toPlainText() + "\nFile: " + line + " not changed!");
//         }
//         qDebug() << "Line:" << line;
//     }
// }

void MainWindow::handlePull(){
    controller->pull();
    ui->textEdit->setText("restored to last commited version successfully!");
}


void MainWindow::showLogWindow() {
    // Retrieve log lines
    QVector<QString> logLines = controller->getLogLines();

    // Create a QDialog for the log window
    QDialog logDialog;
    logDialog.setWindowTitle("Version Control System Log");

    // Create a QListWidget to display log lines
    QListWidget* logListWidget = new QListWidget(&logDialog);
    logListWidget->addItems(QStringList::fromVector(logLines));

    // Create a layout for the dialog
    QVBoxLayout* layout = new QVBoxLayout(&logDialog);
    layout->addWidget(logListWidget);

    // Set the layout for the dialog
    logDialog.setLayout(layout);

    // Set the size of the dialog (adjust as needed)
    logDialog.setMinimumSize(400, 300);

    // Show the log window
    logDialog.exec();
}


void MainWindow::handleChangeRepo(){
    controller->emptyInfo();
    showUserReposPage();
}


void MainWindow::handleLogout(){
    controller->logout();
    showLoginPage();
}


void MainWindow::handleCommitChanges(){
    if(!controller->canCommit()){
        std::cout << "commit" << std::endl;
        ui->textEdit->setText("No changes to commit!");
        return;
    }
    bool ok;
    QString userInput = QInputDialog::getText(nullptr, "Commit message", "Enter commit message:", QLineEdit::Normal, "", &ok);

    if (ok && !userInput.isEmpty()) {
        controller->commitChanges(userInput);
        ui->textEdit->setText("Commited changes successfully!");
    } else {
        ui->textEdit->setText("Need commit message to commit!");
    }
}

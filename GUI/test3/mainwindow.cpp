#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::handleInitializeRepo);
    // connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::handleAddFiles);
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

void MainWindow::showUserReposPage(std::vector<QString> repos){
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
            showUserReposPage(controller->listUserRepos());
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

void MainWindow::handleInitializeRepo(){
    controller->useRepo(ui->lineEdit_5->text(), ui->radioButton->isChecked());
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



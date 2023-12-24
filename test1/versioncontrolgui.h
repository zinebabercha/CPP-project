#ifndef VERSIONCONTROLGUI_H
#define VERSIONCONTROLGUI_H

#include <QWidget>
#include <controller.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class VersionControlGUI;
}
QT_END_NAMESPACE

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>

class VersionControlGUI : public QMainWindow {
    Q_OBJECT

public:
    VersionControlGUI(QWidget *parent = nullptr);

private:
    QStackedWidget* stackedWidget;
    QLineEdit* workingDirectoryLineEdit;
    QLineEdit* newRepoNameLineEdit;

    // Buttons for login page
    QPushButton* loginButton;
    QPushButton* registerButton;

    // Buttons for repositories page
    QPushButton* logoutButton;
    QPushButton* browseReposButton;
    QPushButton* initRepoButton;

    // Button for user repositories page
    QPushButton* backToReposButton;

    // Methods for page creation
    void createLoginPage();
    void createRepositoriesPage();
    void createInitializeRepoPage();
    void createUserRepositoriesPage();
    void createWorkOnRepoPage();
    void createListFilesPage();

    // Signal-Slot handling methods
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

// Constructor
VersionControlGUI::VersionControlGUI(QWidget *parent)
    : QMainWindow(parent), stackedWidget(new QStackedWidget(this)),
    workingDirectoryLineEdit(new QLineEdit(this)), newRepoNameLineEdit(new QLineEdit(this)),
    loginButton(new QPushButton("Login", this)), registerButton(new QPushButton("Register", this)),
    logoutButton(new QPushButton("Logout", this)), browseReposButton(new QPushButton("Browse Repositories", this)),
    initRepoButton(new QPushButton("Initialize New Repository", this)),
    backToReposButton(new QPushButton("Back to Repositories", this))
{
    // Create the pages
    createLoginPage();
    createRepositoriesPage();
    createInitializeRepoPage();
    createUserRepositoriesPage();
    createWorkOnRepoPage();
    createListFilesPage();

    // Set up the stacked widget
    setCentralWidget(stackedWidget);

    // Connect signals to slots for navigating between pages
    connect(loginButton, &QPushButton::clicked, this, &VersionControlGUI::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &VersionControlGUI::handleRegister);
    connect(logoutButton, &QPushButton::clicked, this, &VersionControlGUI::handleLogout);
    connect(browseReposButton, &QPushButton::clicked, this, &VersionControlGUI::handleBrowseRepos);
    connect(initRepoButton, &QPushButton::clicked, this, &VersionControlGUI::handleInitializeRepo);
    connect(backToReposButton, &QPushButton::clicked, this, &VersionControlGUI::handleBackToRepos);

    // Set the initial page (login page)
    stackedWidget->setCurrentIndex(0);
}

void VersionControlGUI::createLoginRegisterPage() {
    // Widgets for login page
    usernameLineEdit = new QLineEdit();
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    loginButton = new QPushButton("Login", this);

    // Widgets for register page
    QLineEdit *newUsernameLineEdit = new QLineEdit();
    QLineEdit *newPasswordLineEdit = new QLineEdit();
    newPasswordLineEdit->setEchoMode(QLineEdit::Password);
    QPushButton *registerButton = new QPushButton("Register", this);

    // Login page layout
    QVBoxLayout *loginLayout = new QVBoxLayout();
    QFormLayout *loginFormLayout = new QFormLayout();
    loginFormLayout->addRow("Username:", usernameLineEdit);
    loginFormLayout->addRow("Password:", passwordLineEdit);
    loginLayout->addLayout(loginFormLayout);
    loginLayout->addWidget(loginButton);

    // Register page layout
    QVBoxLayout *registerLayout = new QVBoxLayout();
    QFormLayout *registerFormLayout = new QFormLayout();
    registerFormLayout->addRow("New Username:", newUsernameLineEdit);
    registerFormLayout->addRow("New Password:", newPasswordLineEdit);
    registerLayout->addLayout(registerFormLayout);
    registerLayout->addWidget(registerButton);

    // Pages for login and register
    QWidget *loginPage = new QWidget();
    loginPage->setLayout(loginLayout);

    QWidget *registerPage = new QWidget();
    registerPage->setLayout(registerLayout);

    // Add pages to the stacked widget
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(registerPage);

    // Connect signals to slots for switching between login and register
    connect(registerButton, &QPushButton::clicked, this, &VersionControlGUI::switchToRegisterPage);
    connect(loginButton, &QPushButton::clicked, this, &VersionControlGUI::handleLogin);
}

void VersionControlGUI::switchToRegisterPage() {
    // Switch to the register page (index 1)
    stackedWidget->setCurrentIndex(1);
}

void VersionControlGUI::handleLogin() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    // Assuming you have an instance of your Controller named 'controller'
    try {
        if (controller.login(username.toStdString(), password.toStdString())) {
            // Successful login, you can perform actions or switch to another interface
            qDebug() << "Login successful!";
            // Transition to the next interface or perform other actions
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

void VersionControlGUI::handleRegister() {
    // Assuming you have an instance of your Controller named 'controller'
    QString newUsername = newUsernameLineEdit->text();
    QString newPassword = newPasswordLineEdit->text();

    try {
        if (controller.registerUser(newUsername.toStdString(), newPassword.toStdString())) {
            // Successful registration
            qDebug() << "Registration successful!";

            // Automatically log in the user after registration
            if (controller.login(newUsername.toStdString(), newPassword.toStdString())) {
                // Redirect to user repositories page
                stackedWidget->setCurrentIndex(2);  // Adjust the index based on your page order
            } else {
                qDebug() << "Auto login failed after registration.";
                // Handle the case where auto login fails
            }
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
void VersionControlGUI::createUserRepositoriesPage() {
    // Widget for displaying user repositories
    QListWidget *repoListWidget = new QListWidget();

    // Button for initializing a new repository
    QPushButton *initRepoButton = new QPushButton("Initialize New Repository", this);

    // Layout for the user repositories page
    QVBoxLayout *repoLayout = new QVBoxLayout();
    repoLayout->addWidget(repoListWidget);
    repoLayout->addWidget(initRepoButton);

    // User repositories page
    QWidget *repoPage = new QWidget();
    repoPage->setLayout(repoLayout);

    // Add the user repositories page to the stacked widget
    stackedWidget->addWidget(repoPage);

    // Connect signals to slots for handling repository initialization
    connect(initRepoButton, &QPushButton::clicked, this, &VersionControlGUI::handleInitializeRepo);

    // Populate the repository list
    try {
        vector<string> userRepos = controller.listUserRepos();
        for (const auto &repo : userRepos) {
            repoListWidget->addItem(QString::fromStdString(repo));
        }
    } catch (const std::exception &e) {
        qDebug() << "Error while listing user repositories: " << e.what();
        // Handle the case where an error occurs while listing repositories
    }
}
void VersionControlGUI::handleInitializeRepo() {
    // Assuming you have an instance of your Controller named 'controller'
    try {
        // Check if the user is logged in before allowing repository initialization
        if (controller.isLoggedIn()) {
            // Switch to the page for initializing a new repository (index 2)
            stackedWidget->setCurrentIndex(2);  // Adjust the index based on your page order
        } else {
            qDebug() << "User not logged in. Cannot initialize a repository.";
            // Display a message or take appropriate action when the user is not logged in
        }
    } catch (const std::exception &e) {
        qDebug() << "Error during repository initialization: " << e.what();
        // Handle the case where an error occurs during repository initialization
    }
}
void VersionControlGUI::createInitializeRepoPage() {
    // Widgets for initializing a new repository page
    QLineEdit *newRepoNameLineEdit = new QLineEdit();
    QPushButton *initializeRepoButton = new QPushButton("Initialize Repository", this);

    // Layout for the initializing repository page
    QVBoxLayout *initRepoLayout = new QVBoxLayout();
    QFormLayout *initRepoFormLayout = new QFormLayout();
    initRepoFormLayout->addRow("New Repository Name:", newRepoNameLineEdit);
    initRepoLayout->addLayout(initRepoFormLayout);
    initRepoLayout->addWidget(initializeRepoButton);

    // Initializing repository page
    QWidget *initRepoPage = new QWidget();
    initRepoPage->setLayout(initRepoLayout);

    // Add the initializing repository page to the stacked widget
    stackedWidget->addWidget(initRepoPage);

    // Connect signals to slots for handling repository initialization
    connect(initializeRepoButton, &QPushButton::clicked, this, &VersionControlGUI::handleRepositoryInitialization);
}
void VersionControlGUI::handleRepositoryInitialization() {
    QString newRepoName = newRepoNameLineEdit->text();

    // Assuming you have an instance of your Controller named 'controller'
    try {
        // Perform repository initialization using the new repository name
        // Call the necessary methods in your Controller to initialize the repository

        // Display a success message or handle further actions
        qDebug() << "Repository initialized successfully.";

        // Switch back to the user repositories page (index 1)
        stackedWidget->setCurrentIndex(1);  // Adjust the index based on your page order
    } catch (const std::exception &e) {
        // Handle the case where an error occurs during repository initialization
        qDebug() << "Error during repository initialization: " << e.what();
        // Display an error message to the user or take appropriate actions
    }
}
void VersionControlGUI::createWorkOnRepoPage() {
    // Widgets for working on a repository page
    QLineEdit *workingDirectoryLineEdit = new QLineEdit();
    QPushButton *checkRepoButton = new QPushButton("Check Repository", this);
    QPushButton *addFilesButton = new QPushButton("Add Files", this);
    QPushButton *commitChangesButton = new QPushButton("Commit Changes", this);

    // Layout for the working on a repository page
    QVBoxLayout *workOnRepoLayout = new QVBoxLayout();
    QFormLayout *workOnRepoFormLayout = new QFormLayout();
    workOnRepoFormLayout->addRow("Working Directory:", workingDirectoryLineEdit);
    workOnRepoLayout->addLayout(workOnRepoFormLayout);
    workOnRepoLayout->addWidget(checkRepoButton);
    workOnRepoLayout->addWidget(addFilesButton);
    workOnRepoLayout->addWidget(commitChangesButton);

    // Working on a repository page
    QWidget *workOnRepoPage = new QWidget();
    workOnRepoPage->setLayout(workOnRepoLayout);

    // Add the working on a repository page to the stacked widget
    stackedWidget->addWidget(workOnRepoPage);

    // Connect signals to slots for handling repository check and actions
    connect(checkRepoButton, &QPushButton::clicked, this, &VersionControlGUI::handleCheckRepository);
    connect(addFilesButton, &QPushButton::clicked, this, &VersionControlGUI::handleAddFiles);
    connect(commitChangesButton, &QPushButton::clicked, this, &VersionControlGUI::handleCommitChanges);
}
void VersionControlGUI::handleCheckRepository() {
    QString workingDirectory = workingDirectoryLineEdit->text();

    // Assuming you have an instance of your Controller named 'controller'
    try {
        // Perform repository existence check using the working directory
        if (controller.repoExists(workingDirectory.toStdString())) {
            // Display a success message or handle further actions
            qDebug() << "Repository exists in the archive.";

            // Show options for adding files and committing changes
            stackedWidget->setCurrentIndex(3);  // Adjust the index based on your page order
        } else {
            // Display a message indicating that the repository does not exist
            qDebug() << "Repository does not exist in the archive.";
        }
    } catch (const std::exception &e) {
        // Handle the case where an error occurs during repository existence check
        qDebug() << "Error during repository existence check: " << e.what();
        // Display an error message to the user or take appropriate actions
    }
}

void VersionControlGUI::handleAddFiles() {
    // Assuming you have an instance of your Controller named 'controller'
    try {
        // Implement the logic for handling the addition of files
        // You may want to open a file dialog or provide a way for the user to select files

        // Example: Open a file dialog to select files
        QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select Files to Add");

        // Process the selected files and call the corresponding Controller method
        for (const QString &fileName : fileNames) {
            // Assuming you have a method in your Controller for adding files
            controller.addFile(fileName.toStdString());
        }

        // Display a success message or handle further actions
        qDebug() << "Files added successfully.";

        // Optionally, switch to the commit changes page or perform other actions
        // stackedWidget->setCurrentIndex(4);  // Adjust the index based on your page order
    } catch (const std::exception &e) {
        // Handle the case where an error occurs during file addition
        qDebug() << "Error during file addition: " << e.what();
        // Display an error message to the user or take appropriate actions
    }
}

void VersionControlGUI::handleCommitChanges() {
    // Assuming you have an instance of your Controller named 'controller'
    try {
        // Implement the logic for handling the commitment of changes
        // Perform the necessary actions in your Controller

        // Example: Open a dialog for the user to input a commit message
        bool ok;
        QString commitMessage = QInputDialog::getText(this, "Commit Changes", "Enter commit message:", QLineEdit::Normal, "", &ok);

        if (ok && !commitMessage.isEmpty()) {
            // Assuming you have a method in your Controller for committing changes
            controller.commitChanges(commitMessage.toStdString());

            // Display a success message or handle further actions
            qDebug() << "Changes committed successfully.";

            // Optionally, switch to the user repositories page or perform other actions
            // stackedWidget->setCurrentIndex(1);  // Adjust the index based on your page order
        } else {
            qDebug() << "Commit message is empty or canceled.";
            // Handle the case where the commit message is empty or canceled
        }
    } catch (const std::exception &e) {
        // Handle the case where an error occurs during commit changes
        qDebug() << "Error during commit changes: " << e.what();
        // Display an error message to the user or take appropriate actions
    }
}
void VersionControlGUI::createListFilesPage() {
    // Widgets for listing files page
    QListWidget *filesListWidget = new QListWidget();
    QPushButton *addSelectedFilesButton = new QPushButton("Add Selected Files", this);

    // Layout for the listing files page
    QVBoxLayout *listFilesLayout = new QVBoxLayout();
    listFilesLayout->addWidget(filesListWidget);
    listFilesLayout->addWidget(addSelectedFilesButton);

    // Listing files page
    QWidget *listFilesPage = new QWidget();
    listFilesPage->setLayout(listFilesLayout);

    // Add the listing files page to the stacked widget
    stackedWidget->addWidget(listFilesPage);

    // Connect signals to slots for handling file selection
    connect(addSelectedFilesButton, &QPushButton::clicked, this, &VersionControlGUI::handleAddSelectedFiles);
}

#endif // VERSIONCONTROLGUI_H

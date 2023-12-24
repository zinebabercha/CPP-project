#include "versioncontrolgui.h"
#include "ui_versioncontrolgui.h"

VersionControlGUI::VersionControlGUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VersionControlGUI)
{
    ui->setupUi(this);
}

VersionControlGUI::~VersionControlGUI()
{
    delete ui;
}

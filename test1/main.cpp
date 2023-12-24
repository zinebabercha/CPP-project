#include "versioncontrolgui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VersionControlGUI w;
    w.show();
    return a.exec();
}

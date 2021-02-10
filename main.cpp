#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QApplication::setOrganizationDomain(STR_ORG_DOMAIN);
    QApplication::setOrganizationName(STR_ORG_NAME);
    QApplication::setApplicationVersion(STR_APP_VERSION);
    QApplication::setApplicationName(STR_APP_NAME);

    MainWindow window;
    window.show();

    return application.exec();
}

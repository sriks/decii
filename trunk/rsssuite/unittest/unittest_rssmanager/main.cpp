#include <QtCore/QCoreApplication>
#include "test_rssmanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    test_rssmanager test;
    test.manualTestAll();
    return a.exec();
}

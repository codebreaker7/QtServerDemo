#include <QCoreApplication>
#include <QtNetwork>
#include "dataserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataServer * srv = new DataServer();
    qInfo("Server started!");

    return a.exec();
}


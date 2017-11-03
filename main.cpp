#include <QCoreApplication>
#include "CGatewayInterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    new CGatewayInterface();

    return a.exec();
}


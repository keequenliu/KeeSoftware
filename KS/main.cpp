#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>

#include "Log/Log.h"
#include "Network/Socket.h"

using namespace KS::LOG;
using namespace KS::Net;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    test();

//    Logger()<<"this is test";

    Logger().log()<<"this is test";
    Logger().log()<<"中国OK";

    MainWindow w;
    w.show();

    return a.exec();
}

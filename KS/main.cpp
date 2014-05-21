#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>

#include "Log/Log.h"
#include "Network/Socket.h"
#include "Utils/Time.h"

#include <pthread.h>

using namespace KS::LOG;
using namespace KS::Net;
using namespace KS::Utils;




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    test();

//    Logger()<<"this is test";

    Time tick;
    tick.setStartTick();

    Logger().log()<<"this is test";
    Logger().log()<<"中国OK";

    Time t=Time::getCurrentTime();
    std::cerr<<"sec: "<<t.second()<<", usec :"<<t.microSecond()<<std::endl;


    std::cerr<<"cur time: "<<t.toString()<<std::endl;

    MainWindow w;
    w.show();

    std::cerr<<"e t:: "<<tick.time_s()<<std::endl;

    return a.exec();
}

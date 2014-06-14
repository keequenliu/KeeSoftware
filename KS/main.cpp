//#include <QtGui/QApplication>
//#include "mainwindow.h"
//#include <QDebug>

#include "Log/Log.h"
#include "Network/Socket.h"
#include "Utils/Time.h"
#include "OSG/CoreViewer.h"

//#include <pthread.h>

using namespace KS::LOG;
//using namespace KS::Net;
using namespace KS::Utils;


int main(int argc, char *argv[])
{

    CoreViewer v;
    v.start();

    return 0;
}

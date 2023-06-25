//#include "mainwindow.h"
#include "control.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    control w;
    w.show();
    return a.exec();
}

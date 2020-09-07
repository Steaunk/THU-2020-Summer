#include "mainwindow.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    srand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QApplication a(argc, argv);
    MainWindow w(argc, argv);
    w.show();
    return a.exec();
}

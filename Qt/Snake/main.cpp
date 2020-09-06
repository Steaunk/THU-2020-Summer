/**@mainpage  贪吃蛇
* <table>
* <tr><th>Project  <td>贪吃蛇
* <tr><th>Author   <td>Wang Jiannan
* </table>
*/
/**@file  main.cpp
* @brief  项目主函数文件
* @author      Wang Jiannan
* @date        2020-8-24
*/
#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
    srand(time(0));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

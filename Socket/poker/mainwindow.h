#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QMainWindow>
#include <QDebug>
#include <QtNetwork>

#include "server.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/**
 * @brief The MainWindow class 斗地主游戏的准备界面
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc, char *argv[], QWidget *parent = nullptr);
    ~MainWindow();
    int player;
private:
    Ui::MainWindow *ui;
    QButtonGroup *btnGroup;
    Server *playerConnect;
    Game *game = nullptr;
signals:
    void startConnection(int);
private slots:
    /**@brief
     *
     */
    void on_connectButton_clicked();
    void ready();
};
#endif // MAINWINDOW_H

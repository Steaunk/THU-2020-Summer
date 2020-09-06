/**@file  mainwindow.h
* @brief  主要管理整个游戏的逻辑
* @author Wang Jiannan
* @date 2020-8-24
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QKeyEvent>
#include <QPainter>
#include <QToolBar>
#include "painter.h"
#include "snake.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**@brief 初始化
     * @details 各变量初始化，信号与槽的 connect
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /**@brief 处理键盘事件
     * @details 处理方向键，控制蛇的移动
     */
    void keyPressEvent(QKeyEvent *ev);
    /**@brief 处理绘制事件
     * @details 调用 Painter 中函数
     */
    void paintEvent(QPaintEvent *ev);
    /**@brief 处理鼠标事件
     * @details 在游戏地图内点击后，调用 Snake 中 reverse 函数
     */
    void mousePressEvent(QMouseEvent *ev);
    /**@brief 处理时间事件
     */
    void timerEvent(QTimerEvent *ev);

private:
    /**@brief 未开始状态
     */
    void readyStatus();
    /**@brief 游戏状态
     */
    void gameStatus();
    /**@brief 暂停状态
     */
    void pauseStatus();
    /**@brief 开始一局新游戏
     * @details Snake 初始化，重新绘制游戏地图
     */
    void newGame();

    Ui::MainWindow *ui;
    Painter *painter;
    Snake *snake;
    int status;

private slots:
    /**@brief 暂停事件
     * @details 调用 Snake 中 readFile 函数
     */
    void pause();
    /**@brief 开始游戏事件
     */
    void play();
    /**@brief 加载事件
     * @details 调用 Snake 中 readFile 函数
     */
    void load();
    /**@brief 重新游戏事件
     */
    void restart();
    /**@brief 继续游戏事件
     */
    void resume();
    /**@brief 保存游戏事件
     * @details 调用 Snake 中 saveFile 函数
     */
    void savefile();
    /**@brief 终止状态
     */
    void endStatus();
};
#endif // MAINWINDOW_H

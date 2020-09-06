/**@file painter.h
* @brief  负责绘制游戏地图
* @author Wang Jiannan
* @date 2020-8-24
  */
#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QPainter>
#include "snake.h"

class Painter
{
public:
    static const int base = 10;
    Painter();
    /**@brief 绘制游戏地图背景
     */
    void background(QPainter *p);
    /**@brief 绘制游戏地图中当前游戏局面
     * @details 绘制蛇、障碍、食物
     */
    void game(QPainter *p, Snake *s);
};

#endif // PAINTER_H

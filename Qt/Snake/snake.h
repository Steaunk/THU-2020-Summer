/**@file  snake.h

* @brief  负责蛇在游戏地图上的活动
* @details 负责蛇的移动、成长、吃食物、判断是否蛇死亡，游戏开始蛇的随机生成、每次食物的随机生成
* @author Wang Jiannan
* @date 2020-8-24
  */
#ifndef SNAKE_H
#define SNAKE_H

#include <cstdlib>
#include <ctime>
#include <vector>
#include <QObject>
#include <algorithm>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QDataStream>

class Snake: public QObject
{
    Q_OBJECT
public:
    Snake();
    ~Snake();
private:
    /// 蛇的长度
    int length;
    /// 剩余增长长度
    int res;
    /// 当前时间
    int cnt;
    /// 游戏地图
    int map[40][40];
    /**@brief 在 map 内寻找一个指定大小元素
     */
    void find(int &x, int &y, int d);
    /**@brief 在 map 内寻找所有大小元素
     * @prama lim 与边界距离
     */
    std::vector<int*> find(int d, int lim);
public:
    /**@brief 随机生成食物
     */
    void genFood();
    /**@brief 初始化地图
     */
    void initMap();
    /**@brief 指定位置进行反转
     * @detail 实现非障碍物与障碍物的转化
     */
    void reverse(const int &, const int &);
    /**@brief 初始化游戏
     */
    void initGame();
    /**@brief 获取蛇的长度
     */
    int getLen();
    /**@brief 获取 map 中的值
     * @return 返回格点类型
     * 	@retval -2 障碍
     * 	@retval -1 食物
     * 	@retval 0 空白点
     * 	@retval >0 蛇，其中蛇头值与 length 相等
     */
    int operator () (const int &x, const int &y);
    /**@brief 蛇向格点 (x,y) 伸长
     * @detail 若 res 为零，则将蛇所在格点值全部减一
     */
    void grow(const int &x, const int &y);
    /**@brief 操控蛇向四周移动
     * @param dir 移动方向，若非 0~3 则为默认移动方向
     * 	- 0 左
     * 	- 1 上
     * 	- 2 右
     * 	- 3 下
     */
    void move(int dir);
    /**@brief 保存游戏状态
     */
    void saveFile(QString filename);
    /**@brief 读取游戏状态
     */
    void readFile(QString filename);
signals:
    /**@brief 蛇死亡，进入终止状态
     */
    void death();
    /**@brief 更新计时器
     */
    void refreshTimer(int);
};

#endif // SNAKE_H

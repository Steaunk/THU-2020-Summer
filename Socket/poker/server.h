/**@file  server.h
* @brief  主要负责服务器、客户端连接通信
* @author Wang Jiannan
* @date 2020-9-02
*/
#ifndef CONNECT_H
#define CONNECT_H

#include <QtNetwork>
#include <QMessageBox>
#include <QObject>
#include <QDebug>
#include <QInputDialog>
#include <QRegExp>
#include <vector>
using namespace std;

class Server;
/**
 * @brief The MyTcp class 组合了服务端和客户端
 */
class MyTcp : public QObject{
    Q_OBJECT
private:
    Server *parent;
    QTcpServer *l;
    QTcpSocket *s;
public:
    MyTcp(Server *parent = nullptr);
    /**
     * @brief startClient 开启客户端
     * @param ip 服务器ip地址
     * @param port 服务器网络端口
     * @param playername 服务器对象名称
     */
    void startClient(QString ip, int port, QString playername);
    /**
     * @brief startServer 开启服务端
     * @param port 服务器网络端口
     */
    void startServer(int port);
    /**
     * @brief write 发送信息
     */
    void write(QByteArray);
public slots:
    void acceptConnection();
    void recvMsg();
};

/**
 * @brief The Server class 斗地主专用通信工具
 */
class Server : public QObject
{
    Q_OBJECT
private:
    int cnt = 0;
    /// 值为 1 时，不用输入 ip 参数；值为 0 时需输入 ip 参数
    bool testMode;
    /// s1 下家；s2 上家
    MyTcp *s1, *s2;
public:
    Server(bool testmode = 0);
    ~Server();
    /**
     * @brief inputIP 输入IP地址
     */
    void inputIP(QString&, int&, QString);
    /**
     * @brief inputPort 输入网络端口
     */
    int inputPort(int def, QString msg);
    /**
     * @brief countDown 记录其他玩家数
     * 达到 2 名玩家时，游戏开始
     */
    void countDown();
signals:
    void ready();
    void dealMsg(QByteArray);
private slots:
    void startConnection(int);
public slots:
    /**
     * @brief sendMsg 发送信息
     * @param id 0 发送给下家； 1 发送给上家； -1 发送给所有玩家
     */
    void sendMsg(const QByteArray &, int id = -1);
};

#endif // CONNECT_H

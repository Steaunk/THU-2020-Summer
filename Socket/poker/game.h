#ifndef GAME_H
#define GAME_H

#include "ui_game.h"
#include "poker.h"
#include <QMainWindow>
#include <QKeyEvent>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QWidget>

namespace Ui {
class Game;
}

/**
 * @brief The Game class 斗地主游戏的游戏界面
 */
class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(int player, QMainWindow *, QWidget *parent = nullptr);
    ~Game();
    /**
     * @brief typeMsg 生成信息格式中的发信人和信息类型
     * @param id 信息类型
     */
    QByteArray typeMsg(int id);
private:
    int player, cnt, landlord, player_p;
    bool showLandlord, isYourTurn = 0;
    QMainWindow *last;
    Poker mycards, landlordcards, lastcards;
    int cntCard[2];
    bool pass[3];
    QMediaPlaylist *playlist;
    QMediaPlayer *mediaplayer;
    PokerTool *tool;
    Ui::Game *ui;
    void init();
    /**
     * @brief startGame 游戏准备完毕
     * 向其他主机发送信息，表明该主机已准备完毕，当全部完毕时，由 0 号主机发牌及随机游戏顺序
     */
    void startGame();
    /**
     * @brief beginGame 游戏开始
     */
    void beginGame();
    /**
     * @brief bidLandlord 开始叫地主
     */
    void bidLandlord();
    /**
     * @brief playCards 出牌
     */
    void playCards();
    /**
     * @brief newRound 新一轮出牌开始
     */
    void newRound();
    void setUser(int);
    /**
     * @brief endGame 游戏结束
     */
    void endGame(int);
    /**
     * @brief newRoundClear 新一轮出牌时清除状态栏
     */
    void newRoundClear();
    /**
     * @brief Win 取得胜利
     * 提示 WIN 以及播放胜利音乐
     */
    void Win();
    /**
     * @brief Lose 失败
     * 提示 LOSE 以及播放失败音乐
     */
    void Lose();
private slots:
    /**
     * @brief dealMsg 处理信息
     * 处理收到的信息，并按照信息类型执行相应的操作
     */
    void dealMsg(QByteArray msg);
    void on_callButton_clicked();
    void on_uncallButton_clicked();
    void on_playButton_clicked();
    void on_passButton_clicked();
    void on_restartButton_clicked();
    void on_quitButton_clicked();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
signals:
    void sendMsg(QByteArray, int);
    void sendMsg(QByteArray);
};

#endif // GAME_H

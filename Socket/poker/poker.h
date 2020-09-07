/**@file  poker.h
* @brief  主要负责处理扑克牌、扑克牌叠和一些对扑克的操作
* @author Wang Jiannan
* @date 2020-9-03
*/
#ifndef POKER_H
#define POKER_H

#include <QString>
#include <vector>
#include <algorithm>
#include <QTime>
#include <QDebug>
#include <functional>
#include <QPainter>
/**
 * @brief The Card class 单张扑克牌
 * 存储单张扑克牌的信息，以及对其信息的查询，和两张扑克牌的比较
 */
class Card{
private:
    char a[2];
    bool chosen;
public:
    Card() = default;
    Card(char, char, bool chosen = 0);
    Card(const QString &);
    bool getChosen() const;
    /**
     * @brief reverse 将 chosen 信息取反
     */
    void reverse();
    /**
     * @brief operator () 扑克牌面信息
     */
    QString operator() () const;
    void setChosen(bool);
    /**
     * @brief name 查询该扑克对应的图片名称
     */
    QString name() const;
    int isJoker() const;
    bool operator < (const Card &A) const;
    bool operator == (const Card &A) const;
    bool operator != (const Card &A) const;
    Card operator + (const int &k) const;
    friend QDebug operator << (QDebug debug, const Card &A){
        debug.nospace() << '(' << A.a[0] << ", " << A.a[1] << ')';
        return debug;
    }
};
/**
 * @brief The Poker class 一组扑克
 * 按照大小顺序存储扑克牌，可进行添加、移除、选择、输出信息
 */
class Poker
{
private:
    std::vector<Card> cards;
public:
    Poker() = default;
    Poker(std::vector<Card> cards);
    void addCard(const Card &A);
    void addCards(const std::vector<Card> &cards);
    /**
     * @brief cntCards 统计牌数
     */
    int cntCards() const;
    void removeCards(const std::vector<Card> &cards);
    void setAllChosen(bool d = false);
    void revCard(int id);
    bool getChosen(int id) const;
    /**
     * @brief chosenCards 取出所有被选中的牌
     */
    Poker chosenCards();
    std::vector<Card> Cards() const;
    void clear();
    /**
     * @brief write 查询该牌组所有扑克的信息
     * @return
     */
    QByteArray write();
    bool operator < (const Poker &) const;
    Card operator [] (const int &id) const;
    friend QDebug operator << (QDebug debug, const Poker &A){
        debug.nospace() << '[';
        auto tmp = A.cards;
        for(auto i : A.cards) debug.maybeSpace() << i;
        debug.nospace() << ']';

        return debug;
    }
};
/**
 * @brief The PokerTool class 扑克游戏工具箱
 * 集合了一些进行扑克游戏需要的工具，如：发牌、判断是否可以出牌等
 */
class PokerTool{
private:
    /// 一副扑克牌所有牌的集合
    std::vector<Card> allcards;
    Poker pokers[4];
    std::vector<QString(PokerTool::*)(const Poker &) const> funcSet;
public:
    PokerTool();
    /**
     * @brief deal 发牌
     * 按照斗地主规则发牌，随机选出 17、17、17、3 共四份牌，存储在 poker[] 中
     */
    void deal();
    /**
     * @brief read 读取卡牌
     * 根据获得的字符串，识别读取到的扑克
     */
    Poker read(const QByteArray &);
    /**
     * @brief getPoker 发牌后，获取相应的牌叠
     */
    Poker getPoker(int);
    /**
     * @brief check 判断给定牌是否合法
     * 按照斗地主规则判断。返回 "F" 则为不合法；反之，共返回长度为四的字符串，前两位为牌型，后两位为大小
     * @return
     * 		- "F" Fail
     * 		- "Ro" Rocket
     * 		- "Si" Single Card
     * 		- "Pa" Pair
     * 		- "Bo" Bomb
     * 		- "Tr" Triplet
     * 		- "TS" Triplet with Single Card
     * 		- "TP" Triplet with Pair
     * 		- "Se" Sequence of Single Cards
     * 		- "SP" Sequence of Pairs
     * 		- "ST" Sequence of Triplets
     * 		- "S1" Sequence of Triplets with Single Card
     * 		- "S2" Sequence of Triplets with Pairs
     * 		- "QS"/"QP" Quadplex Set
     */
    QString check(const Poker &) const;
    /**
     * @brief canPlay 判断给定牌是否可打出
     * 按照斗地主规则判断。当上家打出一定牌 B 时，判断能否打出 A
     * @param A 此时己方打出的牌
     * @param B 上家打出的牌，若为 "" 则无上家
     */
    bool canPlay(const Poker &A, const Poker &B) const;
    std::vector<Poker> sortPoker(const Poker &) const;
private:
    /**
     * @brief single 单牌
     */
    QString single(const Poker &) const;
    /**
     * @brief rocket 火箭
     */
    QString rocket(const Poker &) const;
    /**
     * @brief pair 对子
     */
    QString pair(const Poker &) const;
    /**
     * @brief bomb 炸弹
     */
    QString bomb(const Poker &) const;
    /**
     * @brief triplet 三张牌
     */
    QString triplet(const Poker &) const;
    /**
     * @brief tripletWithSingle 三带一
     */
    QString tripletWithSingle(const Poker &) const;
    /**
     * @brief tripletWithPair 三带二
     */
    QString tripletWithPair(const Poker &) const;
    /**
     * @brief sequence 单顺子
     */
    QString sequence(const Poker &) const;
    /**
     * @brief sequenceOfPairs 双顺子
     */
    QString sequenceOfPairs(const Poker &) const;
    /**
     * @brief sequenceOfTriplets 三顺子
     */
    QString sequenceOfTriplets(const Poker &) const;
    /**
     * @brief sequenceOfTripletsWithSingle 飞机带单牌
     */
    QString sequenceOfTripletsWithSingle(const Poker &) const;
    /**
     * @brief sequenceOfTripletsWithPair 飞机带双牌
     */
    QString sequenceOfTripletsWithPair(const Poker &) const;
    /**
     * @brief quadplexSet 四带二
     */
    QString quadplexSet(const Poker &) const;

};

#endif POKER_H

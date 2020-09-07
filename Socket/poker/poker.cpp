#include "poker.h"

Card::Card(char t1, char t2, bool chosen): a{t1, t2}, chosen(chosen){}
Card::Card(const QString &s){
    a[0] = s.at(0).toLatin1();
    a[1] = s.at(1).toLatin1();
}
QString Card::operator() () const{
    return QString(a[0]) + QString(a[1]);
}
bool Card::getChosen() const { return chosen; }
void Card::reverse(){ chosen ^= 1; }
void Card::setChosen(bool t){ chosen = t; }
int Card::isJoker() const{
    if(a[0] != 'J') return 0;
    else return a[1] - '0';
}
bool Card::operator < (const Card &A) const{
    if(isJoker() && A.isJoker()) return isJoker() < A.isJoker();
    else if(isJoker() || A.isJoker()) return A.isJoker();
    else if(a[1] != A.a[1]) return a[1] < A.a[1];
    else return a[0] < A.a[0];
}
bool Card::operator==(const Card &A) const {
    if(isJoker() && A.isJoker()) return 1;
    if(a[1] == A.a[1]) return 1;
    return 0;
}
bool Card::operator!=(const Card &A) const{
    return !(*this == A);
}
Card Card::operator + (const int &k) const{
    if(a[1] + k - '0' > 14 || isJoker()) return (Card){'N', '0'};
    Card tmp = *this;
    tmp.a[1] += k;
    return tmp;
}
QString Card::name() const{
    return QString(a[0]) + QString::number(a[1] - '0');
}

Poker::Poker(std::vector<Card> cards){
    this->cards = cards;
}
void Poker::setAllChosen(bool d){
    for(auto i : cards) i.setChosen(d);
}
void Poker::revCard(int id){
    if(id >= cntCards()) return ;
    cards[id].reverse();
}
void Poker::addCard(const Card &A){
    cards.push_back(A);
    std::sort(cards.begin(), cards.end());
}
void Poker::addCards(const std::vector<Card> &cards){
    this->cards.insert(this->cards.end(), cards.begin(), cards.end());
    std::sort(this->cards.begin(), this->cards.end());
}
int Poker::cntCards() const{
    return cards.size();
}
void Poker::removeCards(const std::vector<Card> &cards){
    for(auto i = cards.begin(); i != cards.end(); ++i){
        for(auto j = this->cards.begin(); j != cards.end(); ++j){
            if(*i == *j){
                this->cards.erase(j);
                break;
            }
        }
    }
    std::sort(this->cards.begin(), this->cards.end());
}
bool Poker::getChosen(int id) const{
    if(id >= cntCards()) return false;
    return cards[id].getChosen();
}
void Poker::clear(){ cards.clear(); }
std::vector<Card> Poker::Cards() const{
    return cards;
}
bool Poker::operator<(const Poker &A) const{
    if(cntCards() != A.cntCards()) return cntCards() < A.cntCards();
    return this->operator[](0) < A[0];
}
Poker Poker::chosenCards(){
    Poker t;
    for(auto i : cards){
        if(i.getChosen() == 1)
            t.addCard(i);
    }
    return t;
}
QByteArray Poker::write(){
    QByteArray data;
    for(auto i : cards)
        data.append(i());
    return data;
}
Card Poker::operator[](const int &id) const{
    if(id >= cntCards()) return (Card){'N', '3'};
    return cards[id];
}
PokerTool::PokerTool() :
    funcSet{&PokerTool::single,
            &PokerTool::rocket,
            &PokerTool::pair,
            &PokerTool::bomb,
            &PokerTool::triplet,
            &PokerTool::sequence,
            &PokerTool::sequenceOfPairs,
            &PokerTool::tripletWithSingle,
            &PokerTool::tripletWithPair,
            &PokerTool::sequenceOfTriplets,
            &PokerTool::sequenceOfTripletsWithSingle,
            &PokerTool::sequenceOfTripletsWithPair,
            &PokerTool::quadplexSet
            }
{
    char s[] = "SCHD";
    for(int i = 0; i < 4; ++i)
        for(int j = 3; j <= 15; ++j)
            allcards.push_back((Card){s[i], (char)(j + '0')});
    allcards.push_back((Card){'J', '1'});
    allcards.push_back((Card){'J', '2'});
}
void PokerTool::deal(){
    auto t = allcards;
    for(int i = 0; i < 4; ++i) pokers[i].clear();
    std::random_shuffle(t.begin(), t.end());
    //std::sort(t.begin(), t.end());
    auto func = [&t](int x, Poker &a){
        for(int i = 0; i < x; ++i){
            a.addCard(t.back());
            t.pop_back();
        }
    };
    for(int i = 0; i < 3; ++i) func(17, pokers[i]);
    func(3, pokers[3]);
}
Poker PokerTool::getPoker(int id){ return pokers[id]; }
Poker PokerTool::read(const QByteArray &A){
    Poker data;
    bool d = 0; char t;
    for(auto i : A){
        if(d == 0) t = i;
        else {
            data.addCard((Card){t, i});
        }
        d ^= 1;
    }
    return data;
}
std::vector<Poker> PokerTool::sortPoker(const Poker &t) const{
    std::vector<Poker> list;
    if(t.cntCards() == 0) return list;
    Poker now; now.addCard(t[0]);
    for(int len = t.cntCards(), i = 1; i < len; ++i){
        if(t[i] != t[i - 1]){
            list.push_back(now);
            now.clear();
        }
        now.addCard(t[i]);
    }
    list.push_back(now);
    return list;
}
bool PokerTool::canPlay(const Poker &a, const Poker &b) const{
    QString A = check(a), B = check(b);
    if(A == "F") return 0;
    if(B == "F") return 1;
    auto func = [](const QString &tmp, QString &l, Card &r){
        l = tmp.mid(0, 2);
        r = (Card){tmp.mid(2, 2)};
    };
    QString A1, B1;
    Card A2, B2;
    func(A, A1, A2); func(B, B1, B2);
    if(A1 == B1){
        if(a.cntCards() != b.cntCards() || A2 == B2) return 0;
        return B2 < A2;
    }
    if(A1 == "Ro" || B1 == "Ro") return A1 == "Ro";
    if(A1 == "Bo") return 1;
    return 0;
}
QString PokerTool::check(const Poker &t) const{
    if(t.cntCards() == 0) return "F";
    for(auto func : funcSet){
        QString msg = (this->*func)(t);
        if(msg != "F") return msg;
    }
    return "F";
}
QString PokerTool::single(const Poker &t) const{
    if(t.cntCards() != 1) return "F";
    else return "Si" + t[0]();
}
QString PokerTool::rocket(const Poker &t) const{
    if(t.cntCards() != 2) return "F";
    if(!t[0].isJoker() || !t[1].isJoker()) return "F";
    return "Ro" + t[0]();
}
QString PokerTool::pair(const Poker &t) const{
    if(t.cntCards() != 2) return "F";
    if(t[0] == t[1]) return "Pa" + t[0]();
    return "F";
}
QString PokerTool::bomb(const Poker &t) const{
    if(t.cntCards() != 4) return "F";
    for(int i = 0; i < 3; ++i) if(t[i] != t[i + 1]) return "F";
    return "Bo" + t[0]();
}
QString PokerTool::triplet(const Poker &t) const{
    if(t.cntCards() != 3) return "F";
    for(int i = 0; i < 2; ++i) if(t[i] != t[i + 1]) return "F";
    return "Tr" + t[0]();
}
QString PokerTool::tripletWithSingle(const Poker &t) const{
    if(t.cntCards() != 4) return "F";
    auto list = sortPoker(t);
    if(list.size() != 2) return "F";
    for(auto i: list){
        if(i.cntCards() == 3){
            return "TS" + i[0]();
        }
    }
    return "F";
}
QString PokerTool::tripletWithPair(const Poker &t) const{
    if(t.cntCards() != 5) return "F";
    auto list = sortPoker(t);
    if(list.size() != 2) return "F";
    for(auto i: list){
        if(i.cntCards() == 3){
            return "TP" + i[0]();
        }
    }
    return "F";
}
QString PokerTool::sequence(const Poker &t) const{
    if(t.cntCards() < 5) return "F";
    for(int len = t.cntCards(), i = 0; i < len - 1; ++i)
        if(t[i] + 1 != t[i + 1]) return "F";
    return "Se" + t[0]();
}
QString PokerTool::sequenceOfPairs(const Poker &t) const{
    auto list = sortPoker(t);
    if(list.size() < 3) return "F";
    for(int len = list.size(), i = 0; i < len - 1; ++i){
        if(list[i].cntCards() != 2) return "F";
        if(list[i][0] + 1 != list[i + 1][0]) return "F";
    }
    if(list.back().cntCards() != 2) return "F";
    return "SP" + list[0][0]();
}
QString PokerTool::sequenceOfTriplets(const Poker &t) const{
    auto list = sortPoker(t);
    if(list.size() < 2) return "F";
    for(int len = list.size(), i = 0; i < len - 1; ++i){
        if(list[i].cntCards() != 3) return "F";
        if(list[i][0] + 1 != list[i + 1][0]) return "F";
    }
    return "ST" + list[0][0]();
}
QString PokerTool::sequenceOfTripletsWithSingle(const Poker &t) const{
    if(t.cntCards() % 4) return "F";
    int cnt = t.cntCards() / 4;
    auto list = sortPoker(t);
    for(int len = list.size(), i = 0; i < len; ++i){
        if(list[i].cntCards() < 3) continue;
        for(int j = 1; j + i < len; ++j){
            if(list[i + j].cntCards() < 3) break;
            if(list[i + j][0] != list[i + j - 1][0] + 1) break;
            if(j == cnt - 1) return "S1" + list[i][0]();
        }
    }
    return "F";
}
QString PokerTool::sequenceOfTripletsWithPair(const Poker &t) const{
    if(t.cntCards() % 5) return "F";
    auto list = sortPoker(t);
    std::sort(list.begin(), list.end());
    if(list[0].cntCards() == 1) return "F";
    for(int len = list.size(), i = 0; i < len; ++i){
        if(list[i].cntCards() == 3){
            int cnt = t.cntCards() / 5; --cnt;
            for(int j = 1; i + j < len; ++j){
                if(list[i + j].cntCards() != 3) break;
                if(list[i + j][0] != list[i + j - 1][0] + 1) break;
                cnt--;
            }
            if(cnt == 0) return "S2" + list[i][0]();
            return "F";
        }
    }
    return "F";
}
QString PokerTool::quadplexSet(const Poker &t) const{
    auto list = sortPoker(t);
    if(list.size() < 2 || list.size() > 3) return "F";
    std::sort(list.begin(), list.end());
    if(list.back().cntCards() != 4) return "F";
    QString name = list.back()[0]();
    if(list.size() == 2 && list[0].cntCards() == 2) return "QS" + name;
    if(list.size() == 2 && list[0].cntCards() == 4) return "QP" + name;
    if(list.size() == 3){
        if(list[0].cntCards() > 2 || list[0].cntCards() != list[1].cntCards()) return "F";
        return (list[0].cntCards() == 1 ? "QS" : "QP") + name;
    }
    return "F";
}

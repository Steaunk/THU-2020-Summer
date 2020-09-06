#include "snake.h"

int random(int a, int b){ return rand() % (b - a) + a; }
int random(int a){ return rand() % a; }

std::vector<int*> Snake::find(int d, int lim = 0){
    std::vector<int*> v;
    for(int i = lim; i < 40 - lim; ++i){
        for(int j = lim; j < 40 - lim; ++j){
            if(map[i][j] == d){
                v.push_back(&map[i][j]);
            }
        }
    }
    return v;
}
void Snake::find(int &x, int &y, int d){
    for(int i = 0; i < 40; ++i)
        for(int j = 0; j < 40; ++j)
            if(map[i][j] == d){
                x = i;
                y = j;
                return ;
            }
}

void Snake::genFood(){
    std::vector<int*> v = find(0);
    if(v.empty()) return ;
    std::random_shuffle(v.begin(), v.end());
    *(v[0]) = -1;
}
void Snake::initMap(){
    for(int i = 0; i < 40; ++i){
        for(int j = 0; j < 40; ++j)
            map[i][j] = 0;
    }
    length = 2; res = 0;
    cnt = 0; emit refreshTimer(0);
    std::vector<int*> v = find(0, 1);
    std::random_shuffle(v.begin(), v.end());
    auto t = *v.begin();
    std::vector<int*> tmp = {t - 1, t + 1, t - 40, t + 40};
    std::random_shuffle(tmp.begin(), tmp.end());
    *t = 2; *(tmp[0]) = 1;
}
void Snake::reverse(const int &x, const int &y){
    if(map[x][y] == 0) map[x][y] = -2;
    else if(map[x][y] == -2) map[x][y] = 0;
}
#include <QDebug>
void Snake::initGame(){
    genFood();
}
Snake::Snake(){}
Snake::~Snake(){}

void Snake::grow(const int &x, const int &y){
    if(res) --res, ++length;
    else{
        for(int i = 0; i < 40; ++i){
            for(int j = 0; j < 40; ++j)
                if(map[i][j] > 0)
                    map[i][j]--;
        }
    }
    map[x][y] = length;
}
void Snake::move(int d){
    int x, y;
    find(x, y, length);
    if(d == 0) --x;
    else if(d == 1) --y;
    else if(d == 2) ++x;
    else if(d == 3) ++y;
    else{
        int a, b; find(a, b, length - 1);
        x += x - a; y += y - b;
    }
    int tx, ty; find(tx, ty, length - 1);
    if(tx == x && ty == y) return ;
    if(x < 0 || x >= 40 || y < 0 || y >= 40 || map[x][y] > 0 || map[x][y] == -2){
        emit death();
        return ;
    }
    ++cnt;  emit refreshTimer(cnt);
    if(map[x][y] == -1){ res = 3; genFood(); }
    grow(x, y);
}
int Snake::operator () (const int &x, const int &y){
    return map[x][y];
}
int Snake::getLen(){ return length; }
void Snake::saveFile(QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) return ;
    QDataStream out(&file);
    out << length << res << cnt;
    for(int i = 0; i < 40; ++i)
        for(int j = 0; j < 40; ++j)
            out << map[i][j];
}
void Snake::readFile(QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDataStream in(&file);
    in >> length >> res >> cnt;
    for(int i = 0; i < 40; ++i)
        for(int j = 0; j < 40; ++j)
            in >> map[i][j];
    refreshTimer(cnt);
}

#include "painter.h"

Painter::Painter()
{

}
void Painter::background(QPainter *p){
    p->setPen(QColor("#2A3950"));
    p->setBrush(QColor("#2A3950"));
    p->drawRect(0, 0, 400, 400);
}

void Painter::game(QPainter *p, Snake *s){
    p->setPen(QColor("#2A3950"));
    if(s == nullptr) return ;
    for(int i = 0; i < 40; ++i){
        for(int j = 0; j < 40; ++j){
            const int &tmp = (*s)(i, j);
            if(tmp != 0){
                if(tmp == s->getLen()) p->setBrush(QColor("#A5DEF1"));
                else if(tmp == -1) p->setBrush(QColor("#EF4B4C"));
                else if(tmp == -2) p->setBrush(QColor("#FFFFFF"));
                else p->setBrush(QColor("#009394"));
                p->drawRoundedRect(i * base, j * base, base, base, 50, 50);
            }
        }
    }
}

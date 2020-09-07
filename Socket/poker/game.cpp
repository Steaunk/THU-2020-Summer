#include "game.h"

Game::Game(int player, QMainWindow *last, QWidget *parent) :
    QWidget(parent), last(last),
    ui(new Ui::Game)
{
    qDebug() << player_p << "Enter Game::Game";
    this->player_p = this->player = player;
    ui->setupUi(this);

    setWindowTitle("Player " + QString(player_p + 'A'));

    playlist = new QMediaPlaylist;

    playlist->addMedia(QUrl("qrc:/image/audio/BGM1.mp3"));
    playlist->addMedia(QUrl("qrc:/image/audio/BGM2.mp3"));
    playlist->addMedia(QUrl("qrc:/image/audio/BGM3.mp3"));
    playlist->setCurrentIndex(rand() % 3);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    mediaplayer = new QMediaPlayer;

    tool = new PokerTool;

    init();
    cnt = 1;

    ui->restartButton->setVisible(false);
    ui->quitButton->setVisible(false);
    //mediaplayer->setPlaylist(playlist);
    //player->play();
}
void Game::init(){
    ui->callButton->setVisible(false);
    ui->uncallButton->setVisible(false);
    ui->playButton->setVisible(false);
    ui->passButton->setVisible(false);

    ui->myStatus->setText("");
    ui->nextStatus->setText("");
    ui->preStatus->setText("");

    ui->myLabel->setText("");
    ui->nextLabel->setText("");
    ui->preLabel->setText("");

    ui->myCnt->setText("");
    ui->nextCnt->setText("");
    ui->preCnt->setText("");

    cnt = 0;
    showLandlord = false;
    cntCard[0] = cntCard[1] = 0;
    pass[0] = pass[1] = 0;
    isYourTurn = 0;
    landlord = -1;
    mycards.clear(); landlordcards.clear();
    lastcards.clear();

}
void Game::startGame(){
    qDebug() << player_p << "Enter Game::startGame";
    ++cnt;
    if(cnt < 3) return ;
    if(cnt == 3 && isVisible() == false){
        show();
        last->close();
    }
    ui->centerLabel->setText("");
    cntCard[0] = cntCard[1] = 17;

    ui->myLabel->setText("Player " + QString(player_p + 'A'));
    ui->nextLabel->setText("Player " + QString((player_p + 1) % 3 + 'A'));
    ui->preLabel->setText("Player " + QString((player_p + 2) % 3 + 'A'));

    mediaplayer->setPlaylist(playlist);
    mediaplayer->setVolume(100);
    mediaplayer->play();

    if(player_p == 0){
        tool->deal();
        mycards = tool->getPoker(0);
        qDebug() << player_p << ": deal poker";
        emit sendMsg(typeMsg(1) + tool->getPoker(1).write(), 0);
        emit sendMsg(typeMsg(1) + tool->getPoker(2).write(), 1);
        emit sendMsg(typeMsg(2) + tool->getPoker(3).write());
        landlordcards = tool->getPoker(3);
        int first = rand() % 3;
        ++first; emit sendMsg(typeMsg(3) + QByteArray::number(first % 3), 0);
        ++first; emit sendMsg(typeMsg(3) + QByteArray::number(first % 3), 1);
        ++first; setUser(first % 3);
    }
    update();
}
void Game::bidLandlord(){
    ui->callButton->setVisible(true);
    ui->uncallButton->setVisible(true);
}
void Game::setUser(int id){
    player = id;
    if(player == 0) bidLandlord();
}
void Game::beginGame(){
    ui->myStatus->setText("");
    ui->preStatus->setText("");
    ui->nextStatus->setText("");
    showLandlord = 1;
    update();
    if(landlord == (player + 1) % 3) cntCard[0] += 3;
    if((landlord + 1) % 3 == player) cntCard[1] += 3;
    if(landlord == player){
        qDebug() << player_p << "I'm Landlord";
        mycards.addCards(landlordcards.Cards());
        newRound();

    }
}
void Game::endGame(int pl){
    qDebug() << "---------------- End Game -------------------";
    if(pl == player) Win();
    else if(landlord != pl && landlord != player) Win();
    else Lose();
    ui->restartButton->setVisible(true);
    ui->quitButton->setVisible(true);
    init();
}
void Game::Win(){
    ui->centerLabel->setText("WIN");
    mediaplayer->setMedia(QUrl("qrc:/image/audio/victory.mp3"));
    mediaplayer->play();
}
void Game::Lose(){
    ui->centerLabel->setText("LOSE");
    mediaplayer->setMedia(QUrl("qrc:/image/audio/failure.mp3"));
    mediaplayer->play();
}
void Game::newRoundClear(){
    pass[0] = pass[1] = 0;
    ui->nextStatus->setText("");
    ui->preStatus->setText("");
    ui->myStatus->setText("");
    update();
}
void Game::newRound(){
    emit sendMsg(typeMsg(7));
    newRoundClear();
    ui->passButton->setEnabled(false);
    playCards();
}
void Game::playCards(){
    isYourTurn = 1;
    ui->passButton->setVisible(true);
    ui->playButton->setVisible(true);
    ui->playButton->setEnabled(tool->canPlay(mycards.chosenCards(), lastcards));
    ui->myStatus->setText("");
}
QByteArray Game::typeMsg(int id){
    qDebug() << player << "typeMsg:" << id;
    QByteArray my = QByteArray::number(player) + "";
    if(id == 0) return my + QByteArray("S"); //准备完毕
    if(id == 1) return my + QByteArray("A"); //手牌
    if(id == 2) return my + QByteArray("0"); //地主牌
    if(id == 3) return my + QByteArray("T"); //指定玩家顺序
    if(id == 4) return my + QByteArray("Y"); //告诉其他玩家自己是否叫地主
    if(id == 5) return my + QByteArray("L"); //指定谁是地主
    if(id == 6) return my + QByteArray("P"); //出牌
    if(id == 7) return my + QByteArray("R"); //告诉其他玩家新一轮开始
    return my + QByteArray("F");
}
void Game::dealMsg(QByteArray msg){
    qDebug() << player_p << ":" << msg;

    int who = msg.at(0) - '0';
    if(who == (player + 1) % 3) who = 0;
    else who = 1;

    qDebug() << "WHO :" << who;

    auto arrayToPoker = [msg, this](Poker &t){
        qDebug() << "VIP : " << t.cntCards() << msg;
        for(int i = 2; i < msg.length(); i += 2){
            t.addCard((Card){msg[i], msg[i + 1]});
        }
        update();
    };
    if(msg[1] == 'S') startGame();
    if(msg[1] == 'A') arrayToPoker(mycards);
    if(msg[1] == '0') arrayToPoker(landlordcards);
    if(msg[1] == 'T') setUser((int)(msg[2] - '0'));
    if(msg[1] == 'Y'){
        (who ? ui->preStatus : ui->nextStatus)->setText(msg[2] == 'Y' ? "叫地主" : "不叫");
        if(who != 1) return ;
        qDebug() << player_p << who << (who ? ui->preStatus : ui->nextStatus);
        if(player == 0){
            if(ui->preStatus->text() == "叫地主") landlord = (player + 2) % 3;
            else if(ui->nextStatus->text() == "叫地主") landlord = (player + 1) % 3;
            else landlord = player;
            emit sendMsg(typeMsg(5) + QByteArray::number(landlord));
            beginGame();
        }
        else bidLandlord();
    }
    if(msg[1] == 'L'){
        landlord = (int)(msg[2] - '0');
        beginGame();
    }
    if(msg[1] == 'P'){
        if(msg.length() > 3){
            lastcards = tool->read(msg.mid(3));
            cntCard[who] -= lastcards.cntCards();
            update();
            pass[who] = 0;
            (who ? ui->preStatus : ui->nextStatus)->setText("");
            if(cntCard[who] == 0){
                endGame(msg.at(0) - '0');
                return ;
            }
        }
        else {
            pass[who] = 1;
            (who ? ui->preStatus : ui->nextStatus)->setText("不出");
        }
        if(who == 1){
            if(pass[0] && pass[1]){
                lastcards.clear();
                update();
                newRound();
            }
            else{
                ui->passButton->setEnabled(true);
                playCards();
            }
        }
    }
    if(msg[1] == 'R') newRoundClear();
}
void Game::on_callButton_clicked(){
    ui->callButton->setVisible(false);
    ui->uncallButton->setVisible(false);
    ui->myStatus->setText("叫地主");
    emit sendMsg(typeMsg(4) + "Y");
}
void Game::on_uncallButton_clicked(){
    ui->callButton->setVisible(false);
    ui->uncallButton->setVisible(false);
    ui->myStatus->setText("不叫");
    emit sendMsg(typeMsg(4) + "N");
}
void Game::on_playButton_clicked(){
    lastcards = mycards.chosenCards();
    mycards.removeCards(lastcards.Cards());
    emit sendMsg(typeMsg(6) + QByteArray::number(player) + lastcards.write());
    ui->playButton->setVisible(false);
    ui->passButton->setVisible(false);
    update();
    if(mycards.cntCards() == 0){
        endGame(player);
    }
}
void Game::on_passButton_clicked(){
    emit sendMsg(typeMsg(6) + QByteArray::number(player));
    ui->playButton->setVisible(false);
    ui->passButton->setVisible(false);
    ui->myStatus->setText("不出");
    update();
}
void Game::on_restartButton_clicked(){
    startGame();
    emit sendMsg(typeMsg(0));
    ui->restartButton->setVisible(false);
    ui->quitButton->setVisible(false);
}
void Game::on_quitButton_clicked(){
    exit(0);
}
void Game::paintEvent(QPaintEvent *){

    QPainter painter(this);

    if(mycards.cntCards() && cntCard[0] && cntCard[1]){
        ui->myCnt->setText("cnt : " + QString::number(mycards.cntCards()));
        ui->nextCnt->setText("cnt : " + QString::number(cntCard[0]));
        ui->preCnt->setText("cnt : " + QString::number(cntCard[1]));
    }
    QPixmap pix;
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.save();
    painter.translate(50, 600);
    int cnt = 0;
    for(auto card : mycards.Cards()){
        pix.load(":image/cards/" + card.name() + ".png");
        painter.drawPixmap(cnt, card.getChosen() ? -20 : 0, 211, 293, pix);
        cnt += 35;
    }
    painter.restore();

    painter.save();
    painter.translate(412, 20);
    painter.scale(.5, .5);
    cnt = 0;
    for(auto card : landlordcards.Cards()){
        if(showLandlord) pix.load(":image/cards/" + card.name() + ".png");
        else pix.load(":image/cards/PADDING.png");
        painter.drawPixmap(cnt, 0, 211, 293, pix);
        cnt += 211;
    }
    painter.restore();

    painter.save();
    painter.translate(70, 150 + 211 * 0.4);
    painter.scale(0.4, 0.4);
    cnt = 0;
    pix.load(":image/cards/PADDING.png");
    painter.rotate(-90);
    for(int i = 0; i < cntCard[0]; ++i){
        painter.drawPixmap(cnt, 0, 211, 293, pix);
        cnt -= 35;
    }
    painter.restore();

    painter.save();
    painter.translate(1140 - 70, 150);
    painter.scale(0.4, 0.4);
    cnt = 0;
    pix.load(":image/cards/PADDING.png");
    painter.rotate(90);
    for(int i = 0; i < cntCard[1]; ++i){
        painter.drawPixmap(cnt, 0, 211, 293, pix);
        cnt += 35;
    }
    painter.restore();

    painter.save();
    painter.translate(400, 250);
    painter.scale(.6, .6);
    cnt = 0;
    for(auto i : lastcards.Cards()){
        pix.load(":image/cards/" + i.name() + ".png");
        painter.drawPixmap(cnt, 0, 211, 293, pix);
        cnt += 35;
    }
    painter.restore();

    if(landlord != -1){
        pix.load(":image/image/landlord.png");
        if(landlord == player) painter.translate(1140 - 80 - 100, 770);
        else if(landlord == (player + 1) % 3) painter.translate(200, 30);
        else painter.translate(830, 30);
        painter.drawPixmap(0, 0, 100, 100, pix);
    }
}
void Game::mousePressEvent(QMouseEvent *ev){
    if(isYourTurn == false) return ;
    if(ev->button() == Qt::LeftButton){
        QPoint pos = mapFromGlobal(ev->globalPos());
        int right = (mycards.cntCards() - 1) * 35 + 211 + 50;
        if(pos.x() < 50 || pos.x() > right) return ;
        int cnt;
        if(pos.x() >= right - 211) cnt = mycards.cntCards() - 1;
        else cnt = (pos.x() - 50) / 35;
        int eps = mycards.getChosen(cnt) ? -20 : 0;
        if(pos.y() < 600 + eps || pos.y() > 600 + 293 + eps) return ;
        mycards.revCard(cnt);
        qDebug() << mycards.chosenCards();
        ui->playButton->setEnabled(tool->canPlay(mycards.chosenCards(), lastcards.Cards()));
        update();

    }
}
Game::~Game()
{
    delete ui;
    delete tool;
    delete playlist;
}

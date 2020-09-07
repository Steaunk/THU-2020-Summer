#include "server.h"

MyTcp::MyTcp(Server *parent): parent(parent){}
void MyTcp::startClient(QString ip, int port, QString playername){
    s = new QTcpSocket;
    QObject::connect(s, &QTcpSocket::connected, parent, &Server::countDown);
     do{
        parent->inputIP(ip, port, playername);
        s->connectToHost(QHostAddress(ip), port);
    }while(s->waitForConnected() == false && (QMessageBox::about(nullptr, "Error", "Cannot connect to player " + playername + " now. Please try again."), 1));
    QObject::connect(s, SIGNAL(readyRead()), this, SLOT(recvMsg()));
}
void MyTcp::startServer(int port){
    l = new QTcpServer;
    l->listen(QHostAddress::Any, port);
    QObject::connect(l,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}
void MyTcp::recvMsg(){
    QByteArray msg = s->readAll();
    qDebug() << "RECV :" << msg;
    auto msgs = msg.split('$');
    for(auto i : msgs) if(i != "")
        emit parent->dealMsg(i);
}
void MyTcp::acceptConnection(){
    s = l->nextPendingConnection();
    QObject::connect(s, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    parent->countDown();
    l->close();
}
void MyTcp::write(QByteArray msg){
    s->write(msg);
}
Server::Server(bool testmode): testMode(testmode){
    s1 = new MyTcp(this);
    s2 = new MyTcp(this);
}
Server::~Server(){ delete s1; delete s2; }
void Server::inputIP(QString &ip, int &port, QString msg = ""){
    if(testMode) return ;
    qDebug() << msg;
    if(msg == "") msg = "Input IP Address";
    else msg = "Host Address of " + msg;
    qDebug() << msg;
    QString str = "127.0.0.1:" + QString::number(port);
    QRegExp re(R"(^((?:1?\d\d?|2[0-4]\d|25[0-5])[.](?:1?\d\d?|2[0-4]\d|25[0-5])[.](?:1?\d\d?|2[0-4]\d|25[0-5])[.](?:1?\d\d?|2[0-4]\d|25[0-5])):([1-5]?\d{1,4}|6[0-4]\d\d\d|65[0-4]\d\d|655[0-2]\d|6553[0-6])$)");
    while(1){
        str = QInputDialog::getText(nullptr, msg, tr("IP:"), QLineEdit::Normal, str);
        if(re.indexIn(str) == -1) continue;
        QStringList strlist = re.capturedTexts();
        ip = strlist[1]; port = strlist[2].toInt();
        break;
    }
}
int Server::inputPort(int def = 8888, QString msg = ""){
    if(testMode) return def;
    if(msg == "") msg = "Input Port";
    else msg = "Port for " + msg;
    return QInputDialog::getInt(nullptr, msg, tr("Port:"), def, 0, 65535);
}
void Server::startConnection(int player){
    if(player == 0){
        s1->startServer(inputPort(8888, "B"));
        s2->startServer(inputPort(7777, "C"));
    }
    else if(player == 1){
        s1->startServer(inputPort(9999, "C"));
        s2->startClient("127.0.0.1", 8888, "A");
    }
    else{
        s1->startClient("127.0.0.1", 7777, "A");
        s2->startClient("127.0.0.1", 9999, "B");
    }
}
void Server::countDown(){
    ++cnt;
    if(cnt == 2) emit ready();
}
void Server::sendMsg(const QByteArray &msg, int id){
    qDebug() << "SEND :" << msg;
    if(id != 1) s1->write(msg + "$");
    if(id != 0) s2->write(msg + "$");
}

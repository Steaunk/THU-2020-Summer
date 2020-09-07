#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(int argc, char *argv[], QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->chooseLabel->setStyleSheet("background:#E9E9EB;border:2px solid #43506C;");
    setWindowTitle("Connection");
    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->AButton, 0);
    btnGroup->addButton(ui->BButton, 1);
    btnGroup->addButton(ui->CButton, 2);
    playerConnect = new Server(argc > 1 ? (bool)*argv[1] : 0);
    if(argc > 2){
        if(*argv[2] == 'A') ui->AButton->setChecked(true);
        if(*argv[2] == 'B') ui->BButton->setChecked(true);
        if(*argv[2] == 'C') ui->CButton->setChecked(true);
    }
    connect(this, SIGNAL(startConnection(int)), playerConnect, SLOT(startConnection(int)));
    connect(playerConnect, SIGNAL(ready()), this, SLOT(ready()));
    if(argc > 1 && *argv[1] == '1') on_connectButton_clicked();
}
void MainWindow::on_connectButton_clicked(){
    if(btnGroup->checkedId() != -1){
        qDebug() << player << "click Button";
        game = new Game(player = btnGroup->checkedId(), this);
        connect(game, SIGNAL(sendMsg(QByteArray, int)), playerConnect, SLOT(sendMsg(QByteArray, int)));
        connect(game, SIGNAL(sendMsg(QByteArray)), playerConnect, SLOT(sendMsg(QByteArray)));
        connect(playerConnect, SIGNAL(dealMsg(QByteArray)), game, SLOT(dealMsg(QByteArray)));
        emit startConnection(player);
        ui->connectButton->setEnabled(false);
    }
}
void MainWindow::ready(){
    qDebug() << player << "Enter MainWindow::ready";
    emit game->sendMsg(game->typeMsg(0));
    qDebug() << player << "Exit MainWindow::ready";
}
MainWindow::~MainWindow()
{
    delete ui;
    delete btnGroup;
    delete playerConnect;
    if(game != nullptr) delete game;
}


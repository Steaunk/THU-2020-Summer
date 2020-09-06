#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    painter = new Painter();
    snake = new Snake();
    status = 0;

    setWindowTitle("Snake");
    setWindowIcon(QIcon(":/image/image/snake.ico"));
    startTimer(100);
    ui->timeCounter->display("HELLO");

    setFocusPolicy(Qt::StrongFocus);
    readyStatus();

    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT (load()));
    connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT (play()));
    connect(ui->actionPause, SIGNAL(triggered()), this, SLOT (pause()));
    connect(ui->actionRestart, SIGNAL(triggered()), this, SLOT (restart()));
    connect(ui->actionResume, SIGNAL(triggered()), this, SLOT (resume()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT (savefile()));

    connect(snake, SIGNAL(death()), this, SLOT(endStatus()));
    connect(snake, SIGNAL(refreshTimer(int)), ui->timeCounter, SLOT(display(int)));

    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT (load()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT (play()));
    connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT (pause()));
    connect(ui->restartButton, SIGNAL(clicked()), this, SLOT (restart()));
    connect(ui->resumeButton, SIGNAL(clicked()), this, SLOT (resume()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT (savefile()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete painter;
    delete snake;
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter *p = new QPainter(ui->snakeMap);
    painter->background(p);
    painter->game(p, snake);
}

void MainWindow::mousePressEvent(QMouseEvent *ev){
    if(status != 0) return ;
    if(ev->button() == Qt::LeftButton){
        QPoint pos = ui->snakeMap->mapFromGlobal(ev->globalPos());
        if(pos.x() < 0 || pos.y() < 0) return ;
        pos = QPoint(pos.x() / Painter::base, pos.y() / Painter::base);
        if(pos.x() < 40 && pos.y() < 40){
            snake->reverse(pos.x(), pos.y());
            update();
        }
    }
}
void MainWindow::keyPressEvent(QKeyEvent *ev){
    if(status != 1) return ;
    if (ev->key() == Qt::Key_Up){
        snake->move(1);
    }
    else if (ev->key() == Qt::Key_Down){
        snake->move(3);
    }
    else if (ev->key() == Qt::Key_Left){
        snake->move(0);
    }
    else if (ev->key() == Qt::Key_Right){
        snake->move(2);
    }
    update();
}
void MainWindow::timerEvent(QTimerEvent *ev){
    if(status == 1){
        snake->move(-1);
    }
}
void MainWindow::readyStatus(){
    snake->initMap();
    update();
    status = 0;

    ui->actionPlay->setEnabled(true);
    ui->actionPlay->setVisible(true);
    ui->actionResume->setEnabled(false);
    ui->actionResume->setVisible(false);
    ui->actionLoad->setEnabled(true);
    ui->actionSave->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionRestart->setEnabled(false);

    ui->loadButton->show();
    ui->pauseButton->hide();
    ui->playButton->show();
    ui->resumeButton->hide();
    ui->restartButton->hide();
    ui->saveButton->hide();
}
void MainWindow::gameStatus(){
    status = 1;

    ui->actionPlay->setEnabled(false);
    ui->actionPlay->setVisible(false);
    ui->actionResume->setEnabled(true);
    ui->actionResume->setVisible(true);
    ui->actionLoad->setEnabled(false);
    ui->actionSave->setEnabled(true);
    ui->actionPause->setEnabled(true);
    ui->actionRestart->setEnabled(false);

    ui->loadButton->hide();
    ui->pauseButton->show();
    ui->playButton->hide();
    ui->resumeButton->hide();
    ui->restartButton->hide();
    ui->saveButton->hide();
}
void MainWindow::pauseStatus(){
    status = 2;

    ui->actionPlay->setEnabled(false);
    ui->actionPlay->setVisible(false);
    ui->actionResume->setEnabled(true);
    ui->actionResume->setVisible(true);
    ui->actionLoad->setEnabled(false);
    ui->actionSave->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionRestart->setEnabled(true);

    ui->loadButton->hide();
    ui->pauseButton->hide();
    ui->playButton->hide();
    ui->resumeButton->show();
    ui->restartButton->show();
    ui->saveButton->show();
}
void MainWindow::endStatus(){
    status = 3;

    ui->actionPlay->setEnabled(false);
    ui->actionPlay->setVisible(true);
    ui->actionResume->setEnabled(false);
    ui->actionResume->setVisible(false);
    ui->actionLoad->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionRestart->setEnabled(true);

    ui->loadButton->hide();
    ui->pauseButton->hide();
    ui->playButton->hide();
    ui->resumeButton->hide();
    ui->restartButton->show();
    ui->saveButton->hide();
}
void MainWindow::newGame(){
    snake->initGame();
    update();
}

void MainWindow::pause(){
    pauseStatus();
}
void MainWindow::play(){
    newGame();
    gameStatus();
}
void MainWindow::load(){
    QString fileName = QFileDialog::getOpenFileName();
    snake->readFile(fileName);
    update();
    pauseStatus();
}
void MainWindow::restart(){
    readyStatus();
}
void MainWindow::resume(){
    gameStatus();
}
void MainWindow::savefile(){
    QString fileName = QFileDialog::getSaveFileName();
    snake->saveFile(fileName);
}

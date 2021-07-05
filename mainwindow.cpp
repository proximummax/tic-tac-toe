#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <math.h>
#include "mainwindow.h"

const int kBoardMargin = 30; // ширина окна
const int kBlockSize = 40; //размер квадрата
const int kPosDelta = 53;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(kBoardMargin * 2  + kBlockSize * kBoardSizeNum, kBoardMargin*2  + kBlockSize * kBoardSizeNum);//размер доски
    setMouseTracking(true);
    //меню
    QMenu *gameMenu = menuBar()->addMenu(tr("Game"));
    QAction *actionPVE = new QAction("Play with AI", this);
    connect(actionPVE, SIGNAL(triggered()), this, SLOT(initPVEGame()));
    gameMenu->addAction(actionPVE);
    QAction *actionPVP = new QAction ("Play with your friend",this);
    connect(actionPVP,SIGNAL(triggered()),this,SLOT(initPVPGame()));
    gameMenu->addAction(actionPVP);
    QAction *actionInfo = new QAction("Info",this);
    connect (actionInfo,SIGNAL(triggered()),this,SLOT(initInfo()));
    gameMenu->addAction(actionInfo);
    QAction *actionExit = new QAction("Exit",this);
    connect (actionExit,SIGNAL(triggered()),this,SLOT(initEx()));
    gameMenu->addAction((actionExit));

//начало игры
    initGame();
}

MainWindow::~MainWindow()
{
    if (game)
    {
        delete game;
        game = nullptr;
    }
}
void MainWindow::initEx(){
    exit(0);
}

void MainWindow::initGame()
{
    game = new GameModel;
    initPVEGame();

}
void MainWindow::initInfo(){
    QMessageBox::StandardButton inf = QMessageBox::information(this,"info","The players take turns putting signs on the free cells of the 15x15 field (one is always crosses, the other is always zeroes). The first player to line up his 5 pieces vertically, horizontally or diagonally wins. The first move is made by the player placing crosses.");
}
void MainWindow::initPVPGame(){
    game_type= PERSON;
    game->gameStatus=PLAYING;
    game->startGame(game_type);
    update();
}
void MainWindow::initPVEGame()
{
    game_type = BOT;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i <= kBoardSizeNum ; i++)         //игровое поле
    {
        painter.drawLine(kBoardMargin + kBlockSize *i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
        painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
    }
    QBrush brush;
       brush.setStyle(Qt::SolidPattern);
       if (clickPosRow > 0 && clickPosRow <= kBoardSizeNum &&
           clickPosCol > 0 && clickPosCol <= kBoardSizeNum &&
           game->gameMapVec[clickPosRow][clickPosCol] == 0)
       {
           painter.setBrush(brush);
         painter.drawRect(kBlockSize*clickPosCol-5,kBlockSize*clickPosRow-5,15*2,15*2);
       }
    for (int i = 0; i <= kBoardSizeNum; i++)
        for (int j = 0; j <= kBoardSizeNum; j++)
        {
            if (game->gameMapVec[i][j] == 1)
            {
                painter.drawImage(QRect(kBlockSize*j-5,kBlockSize*i-5,15*2,15*2),QImage(":/img/8b73bf5352b8e900df73b94705.jpg"));

            }
            else if (game->gameMapVec[i][j] == -1)
            {

                painter.drawImage(QRect(kBlockSize*j-5,kBlockSize*i-5,15*2,15*2),QImage(":/img/aa.png"));
            }
        }

//победа или поражение
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        (game->gameMapVec[clickPosRow][clickPosCol] == 1 ||
            game->gameMapVec[clickPosRow][clickPosCol] == -1))
    {
        if (game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        {
            game->gameStatus = WIN;
            QString str;
            if (game->gameMapVec[clickPosRow][clickPosCol] == 1)
                str = "cross player";
            else if (game->gameMapVec[clickPosRow][clickPosCol] == -1)
                str = "circle player";
            QMessageBox::StandardButton congr = QMessageBox::information(this, "congratulations", str + " win!");

            // сбрасываем состояние игры
            if (congr == QMessageBox::Ok)
            {
                game->startGame(game_type);
                game->gameStatus = PLAYING;
            }
        }
    }


    if (game->isDeadGame())
    {
        QMessageBox::StandardButton ops = QMessageBox::information(this, "oops", "dead game!");
        if (ops == QMessageBox::Ok)
        {
            game->startGame(game_type);
            game->gameStatus = PLAYING;
        }

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    //метка курсора
int x = event->x();
int y = event->y();

if (x > kBoardMargin + kBlockSize / 2 &&
            x < size().width() - kBoardMargin &&
            y > kBoardMargin + kBlockSize / 2 &&
            y < size().height()- kBoardMargin)
    {

      int col = (x/kBlockSize);
      int row = (y/kBlockSize);
      int leftTopPosX = kBoardMargin + kBlockSize * col;
      int leftTopPosY = kBoardMargin + kBlockSize * row;
       clickPosRow = -1;
       clickPosCol = -1;
       int len = 0;
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
      }

 }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (!(game_type == BOT && !game->playerFlag))
    {
        OneByPerson();
        if (game->gameType == BOT && !game->playerFlag)
        {
           OneByAI();
        }
    }

}

void MainWindow::OneByPerson()
{
    if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        game->actionByPerson(clickPosRow, clickPosCol);
        update();
    }
}

void MainWindow::OneByAI()
{
    game->actionByAI(clickPosRow, clickPosCol);
    update();
}


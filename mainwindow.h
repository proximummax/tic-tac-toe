#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameModel.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    GameModel *game;
    GameType game_type;
    int clickPosRow, clickPosCol;
    void initGame();
private slots:
    void OneByPerson();
    void OneByAI();
    void initInfo();
    void initPVPGame();
    void initPVEGame();
    void initEx();
};

#endif // MAINWINDOW_H

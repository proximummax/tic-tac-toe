#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <vector>
using namespace std;
enum GameType
{
    PERSON,
    BOT
};

enum GameStatus
{
    PLAYING,
    WIN,
    DEAD
};

//размер доски
const int kBoardSizeNum = 15;

class GameModel
{
public:
    GameModel();

public:
    vector<vector<int>> gameMapVec; // пусто 0,крестик 1,нолик -1
    vector<vector<int>> scoreMapVec; // счет
    bool playerFlag;
    GameType gameType;
    GameStatus gameStatus;

    void startGame(GameType type);
    void calculateScore();
    void actionByPerson(int row, int col);
    void actionByAI(int &clickRow, int &clickCol);
    void updateGameMap(int row, int col);
    bool isWin(int row, int col);
    bool isDeadGame();
};

#endif // GAMEMODEL_H

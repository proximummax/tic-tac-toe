#include <time.h>
#include "GameModel.h"
GameModel::GameModel()
{

}


void GameModel::startGame(GameType type)
{
    gameType = type;
    gameMapVec.clear();
    for (int i = 0; i <= kBoardSizeNum; i++)         //заполняем игровое поле 0
    {
        vector<int> lineBoard;
        for (int j = 0; j <= kBoardSizeNum; j++)
            lineBoard.push_back(0);
        gameMapVec.push_back(lineBoard);
    }

    if (gameType == BOT)                //инициализируем массив очков
    {
        scoreMapVec.clear();
        for (int i = 0; i <= kBoardSizeNum; i++)
        {
            vector<int> lineScores;
            for (int j = 0; j <= kBoardSizeNum; j++)
                lineScores.push_back(0);
            scoreMapVec.push_back(lineScores);
        }
    }

    playerFlag = true;


}

void GameModel::updateGameMap(int row, int col)     //смена хода
{
    if (playerFlag)
        gameMapVec[row][col] = 1;
    else
        gameMapVec[row][col] = -1;

    playerFlag = !playerFlag;
}

void GameModel::actionByPerson(int row, int col)
{
    updateGameMap(row, col);
}

void GameModel::actionByAI(int &clickRow, int &clickCol)
{
    //оцениваем ситуацию
    calculateScore();

    // находим позицию наибольшего результата и счета
    int maxScore = 0;
    vector<pair<int, int>> maxPoints;

    for (int row = 0; row <= kBoardSizeNum; row++)
        for (int col = 0; col <= kBoardSizeNum; col++)
        {
            if (gameMapVec[row][col] == 0)
            {
                if (scoreMapVec[row][col] > maxScore)          // выявляем наибольшее число очков и координату
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(make_pair(row, col));
                }
                else if (scoreMapVec[row][col] == maxScore)     //если наибольших несколько то сохраняем все
                    maxPoints.push_back(make_pair(row, col));
            }
        }
    //берем случайное из них
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();

    pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first; // записываем
    clickCol = pointPair.second;
    updateGameMap(clickRow, clickCol);
}

// подсчет очков
void GameModel::calculateScore()
{

    int personNum = 0; // человек
    int botNum = 0; // AI
    int emptyNum = 0;//пустота

    scoreMapVec.clear();
    for (int i = 0; i <= kBoardSizeNum; i++)
    {
        vector<int> lineScores;
        for (int j = 0; j <= kBoardSizeNum; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);
    }

    // полный обход и подсчет очков
    for (int row = 0; row <= kBoardSizeNum; row++)
        for (int col = 0; col <= kBoardSizeNum; col++)
        {
            if (row > 0 && col > 0 &&
                gameMapVec[row][col] == 0)
            {
                // проходим 8 направлений вокруг
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;
                        if (!(y == 0 && x == 0))
                        {
                            // по 4 в каждом направлении (оцениваем игрока)
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 1)//занято игроком
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         gameMapVec[row + i * y][col + i * x] == 0) //пустое пространство
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            //выход за границу
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == 1) // занято игроком
                                {
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         gameMapVec[row - i * y][col - i * x] == 0) //пустое пространство
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            //выход
                                    break;
                            }

                            //защита
                            if (personNum == 1)                      // заблокировать 1
                                scoreMapVec[row][col] += 10;
                            else if (personNum == 2)                 // заблокировать 2
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 40;
                            }
                            else if (personNum == 3)                 // заблокировать 3
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 110;
                            }
                            else if (personNum == 4)                 //заблокировать 4
                                scoreMapVec[row][col] += 10100;

                            emptyNum = 0;

                            // оцениваем для AI
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 1)
                                {
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         gameMapVec[row +i * y][col + i * x] == 0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == -1)
                                {
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         gameMapVec[row - i * y][col - i * x] == 0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            if (botNum == 0)                      //атака
                                scoreMapVec[row][col] += 5;
                            else if (botNum == 1)
                                scoreMapVec[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 50;
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 100;
                            }
                            else if (botNum >= 4)
                                scoreMapVec[row][col] += 10000;

                        }
                    }

            }
        }
}

bool GameModel::isWin(int row, int col)
{
    // рассматриваем все 4 ситуации,когда могли победить
    // горизонтальное направление
    for (int i = 0; i < 5; i++)
    {
        if (col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 4])
            return true;
    }

    // вертикальное направление
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 4][col])
            return true;
    }

    // диагональ слева
    for (int i = 0; i < 5; i++)
    {
        if (row + i < kBoardSizeNum &&
            row + i - 4 > 0 &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 1][col - i + 1] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 2][col - i + 2] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 3][col - i + 3] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 4][col - i + 4])
            return true;
    }

    // диагональ справа
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 1][col - i + 1] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 2][col - i + 2] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 3][col - i + 3] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

bool GameModel::isDeadGame()
{

    for (int i = 1; i <= kBoardSizeNum; i++)
        for (int j = 1; j <= kBoardSizeNum; j++)
        {
            if (gameMapVec[i][j]==0)
                return false;
        }
    return true;
}


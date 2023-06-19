#pragma once

#include "MinefieldButton.h"
#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QTime>
#include <QTimer>

class MyMainWindow : public QMainWindow {
    Q_OBJECT

private:
    std::vector<std::vector<int>> minefieldData;
    std::vector<std::vector<MinefieldButton*>> minefield;
    std::vector<std::vector<bool>> visited;

    QWidget* minefieldWidget;
    QGridLayout* layout;
    QLabel* timerLabel;
    QTime timeElapsed;
    QTimer* timer;

    int lastFieldSize = 0;
    int fieldSize = 0;
    int mineCount = 0;
    int revealedTiles = 0;

    void startTimer();
    void stopTimer();
    void updateTimeElapsed();
    void updateTimerLabel();

public:
    MyMainWindow();

public slots:
    void newGame();

    void handleButtonClick(int row, int col);

    void floodFill(int row, int col);

    void setDifficulty(Difficulty difficulty);
};
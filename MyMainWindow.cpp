#include "MyMainWindow.h"
#include <QMenuBar>
#include <QApplication>
#include <QMessageBox>
#include <QRandomGenerator>
#include <set>
#include <QTimer>
#include <QHBoxLayout>

MyMainWindow::MyMainWindow() : timer(nullptr) {
    QMenuBar* menuBar = new QMenuBar;

    QMenu* gameMenu = new QMenu("Game", this);
    QAction* newGameAction = new QAction("New Game", this);
    QAction* exitAction = new QAction("Exit", this);

    QMenu* difficultyMenu = new QMenu("Difficulty", this);
    QAction* easyAction = new QAction("Easy", this);
    QAction* mediumAction = new QAction("Medium", this);

    connect(newGameAction, &QAction::triggered, this, &MyMainWindow::newGame);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(easyAction, &QAction::triggered, this, [this] { setDifficulty(Easy); });
    connect(mediumAction, &QAction::triggered, this, [this] { setDifficulty(Medium); });

    gameMenu->addAction(newGameAction);
    gameMenu->addAction(exitAction);

    difficultyMenu->addAction(easyAction);
    difficultyMenu->addAction(mediumAction);

    menuBar->addMenu(gameMenu);
    menuBar->addMenu(difficultyMenu);

    this->setMenuBar(menuBar);

    minefieldWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(minefieldWidget);

    QHBoxLayout* timerLayout = new QHBoxLayout;
    timerLabel = new QLabel("00:00:00");
    timerLayout->addStretch();
    timerLayout->addWidget(timerLabel);
    timerLayout->addStretch();
    mainLayout->addLayout(timerLayout);

    layout = new QGridLayout;
    mainLayout->addLayout(layout);

    setCentralWidget(minefieldWidget);

    setFixedSize(650, 680);
}

void MyMainWindow::newGame() {
    // Reset timer
    if (timer != nullptr) {
        delete timer;
        timer = nullptr;
    }
    timeElapsed = QTime(0, 0, 0);
    updateTimerLabel();

    revealedTiles = 0;

    // Resize each time when current size does not match last size
    if (fieldSize != lastFieldSize) {
        visited.resize(fieldSize);
        for (int i = 0; i < fieldSize; i++) {
            visited[i].resize(fieldSize);
        }
        minefield.resize(fieldSize);
        for (int i = 0; i < fieldSize; i++) {
            minefield[i].resize(fieldSize);
        }
        minefieldData.resize(fieldSize);
        for (int i = 0; i < fieldSize; i++) {
            minefieldData[i].resize(fieldSize);
        }
    }

    // Purge the last board layout
    if (!minefield.empty()) {
        for (int i = 0; i < fieldSize; i++) {
            for (int j = 0; j < fieldSize; j++) {
                if (minefield[i][j] != nullptr) {
                    layout->removeWidget(minefield[i][j]);
                    delete minefield[i][j];
                    minefield[i][j] = nullptr;
                }
            }
        }
    }
    minefieldWidget->update();
    std::fill(visited.begin(), visited.end(), std::vector<bool>(fieldSize, false));
    for (auto& row : minefieldData) {
        std::fill(row.begin(), row.end(), 0);
    }

    // For each button in the grid, click on it to link the response event to open the square
    for (int i = 0; i < fieldSize; i++) {
        for (int j = 0; j < fieldSize; j++) {
            minefield[i][j] = new MinefieldButton(i, j);
            minefield[i][j]->setStyleSheet("background-color: gray");
            minefield[i][j]->setFixedSize(600 / fieldSize, 600 / fieldSize);
            layout->addWidget(minefield[i][j], i + 1, j);
            if (!minefield[i][j]->isEnabled()) {
                minefield[i][j]->setEnabled(true);
            }
            connect(minefield[i][j], &MinefieldButton::buttonClicked, this, &MyMainWindow::handleButtonClick);
        }
    }

    // Place the mines randomly
    QRandomGenerator randomGenerator;
    std::set<std::pair<int, int>> mines;
    while (mines.size() < mineCount) {
        int x = randomGenerator.bounded(fieldSize);
        int y = randomGenerator.bounded(fieldSize);
        mines.insert({ x, y });
    }
    for (const auto& mine : mines) {
        minefieldData[mine.first][mine.second] = 1;
    }

    // Start the timer
    startTimer();
}

void MyMainWindow::handleButtonClick(int row, int col) {
    if (minefieldData[row][col] == 1) { // Fallo
        minefield[row][col]->setStyleSheet("background-color: red");
        QMessageBox::information(this, "Game Over", "You've hit a mine!");
        stopTimer();
        newGame();
    }
    else {
        floodFill(row, col);
        // Comproba la condici¨®n de la gana
        int remainingTiles = 0;
        if (revealedTiles == fieldSize * fieldSize - mineCount) {
            QMessageBox::information(this, "Congratulations", "You've won the game!");
            stopTimer();
            newGame();
        }
    }
}

void MyMainWindow::floodFill(int row, int col) {
    if (row < 0 || row >= fieldSize || col < 0 || col >= fieldSize)
        return; // Out of bounds

    if (!minefield[row][col]->text().isEmpty() || visited[row][col])
        return; // Already revealed or visited

    minefield[row][col]->setStyleSheet("background-color: white");
    visited[row][col] = true;
    revealedTiles++;

    int minesNearby = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = row + dx, ny = col + dy;
            if (nx >= 0 && nx < fieldSize && ny >= 0 && ny < fieldSize && minefieldData[nx][ny] == 1) {
                minesNearby++;
            }
        }
    }

    if (minesNearby == 0) {
        minefield[row][col]->setText("");
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                floodFill(row + dx, col + dy);
            }
        }
    }
    else {
        minefield[row][col]->setText(QString::number(minesNearby));
        minefield[row][col]->setStyleSheet("font-size: 24px");
    }
}

void MyMainWindow::setDifficulty(Difficulty difficulty) {
    lastFieldSize = fieldSize;
    switch (difficulty) {
    case Easy:
        fieldSize = 9;
        mineCount = 10;
        break;
    case Medium:
        fieldSize = 16;
        mineCount = 40;
        break;
    }
    if (!minefield.empty()) {
        for (int i = 0; i < lastFieldSize; i++) {
            for (int j = 0; j < lastFieldSize; j++) {
                if (minefield[i][j] != nullptr) {
                    layout->removeWidget(minefield[i][j]);
                    delete minefield[i][j];
                    minefield[i][j] = nullptr;
                }
            }
        }
    }
    newGame();
}

void MyMainWindow::startTimer() {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyMainWindow::updateTimeElapsed);
    timer->start(1000);  // Update every second
}

void MyMainWindow::stopTimer() {
    if (timer != nullptr) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
}

void MyMainWindow::updateTimeElapsed() {
    timeElapsed = timeElapsed.addSecs(1);
    updateTimerLabel();
}

void MyMainWindow::updateTimerLabel() {
    QString timeText = timeElapsed.toString("hh:mm:ss");
    timerLabel->setText(timeText);
}

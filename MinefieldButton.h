#pragma once

#include <QPushButton>
#include <QWidget>


enum Difficulty { Easy, Medium };

class MinefieldButton : public QPushButton {
    Q_OBJECT

public:
    MinefieldButton(int row, int col, QWidget* parent = 0);

    void handleClick();

signals:
    void buttonClicked(int row, int col);

private:
    int row;
    int col;
};
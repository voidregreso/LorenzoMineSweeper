#include "MinefieldButton.h"

MinefieldButton::MinefieldButton(int row, int col, QWidget* parent) : QPushButton(parent), row(row), col(col) {
    connect(this, &QPushButton::clicked, this, &MinefieldButton::handleClick);
}

void MinefieldButton::handleClick() {
    // Emit a signal that this button has been clicked
    emit buttonClicked(row, col);
}
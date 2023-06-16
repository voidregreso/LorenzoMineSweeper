#include "MyWizardDialog.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QCommandLinkButton>
#include <QLabel>
#include <QMessageBox>
#include <QFont>
#include <QPalette>

MyWizardDialog::MyWizardDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Dialogue");

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* messageLabel = new QLabel("Choose a difficulty to start game", this);
    QFont font("Segoe UI", 13);
    messageLabel->setFont(font);
    QPalette palette = messageLabel->palette();
    palette.setColor(QPalette::WindowText, Qt::darkBlue);
    messageLabel->setPalette(palette);

    layout->addWidget(messageLabel);

    QCommandLinkButton* level1Button = new QCommandLinkButton("Level Easy", this);
    connect(level1Button, &QCommandLinkButton::clicked, this, [this]() {
        emit difficultySelected(Difficulty::Easy); 
        this->close();
        });
    layout->addWidget(level1Button);

    QCommandLinkButton* level2Button = new QCommandLinkButton("Level Medium", this);
    connect(level2Button, &QCommandLinkButton::clicked, this, [this]() {
        emit difficultySelected(Difficulty::Medium); 
        this->close();
        });
    layout->addWidget(level2Button);

    setLayout(layout);
}
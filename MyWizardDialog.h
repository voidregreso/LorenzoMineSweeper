#pragma once

#include "MinefieldButton.h"
#include <QDialog>
#include <QCloseEvent>

class MyWizardDialog : public QDialog {
    Q_OBJECT
public:
    MyWizardDialog(QWidget* parent = nullptr);
signals:
    void difficultySelected(Difficulty level);
};
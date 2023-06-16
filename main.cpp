#include "MyMainWindow.h"
#include "MyWizardDialog.h"
#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MyMainWindow mw;
    // Wizard
    MyWizardDialog dialog;
    QObject::connect(&dialog, &MyWizardDialog::difficultySelected, &mw, &MyMainWindow::setDifficulty);
    mw.show();
    dialog.exec();
    return app.exec();
}
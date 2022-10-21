#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QSplashScreen>
#include <QTimer>
#include <QPixmap>
#include <unistd.h>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QSplashScreen *splash = new QSplashScreen();
    splash->setPixmap(QPixmap("/home/anurag/Workspace/QtSpace/CompressHub/imgs/dtu.png")); // splash picture
    splash->setWindowFlag(Qt::WindowStaysOnTopHint);
    splash->show();

    MainWindow textwin;
//    splash->close();

//    QTimer::singleShot(2000, splash,SLOT(close()));
//    QTimer::singleShot(2500, &textwin, SLOT(show()));
    QTimer::singleShot(700, &textwin, SLOT(raise()));
    QTimer::singleShot(700, splash, SLOT(close()));
//    textwin.setFocus();
    textwin.showNormal();
//    textwin.raise();
    textwin.activateWindow();
    textwin.move(QApplication::desktop()->screen()->rect().center() - textwin.rect().center());

    textwin.show();

    return app.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QFont>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setFont(QFont("Times", 13, QFont::Bold));
    QPixmap pixmap("../Qt_MutiEdit/setup.jpg");
    QSplashScreen splash(pixmap);
    splash.showMessage("Welcome to MutiEdit",Qt::AlignCenter,Qt::red);
    splash.show();
    a.processEvents();
    MainWindow w;
    w.show();
    splash.finish(&w);

    return a.exec();
}

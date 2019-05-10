#include "mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QFont>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setFont(QFont("Times", 13, QFont::Bold));
    QPixmap pixmap(":/new/pic/Icon/setup");
    QSplashScreen splash(pixmap);
    splash.showMessage("Welcome to MutiEdit",Qt::AlignCenter,Qt::red);
    splash.show();
    a.processEvents();
    MainWindow w;
    w.setWindowTitle("MultiEdit");
    w.setWindowIcon(QIcon(":/new/pic/Icon/book.jpg"));
    w.show();
    splash.finish(&w);

    return a.exec();
}

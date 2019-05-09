#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <finddialog.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void doProcessActionSlot();
    void doProcessResponseFindSlot(QString findstr, bool isall, int findnum);
    void doProcessResponseFindRepSlot(QString findstr, QString repstr, int type, bool isall);
protected:
    void closeEvent(QCloseEvent *);
private:
    void Init();
private:
    Ui::MainWindow *ui;
    findDialog *findlog;
};

#endif // MAINWINDOW_H

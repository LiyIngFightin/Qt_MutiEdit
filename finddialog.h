#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class findDialog;
}

class findDialog : public QDialog
{
    Q_OBJECT

public:
    explicit findDialog(QWidget *parent = nullptr);
    ~findDialog();
private slots:
    void doProcessPushButtonSlot();
    void doProcessRadioButtonSlot(bool ret);
    void doProcessComboBoxSlot(int index);
signals:
    void SignalFindtoMain(QString findstr,bool isall,int num);
    void SignalFindReptoMain(QString findstr,QString repstr,int type,bool isall);
private:
    Ui::findDialog *ui;
    bool isFind;
    bool isAll;
    int findnumber;
    int repnumber;
};

#endif // FINDDIALOG_H

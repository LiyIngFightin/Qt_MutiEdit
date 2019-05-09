#include "finddialog.h"
#include "ui_finddialog.h"
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QMessageBox>
#include <QtDebug>
findDialog::findDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findDialog)
{
    ui->setupUi(this);
    isFind=true;
    isAll=true;
    findnumber=0;
    repnumber=0;
    connect(ui->allbtn,SIGNAL(clicked()),
            this,SLOT(doProcessPushButtonSlot()));
    connect(ui->prebtn,SIGNAL(clicked()),
            this,SLOT(doProcessPushButtonSlot()));
    connect(ui->nextbtn,SIGNAL(clicked()),
            this,SLOT(doProcessPushButtonSlot()));
    connect(ui->singlebtn,SIGNAL(clicked()),
            this,SLOT(doProcessPushButtonSlot()));
    connect(ui->findradioButton,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRadioButtonSlot(bool)));
    connect(ui->repradioButton,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRadioButtonSlot(bool)));
    connect(ui->comboBox,SIGNAL(activated(int)),
            this,SLOT(doProcessComboBoxSlot(int)));

}

findDialog::~findDialog()
{
    delete ui;
}

void findDialog::doProcessPushButtonSlot()
{
    QPushButton *btn=qobject_cast<QPushButton *>(this->sender());
    QString findstr=ui->findlineEdit->text();
    QString repstr=ui->replineEdit->text();
    if(findstr.isEmpty()) {
        QMessageBox::warning(this,"警告：",
                             "查找数据为空！",QMessageBox::Ok);
        return;
    }
    if(btn == ui->nextbtn) {
        findnumber++;
        if(isFind==false) {
            QMessageBox::warning(this,"警告：",
                                 "勾选查找框！",QMessageBox::Ok);
            return;
        }
        emit SignalFindtoMain(findstr,isAll,findnumber);
    }
    else if(btn == ui->prebtn) {
        if(findnumber==0) {
            QMessageBox::warning(this,"警告：",
                                 "这是最开始！",QMessageBox::Ok);
            return;
        }
        findnumber--;
        if(isFind==false) {
            QMessageBox::warning(this,"警告：",
                                 "勾选查找框！",QMessageBox::Ok);
            return;
        }
        emit SignalFindtoMain(findstr,isAll,findnumber);
    }
    else if(btn == ui->singlebtn) {
        repnumber++;
        if(isFind==true) {
            QMessageBox::warning(this,"警告：",
                                 "勾选替换框！",QMessageBox::Ok);
            return;
        }
        if(repstr.isEmpty()) {
            QMessageBox::warning(this,"警告：",
                                 "替换数据为空！",QMessageBox::Ok);
            return;
        }
        emit SignalFindReptoMain(findstr,repstr,0,isAll);
    }
    else if(btn == ui->allbtn) {
        if(isFind==true) {
            QMessageBox::warning(this,"警告：",
                                 "勾选替换框！",QMessageBox::Ok);
            return;
        }
        if(repstr.isEmpty()) {
            QMessageBox::warning(this,"警告：",
                                 "替换数据为空！",QMessageBox::Ok);
            return;
        }
        emit SignalFindReptoMain(findstr,repstr,1,isAll);
    }
}

void findDialog::doProcessRadioButtonSlot(bool ret)
{
    QRadioButton *btn=qobject_cast<QRadioButton *>(this->sender());
    qDebug()<<"radiobutton";
    if(btn == ui->findradioButton) {
          isFind=ret;
    }
    else if(btn == ui->repradioButton) {
           isFind=!ret;
    }
}

void findDialog::doProcessComboBoxSlot(int index)
{
    if(index == 0){
        isAll=true;
    }
    else if (index == 1) {
        isAll=false;
    }
}

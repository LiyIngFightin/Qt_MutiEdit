#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myedit.h"
#include <QtDebug>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QDir>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doProcessActionSlot()
{
    QAction *act=qobject_cast<QAction *>(this->sender());
    if(act == ui->newAction)
    {
        myEdit *edit=new myEdit(this);
        ui->mdiArea->addSubWindow(edit);
        edit->newFile();
        edit->show();
    }
    else if(act == ui->openAction) {
        QStringList filenames=QFileDialog::getOpenFileNames(this,"打开文件",
                                                      QDir::currentPath(),
                                                      tr("Text files (*.txt)"));
        if(filenames.isEmpty()) {
            QMessageBox::warning(this,"警告","文件名无效",QMessageBox::Ok);
            return;
        }
        for (int var = 0; var < filenames.length(); ++var) {
            QString filepath=QFileInfo(filenames[var]).canonicalFilePath();
            foreach(QMdiSubWindow *window,ui->mdiArea->subWindowList()){
                myEdit *edit=qobject_cast<myEdit *>(window->widget());
                if(edit->getCurrentFileName() == filepath)
                {
                    ui->mdiArea->setActiveSubWindow(window);
                    continue;
                }
            }
            myEdit *newedit=new myEdit;
            ui->mdiArea->addSubWindow(newedit);
            if(newedit->openFile(filenames[var]))
            {
                ui->statusBar->showMessage("打开"+QFileInfo(filenames[var]).fileName()+"文件成功",2000);
                newedit->show();
            }
            else {
                newedit->close();
                ui->statusBar->showMessage("打开"+QFileInfo(filenames[var]).fileName()+"文件失败",2000);
            }
        }

    }
    else if(act == ui->saveAction){
        myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
        if (edit && edit->saveFile())
        {
           ui->statusBar->showMessage("保存文件成功",2000);
        }
        else
        {
            ui->statusBar->showMessage("保存文件失败",2000);
        }
    }
    else if(act == ui->saveasAction){
        myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
        if (edit && edit->saveasFile() )
            ui->statusBar->showMessage("另存为文件成功",2000);
        else{
             ui->statusBar->showMessage("另存为文件失败",2000);
        }
    }
    else if(act == ui->exitAction)
    {
        this->close();
    }
    else if(act == ui->undoAction){
        myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
        edit->undo();
    }
    else if(act == ui->redoAction){
        myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
        edit->redo();
    }
    else if(act == ui->cutAction){
        myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
        edit->cut();
    }
    else if(act == ui->copyAction){
        myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
        edit->copy();
    }
    else if(act == ui->pasteAction){
        myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
        edit->paste();
    }
    else if(act == ui->closeAction){
        ui->mdiArea->closeActiveSubWindow();
    }
    else if(act == ui->closeallAction){
        ui->mdiArea->closeAllSubWindows();
    }
    else if(act == ui->pingAction){
        ui->mdiArea->tileSubWindows();
    }
    else if(act == ui->cenAction){
        ui->mdiArea->cascadeSubWindows();
    }
    else if(act == ui->findAction){
        findlog=new findDialog;
        findlog->show();
        connect(findlog,&findDialog::SignalFindtoMain,
                this,&MainWindow::doProcessResponseFindSlot);
        connect(findlog,&findDialog::SignalFindReptoMain,
                this,&MainWindow::doProcessResponseFindRepSlot);

    }
    else if(act == ui->nextAction){
        ui->mdiArea->activateNextSubWindow();
    }
    else if(act == ui->preAction){
        ui->mdiArea->activatePreviousSubWindow();
    }
    else if(act == ui->aboutAction){
        QWidget *help=new QWidget;
        help->set
    }
    else if(act == ui->qtAction){

    }
}

void MainWindow::doProcessResponseFindSlot(QString findstr,bool isall,int findnum)
{
    QTextDocument::FindFlag findflag=QTextDocument::FindBackward;
    myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
    edit->moveCursor(QTextCursor::Start);
    if(isall == true) {
        findflag=QTextDocument::FindWholeWords;
    }
    else {
        findflag=QTextDocument::FindCaseSensitively;
    }
    for (int i=0;i<findnum-1;i++) {
        if(!edit->find(findstr,findflag)) {
            QMessageBox::information(this,tr("注意"),
                                         tr("没有找到内容"),QMessageBox::Ok);
            return;
        }
    }
    if(edit->find(findstr,findflag)) {
        QPalette palette = edit->palette();
        palette.setColor(QPalette::Highlight,
                         palette.color(QPalette::Active,QPalette::Highlight));
        edit->setPalette(palette);
    }
    else {
         QMessageBox::information(this,tr("注意"),
                                  tr("没有找到内容"),QMessageBox::Ok);
    }
}


void MainWindow::doProcessResponseFindRepSlot(QString findstr, QString repstr, int type,bool isall)
{
    QTextDocument::FindFlag findflag=QTextDocument::FindBackward;
    myEdit *edit =qobject_cast<myEdit *>(ui->mdiArea->activeSubWindow()->widget());
    edit->moveCursor(QTextCursor::Start);
    if(isall == true) {
        findflag=QTextDocument::FindWholeWords;
    }
    else {
        findflag=QTextDocument::FindCaseSensitively;
    }
    while(1) {
        if(edit->find(findstr,findflag)) {
            QPalette palette = edit->palette();
            palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,QPalette::Highlight));
            edit->setPalette(palette);
            if(type == 0) {
                int ret=QMessageBox::information(this,tr("注意"),tr("确定替换此数据？"),QMessageBox::Ok,QMessageBox::Cancel);
                if(ret != QMessageBox::Ok) {
                    continue;
                }
            }
            QTextCursor cursor=edit->textCursor();
            if(cursor.hasSelection()) {
                cursor.clearSelection();
            }
            edit->insertPlainText(repstr);
        }
        else{
            if(type == 0){
                QMessageBox::information(this,tr("注意"),
                                         tr("已到达文件结尾"),QMessageBox::Ok);
            }
            else{
                QMessageBox::information(this,tr("注意"),
                                         tr("全部替换完成"),QMessageBox::Ok);
            }
            break;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    ui->mdiArea->closeAllSubWindows();
    this->close();
}
void MainWindow::Init()
{
    findlog = NULL;
    connect(ui->newAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->openAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->saveAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->saveasAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->exitAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->undoAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->redoAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->cutAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->copyAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->pasteAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->closeAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->closeallAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->pingAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->cenAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->findAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->nextAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->preAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->aboutAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
    connect(ui->qtAction,SIGNAL(triggered()),
            this,SLOT(doProcessActionSlot()));
}

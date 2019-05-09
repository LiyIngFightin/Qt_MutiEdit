#include "myedit.h"
#include <QtDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QMenu>
#include <QAction>
myEdit::myEdit(QWidget *parent) : QTextEdit(parent)
{
    isNew=true;
}
int myEdit::fileNumber=1;
void myEdit::newFile()
{
    isNew=true;
    currFileName=QString("未命名文档%1.txt").arg(fileNumber++);
    setWindowTitle(currFileName + "[*]" + "-多文本编辑器");
    connect(this->document(),SIGNAL(contentsChanged()),
            this,SLOT(doProcessContentsChangeSlot()));
}

bool myEdit::openFile(const QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法读取文件 %1:\n%2.")
                             .arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    this->setPlainText(in.readAll());
    QGuiApplication::restoreOverrideCursor();
    this->moveCursor(QTextCursor::End);
    QString displayname=QFileInfo(filename).fileName();
    this->currFileName=filename;
    this->isNew=false;
    this->setWindowModified(false);
    this->setWindowTitle(displayname + "[*]");
    connect(this->document(),SIGNAL(contentsChanged()),
            this,SLOT(doProcessContentsChangeSlot()));
    return true;
}

bool myEdit::saveFile()
{
    if(isNew == true) {
        return this->saveasFile();
    }
    else {
        this->isNew=false;
        this->setWindowModified(false);
        this->setWindowTitle(QFileInfo(currFileName).fileName() + "[*]");
        return this->saveData();
    }
}

bool myEdit::saveasFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, currFileName+tr("另存为"),
                                 QDir::currentPath(),
                                 tr("Text files (*.txt)"));
    if(fileName.isEmpty()) {
        QMessageBox::warning(this,"警告",currFileName+"另存为失败",QMessageBox::Ok);
        return false;
    }
    else {
        currFileName=fileName;
        this->isNew=false;
        this->setWindowModified(false);
        this->setWindowTitle(QFileInfo(currFileName).fileName() + "[*]");
        return this->saveData();
    }
}

bool myEdit::saveData()
{
    QString filename=QFileInfo(currFileName).fileName();
    isNew=false;

    QFile file(currFileName);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,"警告","保存失败",QMessageBox::Ok);
        return false;
    }
    QTextStream out(&file);
    out<<this->toPlainText();
    file.close();
    this->setWindowTitle(filename+"[*]");
    this->document()->setModified(false);
    return true;
}

QString myEdit::getCurrentFileName()
{
    return this->currFileName;
}

void myEdit::closeEvent(QCloseEvent *)
{
    if(isNew == true) {
        int btn= QMessageBox::information(this,"提示：",currFileName+"是否保存？",
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel,
                                 QMessageBox::NoButton);
        if(btn ==  QMessageBox::Ok) {
            this->saveasFile();
        }
    }
    else {
        if(document()->isModified()) {
            int btn= QMessageBox::information(this,"提示：",currFileName+"是否保存？",
                                     QMessageBox::Ok,
                                     QMessageBox::Cancel,
                                     QMessageBox::NoButton);
            if(btn ==  QMessageBox::Ok) {
                qDebug()<<"save";
                this->saveFile();
            }
        }
    }
    fileNumber--;
}

void myEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = new QMenu; // 创建菜单，并向其中添加动作
    QAction *undo = menu->addAction(tr("撤销(&U)"),this,
                                    SLOT(undo()),QKeySequence::Undo);
    undo->setEnabled(document()->isUndoAvailable());
    QAction *redo = menu->addAction(tr("恢复(&R)"),this,
                                    SLOT(redo()),QKeySequence::Redo);
    redo->setEnabled(document()->isRedoAvailable());
    menu->addSeparator();
    QAction *cut = menu->addAction(tr("剪切(&T)"),this,
                                   SLOT(cut()),QKeySequence::Cut);
    cut->setEnabled(textCursor().hasSelection());
    QAction *copy = menu->addAction(tr("复制(&C)"),this,
                                    SLOT(copy()),QKeySequence::Copy);
    copy->setEnabled(textCursor().hasSelection());
    menu->addAction(tr("粘贴(&P)"),this,SLOT(paste()),QKeySequence::Paste);
    QAction *clear = menu->addAction(tr("清空"),this,SLOT(clear()));
    clear->setEnabled(!document()->isEmpty());
    menu->addSeparator();
    QAction *select = menu->addAction(tr("全选"),this,
                                SLOT(selectAll()),QKeySequence::SelectAll);
    select->setEnabled(!document()->isEmpty());
    menu->exec(e->globalPos()); // 获取鼠标的位置，然后在这个位置显示菜单
    delete menu;
}

void myEdit::doProcessContentsChangeSlot()
{
    setWindowModified(this->document()->isModified());
}

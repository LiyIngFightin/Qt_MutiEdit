#ifndef MYEDIT_H
#define MYEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QContextMenuEvent>

class myEdit : public QTextEdit
{
    Q_OBJECT
public:

    explicit myEdit(QWidget *parent = nullptr);
    void newFile();
    bool openFile(const QString filename);
    bool saveFile();
    bool saveasFile();
    bool saveData();
    QString getCurrentFileName();
protected:
    void closeEvent(QCloseEvent *);
    void contextMenuEvent(QContextMenuEvent *e);
signals:

public slots:
    void doProcessContentsChangeSlot();
private:
    bool isNew;
    QString currFileName;
    static int fileNumber;
};

#endif // MYEDIT_H

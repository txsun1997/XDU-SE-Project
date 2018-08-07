#include "qpasswordlineedit.h"
#include <QTimer>
#include <QDebug>
QPasswordLineEdit::QPasswordLineEdit(QWidget *parent,int timeout) : QLineEdit(parent)
{
    mTimeout = timeout;
    mLineEditText = "";
    mLastCharCount = 0;
    connect(this,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(slotCursorPositionChanged(int,int)));
    connect(this,SIGNAL(textEdited(const QString&)),this,SLOT(slotTextEdited(const QString&)));
}

QPasswordLineEdit::~QPasswordLineEdit()
{

}

void QPasswordLineEdit::slotCursorPositionChanged(int oldPos,int newPos)
{
    if(oldPos>=-1 && newPos>=0 )
    {
        if(newPos>oldPos)
        {
           QTimer::singleShot(mTimeout,this,SLOT(slotDisplayMaskPassword()));
        }
        else
        {
            this->setCursorPosition(oldPos);
        }
    }
}
void QPasswordLineEdit::slotDisplayMaskPassword()
{
    this->setText(getMaskPassword());
}

void QPasswordLineEdit::slotTextEdited(const QString& text)
{
    if(text.count()>mLastCharCount)//输入
    {
        mLineEditText.append(text.right(1));
    }
    else if(text.count()<mLastCharCount)//删除
    {
        mLineEditText.remove(mLineEditText.count()-1,1);
    }
    mLastCharCount = mLineEditText.count();
}

QString QPasswordLineEdit::getPassword()
{
    return mLineEditText;
}
void QPasswordLineEdit::setTimeout(int msec)
{
    mTimeout = msec;
}

QString QPasswordLineEdit::getMaskPassword()
{
    QString mask = "";
    int count = this->text().length();
    if(count>0)
    {
        for(int i=0;i<count;i++)
        {
            mask += "*";
        }
    }
    return mask;
}

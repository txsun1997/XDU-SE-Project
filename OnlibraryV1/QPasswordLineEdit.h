#ifndef QPASSWORDLINEEDIT_H
#define QPASSWORDLINEEDIT_H

#include <QLineEdit>
class QPasswordLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    //默认输入300毫秒后消失
    QPasswordLineEdit(QWidget *parent,int timeout = 300);
    ~QPasswordLineEdit();

    //获取真实的密码
    QString getPassword();
    //设置显示到隐藏的时间间隔
    void setTimeout(int msec);

private slots:
    void slotCursorPositionChanged(int,int);
    void slotTextEdited(const QString&);
    //显示隐藏后的密码
    void slotDisplayMaskPassword();

private:
    //获取隐藏后的密码，这里为星号
    QString  getMaskPassword();

private:
    int    mTimeout;
    QString    mLineEditText;
    //到最后一个字符的长度
    int    mLastCharCount;
};

#endif // QPASSWORDLINEEDIT_H

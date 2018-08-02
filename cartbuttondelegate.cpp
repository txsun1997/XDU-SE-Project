#include "cartbuttondelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include "cartdetaildlg.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QtGui>
#include <QMessageBox>

c_ButtonDelegate::c_ButtonDelegate(QTableView *&booksView, QObject *parent) :
    QItemDelegate(parent)
{
    tableView = booksView;
    extern QString user_id;
    my_model.setQuery(QString("select books.book_id AS ID,book_name AS Name,payment AS Payment,"
                              "s_number AS Number,writer AS Writer,Options from books,(select * from shopcart where user_id='%1')AS TEMP "
                              "where books.book_id = TEMP.book_id").arg(user_id));
}


void c_ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
    if (!buttons) {
        QStyleOptionButton* button1 = new QStyleOptionButton();
        //button1->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4) , -4); //
        button1->text = tr("详细信息");
        button1->state |= QStyle::State_Enabled;

        QStyleOptionButton* button2 = new QStyleOptionButton();
        //button2->rect = option.rect.adjusted(button1->rect.width() + 4, 4, -4, -4);
        button2->text = tr("删除");
        button2->state |= QStyle::State_Enabled;
        buttons =new  QPair<QStyleOptionButton*, QStyleOptionButton*>(button1, button2);
        (const_cast<c_ButtonDelegate *>(this))->m_btns.insert(index, buttons);
    }
    buttons->first->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4) , -4); //
    buttons->second->rect = option.rect.adjusted(buttons->first->rect.width() + 4, 4, -4, -4);
    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());

    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, buttons->first, painter);
    QApplication::style()->drawControl(QStyle::CE_PushButton, buttons->second, painter);
}

bool c_ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent* e =(QMouseEvent*)event;

        if (m_btns.contains(index)) {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if (btns->first->rect.contains(e->x(), e->y())) {
                btns->first->state |= QStyle::State_Sunken;
            }
            else if(btns->second->rect.contains(e->x(), e->y())) {
                btns->second->state |= QStyle::State_Sunken;
            }
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e =(QMouseEvent*)event;

        if (m_btns.contains(index)) {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if (btns->first->rect.contains(e->x(), e->y())) {
                btns->first->state &= (~QStyle::State_Sunken);
                showMsgDetail();
            } else if(btns->second->rect.contains(e->x(), e->y())) {
                btns->second->state &= (~QStyle::State_Sunken);
                showMsgDelete();
            }
            extern QString user_id;
            my_model.setQuery(QString("select books.book_id AS ID,book_name AS Name,payment AS Payment,"
                                      "s_number AS Number,writer AS Writer,Options from books,(select * from shopcart where user_id='%1')AS TEMP "
                                      "where books.book_id = TEMP.book_id").arg(user_id));
            tableView->setModel(&my_model);
        }
    }
    return true;
}

void c_ButtonDelegate::showMsgDetail()
{
    int n = tableView->currentIndex().row();
    //qDebug() << "n:" << n << endl;
    QString str=my_model.data(my_model.index(n,0)).toString();
    //qDebug() << str << endl;
    CartDetailDlg bookDetail(str);
    bookDetail.setWindowTitle(tr("详细信息"));
    bookDetail.exec();
}

void c_ButtonDelegate::showMsgDelete()
{
    int n = tableView->currentIndex().row();
    //qDebug() << "n:" << n << endl;
    QString bookid = my_model.data(my_model.index(n,0)).toString();
    if (QMessageBox::Yes == QMessageBox::question(NULL, tr("DELETE"),tr(" Confirm to delete?\t "),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        QSqlQuery query;
        extern QString user_id;
        query.exec(QString("select s_number from shopcart where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
        query.next();
        int number = query.value(0).toInt();
        query.exec(QString("update books set stock=stock+%1 where book_id='%2'").arg(number).arg(bookid));

        query.exec(QString("delete from shopcart where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
        QMessageBox::information(NULL,tr("消息"),tr("\t删除成功!\t\t"),QMessageBox::Yes);
    }
}

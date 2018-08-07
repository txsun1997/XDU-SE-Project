#include "orderBtndelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include "detaildlg.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QtGui>
#include <QMessageBox>
#include "scoredlg.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

o_ButtonDelegate::o_ButtonDelegate(QTableView *&booksView, QObject *parent) :
    QItemDelegate(parent)
{
    tableView = booksView;
    extern QString user_id;
    my_model.setQuery(QString("select order_id AS ID,book_name AS Name,total_payment AS Payment,order_time,order_state AS State,Options "
                              "from books,(select * from orders where user_id='%1')AS temp "
                              "where books.book_id=temp.book_id").arg(user_id));
}


void o_ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
    if (!buttons) {
        QStyleOptionButton* button1 = new QStyleOptionButton();
        //button1->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4) , -4); //
        button1->text = tr("评分");
        button1->state |= QStyle::State_Enabled;

        QStyleOptionButton* button2 = new QStyleOptionButton();
        //button2->rect = option.rect.adjusted(button1->rect.width() + 4, 4, -4, -4);
        button2->text = tr("确认收货");
        button2->state |= QStyle::State_Enabled;
        buttons =new  QPair<QStyleOptionButton*, QStyleOptionButton*>(button1, button2);
        (const_cast<o_ButtonDelegate *>(this))->m_btns.insert(index, buttons);
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

bool o_ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
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
                showMsgScore();
            } else if(btns->second->rect.contains(e->x(), e->y())) {
                btns->second->state &= (~QStyle::State_Sunken);
                showMsgReceive();
            }
        }
    }
    return true;
}

void o_ButtonDelegate::showMsgScore()
{
    int n = tableView->currentIndex().row();
    //qDebug() << "n:" << n << endl;
    int order_id = my_model.data(my_model.index(n,0)).toInt();
    //qDebug() << order_id << endl;
    QSqlQuery query;
    extern QString user_id;
    query.exec(QString("select book_id from orders where order_id=%1").arg(order_id));
    query.next();
    QString bookid = query.value(0).toString();
    query.exec(QString("select * from scores where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
    query.next();
    QString uid = query.value(1).toString();
    if(uid.isNull()){
        ScoreDlg scoredlg(order_id);
        scoredlg.setWindowTitle(tr("图书评价"));
        scoredlg.exec();
    }
    else{
        QMessageBox::about(NULL,tr("消息"),tr("\t无法重复评分!\t\t"));
    }
}

void o_ButtonDelegate::showMsgReceive()
{
    int n = tableView->currentIndex().row();
    //qDebug() << "n:" << n << endl;
    int order_id = my_model.data(my_model.index(n,0)).toInt();
    if (QMessageBox::Yes == QMessageBox::question(NULL, tr("确认收货"),tr("\t是否确认收货?\t\t"),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        QSqlQuery query;
        query.exec(QString("select order_state from orders where order_id=%1").arg(order_id));
        query.next();
        QString jianyan = query.value(0).toString();
        if(jianyan.trimmed()==QString("RECEIVED")){
            QMessageBox::about(NULL,tr("消息"),tr("\t无法重复确认收货!\t\t"));
            return ;
        }

        query.exec(QString("UPDATE orders SET order_state='RECEIVED' WHERE order_id=%1").arg(order_id));
        //my_model.setQuery(QString("UPDATE orders SET order_state='RECEIVED' WHERE order_id=%1").arg(order_id));
        extern QString user_id;
        my_model.setQuery(QString("select order_id AS ID,book_name AS Name,total_payment AS Payment,order_time,order_state AS State,Options "
                                  "from books,(select * from orders where user_id='%1')AS temp "
                                  "where books.book_id=temp.book_id").arg(user_id));
        tableView->setModel(&my_model);//刷新数据
    }
}

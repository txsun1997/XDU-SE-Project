#include "orderBtndelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QtGui>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

o_ButtonDelegate::o_ButtonDelegate(QTableView *&booksView, QObject *parent) :
    QItemDelegate(parent)
{
    tableView = booksView;
    my_model.setQuery(QString("select order_id AS ID,book_name AS Name,user_id AS Buyer,total_payment AS Payment,number AS Num,order_time,order_state AS State,Options "
                              "from books,(select * from orders)AS temp "
                              "where books.book_id=temp.book_id"));
}


void o_ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
    if (!buttons) {
        QStyleOptionButton* button1 = new QStyleOptionButton();
        //button1->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4) , -4); //
        button1->text = tr("取消订单");
        button1->state |= QStyle::State_Enabled;

        QStyleOptionButton* button2 = new QStyleOptionButton();
        //button2->rect = option.rect.adjusted(button1->rect.width() + 4, 4, -4, -4);
        button2->text = tr("发货");
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
                showMsgCancel();
            } else if(btns->second->rect.contains(e->x(), e->y())) {
                btns->second->state &= (~QStyle::State_Sunken);
                showMsgSend();
            }
            my_model.setQuery(QString("select order_id AS ID,book_name AS Name,user_id AS Buyer,total_payment AS Payment,number AS Num,order_time,order_state AS State,Options "
                                      "from books,(select * from orders)AS temp "
                                      "where books.book_id=temp.book_id"));
            tableView->setModel(&my_model);
        }
    }
    return true;
}

void o_ButtonDelegate::showMsgCancel() //取消订单
{
    int n = tableView->currentIndex().row();
    //qDebug() << "n:" << n << endl;
    int order_id = my_model.data(my_model.index(n,0)).toInt();
    if (QMessageBox::Yes == QMessageBox::question(NULL, tr("Cancel Order"),tr(" Confirm to cancel the order?\t "),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        QSqlQuery query;
        query.exec(QString("select order_state from orders where order_id=%1").arg(order_id));
        query.next();
        QString jianyan = query.value(0).toString();
        if(jianyan.trimmed()!=QString("UNSHIPPED")){
            QMessageBox::about(NULL,tr("消息"),tr("\tCan not Cancel the Order!\t\t"));
            return ;
        }

        query.exec(QString("select user_id from orders where order_id=%1").arg(order_id));
        query.next();
        QString uuid = query.value(0).toString();
        query.exec(QString("update users set msg=%1 where user_id='%2'").arg(order_id).arg(uuid));
        query.exec(QString("delete from orders where order_id='%1'").arg(order_id));
        //my_model.setQuery(QString("UPDATE orders SET order_state='RECEIVED' WHERE order_id=%1").arg(order_id));
        query.exec(QString("select * from orders where order_id='%1'").arg(order_id));
        query.next();
        QString uid = query.value(1).toString();
        if(uid.isNull()){
            QMessageBox::information(NULL,"Infomation","  Cancel success!\t ",QMessageBox::Yes);
        }
        else{
            QMessageBox::information(NULL,"Infomation","  Cancel failed!\t ",QMessageBox::Yes);
        }

    }
}

void o_ButtonDelegate::showMsgSend()  //发货
{
    int n = tableView->currentIndex().row();
    //qDebug() << "n:" << n << endl;
    int order_id = my_model.data(my_model.index(n,0)).toInt();
    QSqlQuery query;
    query.exec(QString("select order_state from orders where order_id=%1").arg(order_id));
    query.next();
    QString jianyan = query.value(0).toString();
    if(jianyan.trimmed()!=QString("UNSHIPPED")){
        QMessageBox::about(NULL,tr("消息"),tr("\t无法重复发货!\t\t"));
        return ;
    }
    query.exec(QString("update orders set order_state='SENDING' where order_id='%1'").arg(order_id));
    query.exec(QString("select order_state from orders where order_id='%1'").arg(order_id));
    query.next();
    QString state = query.value(0).toString();
    QString match = "SENDING";
    if(state.trimmed()==match){
        QMessageBox::information(NULL,"Infomation","  The Order has been Shipped!\t ",QMessageBox::Yes);
    }
    else{
        QMessageBox::information(NULL,"Infomation","  Error!\t ",QMessageBox::Yes);
    }
}

#include "buttondelegate.h"

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
#include "buydlg.h"

ButtonDelegate::ButtonDelegate(QString str, QTableView *&booksView, QObject *parent) :
    QItemDelegate(parent)
{
    tableView = booksView;
    ss = str;
    my_model.setQuery(str);
}


void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
    if (!buttons) {
        QStyleOptionButton* button1 = new QStyleOptionButton();
        //button1->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4) , -4); //
        button1->text = tr("详细信息");
        button1->state |= QStyle::State_Enabled;

        QStyleOptionButton* button2 = new QStyleOptionButton();
        //button2->rect = option.rect.adjusted(button1->rect.width() + 4, 4, -4, -4);
        button2->text = tr("购买");
        button2->state |= QStyle::State_Enabled;
        buttons =new  QPair<QStyleOptionButton*, QStyleOptionButton*>(button1, button2);
        (const_cast<ButtonDelegate *>(this))->m_btns.insert(index, buttons);
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

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
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
                showMsgBuy();
            }
            //my_model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
            //                          "book_press AS Press,writer AS Writer,Options from books where stock>0"));
            //tableView->setModel(&my_model);
            my_model.setQuery(ss);
            tableView->setModel(&my_model);
        }
    }
    return true;
}

void ButtonDelegate::showMsgDetail()
{
    int n = tableView->currentIndex().row();
    QString str=my_model.data(my_model.index(n,0)).toString();
    QSqlQuery query;
    extern QString user_id;
    query.exec(QString("select l_score from history where user_id='%1' AND book_id='%2'").arg(user_id).arg(str));
    query.next();
    QString jianyan = query.value(0).toString();
    if(jianyan.isNull()){
        query.exec(QString("insert into history values('%1','%2',1)").arg(user_id).arg(str));
    }
    DetailDlg bookDetail(str);
    bookDetail.setWindowTitle(tr("详细信息"));
    bookDetail.exec();
}

void ButtonDelegate::showMsgBuy()
{
    QSqlQuery query;
    extern QString user_id;
    int n = tableView->currentIndex().row();
    QString str = my_model.data(my_model.index(n,0)).toString();
    query.exec(QString("SELECT stock FROM books WHERE book_id = '%1'").arg(str));
    query.next();
    int stock = query.value(0).toInt();
    if(stock>=0){
        qDebug() << "flag" << endl;
        BuyDlg buy(user_id,str);
        buy.setWindowTitle(tr("订单信息"));
        buy.exec();
        extern QString user_id;
        query.exec(QString("select * from history where user_id='%1' AND book_id='%2'").arg(user_id).arg(str));
        query.next();
        QString jianyan = query.value(0).toString();
        if(jianyan.isNull()){
            query.exec(QString("insert into history values('%1','%2',3)").arg(user_id).arg(str));
        }
        else{
            query.exec(QString("update history set l_score=3 where user_id='%1' AND book_id='%2'").arg(user_id).arg(str));
        }
    }
    else{
        QMessageBox::about(NULL,tr("消息"),tr("\t库存不足!\t\t"));
    }
}

#include "buttondelegate.h"

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
#include "updatedlg.h"
#include "adddlg.h"

ButtonDelegate::ButtonDelegate(QTableView *&booksView, QObject *parent) :
    QItemDelegate(parent)
{
    tableView = booksView;
    my_model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                           "book_press AS Press,writer AS Writer,Options from books"));
}


void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
    if (!buttons) {
        QStyleOptionButton* button1 = new QStyleOptionButton();
        //button1->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4) , -4); //
        button1->text = tr("修改");
        button1->state |= QStyle::State_Enabled;

        QStyleOptionButton* button2 = new QStyleOptionButton();
        //button2->rect = option.rect.adjusted(button1->rect.width() + 4, 4, -4, -4);
        button2->text = tr("库存管理");
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
                showMsgUpdate();
            } else if(btns->second->rect.contains(e->x(), e->y())) {
                btns->second->state &= (~QStyle::State_Sunken);
                showMsgAdd();
            }
            my_model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                                   "book_press AS Press,writer AS Writer,Options from books"));
            tableView->setModel(&my_model);

        }
    }
    return true;
}

void ButtonDelegate::showMsgUpdate()
{
    int n = tableView->currentIndex().row();
    //qDebug() << "n:" << n << endl;
    QString str=my_model.data(my_model.index(n,0)).toString();
    //qDebug() << str << endl;
    UpdateDlg bookDetail(str);
    bookDetail.setWindowTitle(tr("修改信息"));
    bookDetail.exec();
}

void ButtonDelegate::showMsgAdd()
{
    int n = tableView->currentIndex().row();
    //qDebug() << "n:" << n << endl;
    QString str=my_model.data(my_model.index(n,0)).toString();
    //qDebug() << str << endl;
    AddDlg add(str);
    add.setWindowTitle(tr("新增"));
    add.exec();
}

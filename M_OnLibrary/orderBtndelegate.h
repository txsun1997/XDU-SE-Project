#ifndef ORDERBTNDELEGATE_H
#define ORDERBTNDELEGATE_H
#include "mainwindow.h"

#include <QItemDelegate>
#include <QtWidgets/QTableView>

class o_ButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit o_ButtonDelegate(QTableView *&booksView, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    QTableView *tableView;
    QSqlQueryModel my_model;
signals:

public slots:

private:
//public:
    void showMsgCancel();
    void showMsgSend();

private:
//public:
    typedef QMap<QModelIndex, QPair<QStyleOptionButton*, QStyleOptionButton*>* >  collButtons;
    collButtons m_btns;

};

#endif // BUTTONDELEGATE_H

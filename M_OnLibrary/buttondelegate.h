#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H
#include "mainwindow.h"

#include <QItemDelegate>
#include <QtWidgets/QTableView>

class ButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ButtonDelegate(QTableView *&booksView, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    QTableView *tableView;
    QSqlQueryModel my_model;
signals:

public slots:

private:
//public:
    void showMsgUpdate();
    void showMsgAdd();

private:
//public:
    typedef QMap<QModelIndex, QPair<QStyleOptionButton*, QStyleOptionButton*>* >  collButtons;
    collButtons m_btns;

};

#endif // BUTTONDELEGATE_H

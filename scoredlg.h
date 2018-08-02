#ifndef SCOREDLG_H
#define SCOREDLG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

namespace Ui {
class ScoreDlg;
}

class ScoreDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ScoreDlg(int str, QWidget *parent = 0);
    ~ScoreDlg();

private slots:
    void on_commitBtn_clicked();

private:
    Ui::ScoreDlg *ui;
    QString bookid;
};

#endif // SCOREDLG_H

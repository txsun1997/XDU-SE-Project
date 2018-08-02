#ifndef ZHAOHUIDLG_H
#define ZHAOHUIDLG_H

#include <QDialog>

namespace Ui {
class ZhaohuiDlg;
}

class ZhaohuiDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ZhaohuiDlg(QWidget *parent = 0);
    ~ZhaohuiDlg();

private slots:
    void on_commitBtn_clicked();

private:
    Ui::ZhaohuiDlg *ui;
};

#endif // ZHAOHUIDLG_H

#include "newbookdlg.h"
#include "ui_newbookdlg.h"
#include <QtGui>
#include <QMessageBox>

NewbookDlg::NewbookDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewbookDlg)
{
    ui->setupUi(this);

   // ui->comboBox->setView(new QListView());

    ui->comboBox->addItem(tr("Literature"));
    ui->comboBox->addItem(tr("Social Science"));
    ui->comboBox->addItem(tr("Text Book"));
    ui->comboBox->addItem(tr("Teaching-Material"));
    ui->comboBox->addItem(tr("Novel"));
    ui->comboBox->addItem(tr("Economy Management"));
    ui->comboBox->addItem(tr("Technology"));
    ui->comboBox->addItem(tr("Computer Science"));
}

NewbookDlg::~NewbookDlg()
{
    delete ui;
}

void NewbookDlg::on_updateBtn_clicked()
{
    QSqlQuery query;
    QString id = ui->idEdit->text();
    QString name = ui->nameEdit->text();
    QString price = ui->priceEdit->text();
    QString press = ui->pressEdit->text();
    QString writer = ui->writerEdit->text();
    int stock = ui->spinBox->value();
    double d_price=price.toDouble();
    QString info = ui->textEdit->toPlainText();

    int index = ui->comboBox->currentIndex();
    qDebug() << index << endl;
    //检验ID是否能作为主码
    query.exec(QString("select * from books where book_id='%1'").arg(id));
    query.next();
    QString str = query.value(0).toString();
    if(str.isNull()){  //说明该ID合法
        switch(index)
        {
            case 0:
            {
                query.exec(QString("insert into books values ('%1','%2',%3,'%4','%5','123',NULL,'%6',%7,'literature')"
                               "").arg(id).arg(name).arg(d_price).arg(press).arg(writer).arg(info).arg(stock));
                break;
            }
            case 1:
            {
                query.exec(QString("insert into books values ('%1','%2',%3,'%4','%5','123',NULL,'%6',%7,'Social Science')"
                               "").arg(id).arg(name).arg(d_price).arg(press).arg(writer).arg(info).arg(stock));
                break;
            }
            case 2:
            {
                query.exec(QString("insert into books values ('%1','%2',%3,'%4','%5','123',NULL,'%6',%7,'textbook')"
                               "").arg(id).arg(name).arg(d_price).arg(press).arg(writer).arg(info).arg(stock));
                break;
            }
            case 3:
            {
                query.exec(QString("insert into books values ('%1','%2',%3,'%4','%5','123',NULL,'%6',%7,'ttcj')"
                               "").arg(id).arg(name).arg(d_price).arg(press).arg(writer).arg(info).arg(stock));
                break;
            }
            case 4:
            {
                query.exec(QString("insert into books values ('%1','%2',%3,'%4','%5','123',NULL,'%6',%7,'Novel')"
                               "").arg(id).arg(name).arg(d_price).arg(press).arg(writer).arg(info).arg(stock));
                break;
            }
            case 5:
            {
                query.exec(QString("insert into books values ('%1','%2',%3,'%4','%5','123',NULL,'%6',%7,'Economy management')"
                               "").arg(id).arg(name).arg(d_price).arg(press).arg(writer).arg(info).arg(stock));
                break;
            }
            case 6:
            {
                query.exec(QString("insert into books values ('%1','%2',%3,'%4','%5','123',NULL,'%6',%7,'Technology')"
                               "").arg(id).arg(name).arg(d_price).arg(press).arg(writer).arg(info).arg(stock));
                break;
            }
            case 7:
            {
                query.exec(QString("insert into books values ('%1','%2',%3,'%4','%5','123',NULL,'%6',%7,'Computer Science')"
                               "").arg(id).arg(name).arg(d_price).arg(press).arg(writer).arg(info).arg(stock));
                break;
            }
        }



        query.exec(QString("select * from books where book_id='%1'").arg(id));
        query.next();
        QString jianyan = query.value(0).toString();
        if(jianyan.isNull()){
            QMessageBox::about(this,tr("消息"),tr("\t添加失败!\t\t"));
        }
        else{
            QMessageBox::about(this,tr("消息"),tr("\t添加成功!\t\t"));
        }
        this->close();
    }
    else{
        QMessageBox::about(this,tr("消息"),tr("\tInvalid ID!\t\t"));
        ui->idEdit->clear();
        ui->idEdit->setFocus();
    }

}

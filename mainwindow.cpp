#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <ntp.h>
#include <QCompleter>
#include "version.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSock = new QUdpSocket(this);

    setWindowTitle(QString(tr("NTP服务器测试工具 V%1.%2").arg(VERSION_MAIN).arg(VERSION_SECOND)));

    ui->plainTextEdit->document()->setMaximumBlockCount(999);

    /* 输入提示配置 */
    QStringList list;
    list << "64.113.32.5";
    list << "52.178.223.23";
    QCompleter *completer = new QCompleter(list, this);
    ui->lineEdit->setCompleter(completer);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete udpSock;
}

void MainWindow::msgShow(QString &msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

QString MainWindow::timestamp()
{
    QDateTime time = QDateTime::currentDateTime();
    return time.toString(QString(tr("[hh:mm:ss.zzz]")));
}

void MainWindow::udpReadyRead()
{
    QString msg;
    QByteArray data;

    if (udpSock && udpSock->isReadable() && udpSock->bytesAvailable()) {
        data = udpSock->readAll();
        QString parse;
        time_t sec = ntp_parse_sec(data.data(), data.size());
        QDateTime time;
        time.setTime_t(sec);
        parse.append(time.toString("yyyy/MM/dd HH:mm:ss"));

        msg.append(timestamp() + QString(tr(" RECV[%1] ").arg(data.size())) + parse);
        msgShow(msg);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString msg;

    if (udpSock->isOpen() == false) {
        QHostAddress ntp;
        ntp.setAddress(ui->lineEdit->text());
        udpSock->connectToHost(ntp, 123);
        connect(udpSock,SIGNAL(readyRead()),this,SLOT(udpReadyRead()));
        msg.append(timestamp() + QString(tr(" 正在连接到服务器...")));
        msgShow(msg);
    }

    if (udpSock->isOpen() && udpSock->isWritable()) {
        msg.clear();

        QByteArray data;
        data.resize(128);
        int len = ntp_packet_build(data.data(), data.size(), ui->spinBox->value());
        if (len > 0) {
            udpSock->write(data.data(), len);
            msg.append(timestamp() + QString(tr(" 发送NTP数据到服务器...")));
        } else {
            msg.append(timestamp() + QString(tr(" 发送NTP数据失败，组包错误")));
        }
        msgShow(msg);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->plainTextEdit->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    QString msg;

    if (udpSock->isOpen()) {
        udpSock->close();
        msg.append(timestamp() + QString(tr(" 连接关闭成功")));
    } else {
        msg.append(timestamp() + QString(tr(" 连接已关闭")));
    }

    msgShow(msg);
}

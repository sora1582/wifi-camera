#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    tSocket = new QTcpSocket;
    tSocket->connectToHost("59.110.90.231", 8000);

    connect(tSocket, &QTcpSocket::connected, [this]()
    {
        qDebug() << "连接服务器成功";
    });

    connect(tSocket, &QTcpSocket::readyRead, this, &Widget::tcpReadSlot);

    QJsonObject obj;
    obj.insert("cmd", "get_video_data");
    obj.insert("appid", "1000");
    obj.insert("deviceid", "0001");
    tSocket->write(QJsonDocument(obj).toJson());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::tcpReadSlot()
{
    QByteArray ba = tSocket->readAll();
    QJsonObject obj = QJsonDocument::fromJson(ba).object();
    if (obj.value("cmd").toString() == "reply_port_info")
    {
        port = obj.value("port").toInt();

        //初始化QUdpSocket
        uSocket = new QUdpSocket;
        uSocket->writeDatagram(QByteArray("hello"), QHostAddress("59.110.90.231"), port);
        connect(uSocket, &QUdpSocket::readyRead, this, &Widget::udpReadSlot);
    }
    else
    {
        qDebug() << "服务器返回数据有误";
    }
}

//接收视频数据
void Widget::udpReadSlot()
{
    QByteArray ba;
    ba.resize(uSocket->pendingDatagramSize());
    uSocket->readDatagram(ba.data(), ba.size());

    QPixmap pix;
    pix.loadFromData(ba);
    ui->label->setPixmap(pix);
}

void Widget::on_upButton_clicked()
{
    QJsonObject obj;
    obj.insert("cmd", "control");
    obj.insert("action", "up");
    obj.insert("appid", "1000");
    obj.insert("deviceid", "0001");

    tSocket->write(QJsonDocument(obj).toJson());
}

void Widget::on_downButton_clicked()
{
    QJsonObject obj;
    obj.insert("cmd", "control");
    obj.insert("action", "down");
    obj.insert("appid", "1000");
    obj.insert("deviceid", "0001");

    tSocket->write(QJsonDocument(obj).toJson());
}



void Widget::on_leftButton_clicked()
{
    QJsonObject obj;
    obj.insert("cmd", "control");
    obj.insert("action", "left");
    obj.insert("appid", "1000");
    obj.insert("deviceid", "0001");

    tSocket->write(QJsonDocument(obj).toJson());
}

void Widget::on_rightButton_clicked()
{
    QJsonObject obj;
    obj.insert("cmd", "control");
    obj.insert("action", "right");
    obj.insert("appid", "1000");
    obj.insert("deviceid", "0001");

    tSocket->write(QJsonDocument(obj).toJson());
}

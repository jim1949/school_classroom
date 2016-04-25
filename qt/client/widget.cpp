//Team Explorer
//author:rescuer-Liao

#include "widget.h"
#include "ui_widget.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QDebug>

using namespace cv ;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    send_image_(false)
{
    camera_server_ip_ = new QHostAddress() ;
    camera_client_ = new ImageSocket(this) ;
    ui->setupUi(this);
    captrue_ = new Camera(this , 0) ;
    connect(ui->pushButton_open,SIGNAL(clicked(bool)),captrue_,SLOT(getCaptrueOn())) ;
    connect(ui->pushButton_close,SIGNAL(clicked(bool)),captrue_,SLOT(closeCaptrue())) ;
    connect(ui->pushButton_close,SIGNAL(clicked(bool)),this,SLOT(closeImage())) ;
    connect(captrue_,SIGNAL(getImageInfo(QImage)),this , SLOT(getImage(QImage))) ;
    connect(ui->pushButton_connect,SIGNAL(clicked(bool)),this,SLOT(sendCamera())) ;
    connect(this->captrue_,SIGNAL(updatePeopleNum(int)),this,SLOT(updatePeople(int))) ;
    camera_server_ip_->setAddress("127.0.0.1");
    camera_client_->connectToHost(*camera_server_ip_,8001);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::getImage(QImage image)
{
    ui->frame->setImage(image);
    if(send_image_)
    {
        qDebug()<<"send image" ;
        camera_client_->sendImage(ui->lineEdit_classroom->text(),
                                  ui->lineEdit_people->text(),image);
    }
}

void Widget::closeImage()
{
    ui->frame->setNoImage();
}

void Widget::updatePeople(int a)
{
    ui->lineEdit_people->setText(QString::number(a));
}

void Widget::sendCamera()
{
    if(!send_image_)
    {
        send_image_ = true ;
        camera_client_->connectToHost(*camera_server_ip_,8001);
        ui->pushButton_connect->setText("unconnect");
    }
    else
    {
        send_image_ = false ;
        camera_client_->abort();
        ui->pushButton_connect->setText("connect");
    }
}

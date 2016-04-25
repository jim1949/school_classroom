#include "videodevice.h"
#include <QDebug>
VideoDevice::VideoDevice(QObject *parent) : QObject(parent)
{
    this->cp_ = NULL ;
}

void VideoDevice::setDevNum(int dev_num)
{
    this->dev_num_ = dev_num ;
}

void VideoDevice::openDevice()
{
    //cp_ = cvCreateCameraCapture(dev_num_) ;
    cp_=cvCreateFileCapture("/home/liao/Documents/homework/out.avi");
}

void VideoDevice::closeDevice()
{
    cvReleaseCapture(&cp_) ;
}
QImage VideoDevice::getFrame()
{
    qDebug()<<"frame" ;
    frame_ = cvQueryFrame(cp_) ;
    cvCvtColor(frame_,frame_ , CV_BGR2RGB);
    QImage image((const uchar *)frame_->imageData,frame_->width,frame_->height,QImage::Format_RGB888) ;
    //add algorithm at this function
    if(image_callback_)image_callback_(frame_) ;

    return image ;
}


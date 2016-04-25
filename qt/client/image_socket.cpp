#include "image_socket.h"

ImageSocket::ImageSocket(QObject *parent)
{

}

void ImageSocket::sendImage(QString classroom_name , QString people_num,QImage &current_image)
{
    int size ;
    QByteArray bArr ;
    QBuffer buffer(&bArr) ;
    buffer.open(QIODevice::WriteOnly) ;
    current_image.save(&buffer,"jpg",-1) ;
    size = bArr.size() ;
    QDataStream outcamera(this) ;
    outcamera.setVersion(QDataStream::Qt_5_5);
    outcamera<<classroom_name<<people_num<<(qint64)size<<bArr ;
}

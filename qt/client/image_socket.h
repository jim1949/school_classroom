#ifndef IMAGESOCKET_H
#define IMAGESOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QBuffer>
#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class ImageSocket : public QTcpSocket
{
    Q_OBJECT
public:
    ImageSocket(QObject *parent);
public slots:
    void sendImage(QString classroom_name, QString people_num, QImage &current_image) ;
};

#endif // IMAGESOCKET_H

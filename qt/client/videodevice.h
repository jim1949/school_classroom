#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <QObject>
#include <QImage>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvwimage.h>
#include <boost/function.hpp>

typedef boost::function<void(IplImage *)> ImageCallback ;

class VideoDevice : public QObject
{
    Q_OBJECT
public:
    explicit VideoDevice(QObject *parent = 0);
    void openDevice() ;
    void setDevNum(int dev_num) ;
    void closeDevice() ;
    QImage getFrame() ;
    void setImageCallBack(const ImageCallback &image_callback){this->image_callback_ = image_callback;}
signals:
     void getImage(IplImage *) ;
public slots:
private:
    CvCapture *cp_ ;
    IplImage *frame_ ;
    int dev_num_ ;
    ImageCallback image_callback_ ;
};

#endif // VIDEODEVICE_H

#ifndef CAMERA_H
#define CAMERA_H

#include "videodevice.h"
#include "mOpenCV.h"
#include <QObject>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QTimer>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <boost/bind.hpp>

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent,int dev_id) ;
    ~Camera() ;
    QString getThisClassroomName()
    {
        return this->classroom_name_ ;
    }
    int getThisClassRoomPeople()
    {
        return this->current_people_num_ ;
    }
    void ImageCallback(IplImage *rawImage) ;
signals:
    void getImageInfo(QImage) ;
    void updatePeopleNum(int) ;
public slots:
    void getCaptrueOn() ;
    void closeCaptrue() ;
private slots:
    void timeOut() ;
private:
    bool camera_on_  ;
    VideoDevice *current_video_ ;
    QTimer *camera_timer_ ;
    QString classroom_name_ ;
    int current_people_num_ ;
    int dev_id_ ;
};

#endif // CAMERA_H

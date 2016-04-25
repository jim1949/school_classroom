#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include "camera.h"
#include <QImage>
#include <image_socket.h>
#include <QTcpServer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void getImage(QImage image) ;
    void closeImage() ;
    void sendCamera() ;
    void updatePeople(int a) ;
private:
    bool send_image_ ;
    Ui::Widget *ui;
    Camera *captrue_ ;
    ImageSocket *camera_client_ ;
    QHostAddress *camera_server_ip_ ;
};

#endif // WIDGET_H

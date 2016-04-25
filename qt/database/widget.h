#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <classroom_treeview.h>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <classroom_database.h>
#include <map>
#include <QThread>
#include <global_file.h>


namespace Ui {
class Widget;
}

class PicSaveThread : public QThread
{
    Q_OBJECT
public:
    PicSaveThread() ;
    PicSaveThread(QString file_name , QImage &current_image) ;
    ~PicSaveThread() ;
    void setPicInfo(QString file_name , QImage &img) ;
private slots:
    void run();
private :
    QString file_name_ ;
    QImage current_image_ ;
};


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void initBuildingName() ;
    void initClassRoomName() ;
    QString getCurrentTime(bool need_sec,bool need_point = false) ;
    void initClassroomTreeView() ;
    void initTcp() ;
    void initTimer() ;
    void addImageTime(const QString &classroom_name , QImage &current_image) ;

public slots:
    void acceptConnection() ;
    void setCurrentTime() ;
    void readClient() ;
    void setCurrentClassRoomImage(QString current_classroom_name) ;
    void savePic(QString pic_name,QImage &current_image) ;
//    void checkImage() ;


private slots:
    void on_pushButton_operate_clicked();

    void on_pushButton_database_clicked();

    void on_pushButton_user_clicked();

private:
    Ui::Widget *ui;
    QStringList build_list_ ;
    QMap<QString,QStringList> classroom_list_ ;
    ClassRoomTreeView *classroom_tree_view_ ;
    QTimer *lcd_timer_ ;
//    QTimer *camera_check_timer_ ;
    //QMap<QString , QImage> classroom_image_ ;
    QTcpServer *sever_ ;
    QTcpSocket *client_ ;
    QString current_classroom_image_ ;
    ClassRoomDataBase *database ;
    std::map<QString , QTime> classroom_frame_num_ ;
};
#endif // WIDGET_H

#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
    this->setWindowTitle("工大自习室管理系统");
    initTimer();
    initClassroomTreeView();
    initTcp();
    database = new ClassRoomDataBase() ;
    connect(this->classroom_tree_view_,SIGNAL(setClassroomImage(QString)),
            this , SLOT(setCurrentClassRoomImage(QString))) ;
}

Widget::~Widget()
{
    qDebug()<<"!@D" ;
    delete ui;
}

void Widget::initBuildingName()
{
    build_list_<<"教西A楼"<<"教西B楼"<<"教西C楼"<<"教西D楼" ;
    GlobalFile::build_list_ = build_list_ ;
}

void Widget::initClassRoomName()
{
    classroom_list_["教西A楼"]<<"A101"<<"A105"<<"A103"<<"A106";
    classroom_list_["教西A楼"]<<"A201"<<"A205"<<"A202"<<"A206";
    classroom_list_["教西A楼"]<<"A301"<<"A305"<<"A302"<<"A306" ;

    classroom_list_["教西B楼"]<< "B103"<<"B105"<<"B102"<<"B106";
    classroom_list_["教西B楼"]<< "B203"<<"B205"<<"B207"<<"B209"<<"B211"<<"B202"<<"B206"<<"B208"<<"B210"<<"B212"<<"B214"<<"B216";
    classroom_list_["教西B楼"]<<"B303"<<"B305"<<"B307"<<"B309"<<"B311"<<"B302"<<"B306"<<"B308"<<"B310"<<"B312"<<"B314"<<"B316" ;

    classroom_list_["教西C楼"]<< "C203"<<"C205"<<"C207"<<"C209"<<"C211"<<"C202"<<"C206"<<"C208"<<"C210"<<"C212"<<"C214"<<"C216";
    classroom_list_["教西C楼"]<<"C303"<<"C305"<<"C307"<<"C309"<<"C311"<<"C302"<<"C306"<<"C308"<<"C310"<<"C312"<<"C314"<<"C316";

    classroom_list_["教西D楼"]<<"D101"<<"D105"<<"D102"<<"D106";
    classroom_list_["教西D楼"]<<"D201"<<"D205"<<"D202"<<"D206" ;
    classroom_list_["教西D楼"]<<"D301"<<"D305"<<"D302"<<"D306" ;
    GlobalFile::classroom_list_ = classroom_list_ ;
}


void Widget::setCurrentTime()
{
    static int i = 0 ;
    QString lcd_text ;
    if(i%2)lcd_text = getCurrentTime(false) ;
    else lcd_text = getCurrentTime(false,true) ;
    ui->lcdNumber_time->display(lcd_text);
    i++ ;
}

QString Widget::getCurrentTime(bool need_sec, bool need_point)
{
    QTime current_time = QTime::currentTime() ;
    QString current_time_string ;
    if(need_sec)
    {
         current_time_string = current_time.toString("hh:mm:ss") ;
    }
    else
    {
        if(need_point)
        {
            current_time_string = current_time.toString("hh:mm") ;
        }
        else
        {
            current_time_string = current_time.toString("hh mm") ;
        }
    }
    return current_time_string ;
}

void Widget::readClient()
{
    qint64 image_size ;
    QString classroom_name ;
    QString current_people ;
    QByteArray socket_data ;
    QByteArray byte_image ;
    socket_data = client_->readAll() ;
    QDataStream buffer(socket_data) ;
    buffer>>classroom_name ;
    buffer>>current_people ;
    buffer>>image_size ;
    byte_image.resize(image_size);
    buffer>>byte_image ;
    QImage image ;
    image.loadFromData(byte_image)  ;
    addImageTime(classroom_name,image);
    //classroom_image_[classroom_name] = image ;
    qDebug()<<classroom_name<<" "<<current_people<<" "<<image_size ;
    this->classroom_tree_view_->setCurrentPeople(classroom_name,current_people.toInt());
    if(current_classroom_image_ == classroom_name)
    {
        ui->frame_image->setImage(image);
        if(classroom_frame_num_[classroom_name].isNull())
        {
            classroom_frame_num_[classroom_name]  = QTime::currentTime() ;
            PicSaveThread *save_pic = new PicSaveThread(classroom_name,image) ;
//            savePic(classroom_name,image);
            save_pic->start();
        }

        QTime last_time  = classroom_frame_num_[classroom_name] ;
        int min = QTime::currentTime().minute() - last_time.minute() ;
        if(min>=1)
        {
            PicSaveThread *save_pic = new PicSaveThread(classroom_name,image) ;
            save_pic->start();
//            savePic(classroom_name,image);
            classroom_frame_num_[classroom_name] = QTime::currentTime() ;
        }
    }
    else
    {
        QImage no_image ;
         ui->frame_image->setImage(no_image);
    }
}

void Widget::acceptConnection()
{
    client_ = sever_->nextPendingConnection() ;
    qDebug()<<"new client in!!" ;
    connect(client_,SIGNAL(readyRead()),this,SLOT(readClient()));
}

void Widget::initClassroomTreeView()
{
    QStringList horizontalHeader ;
    horizontalHeader<<"教室"<<"当前人数" ;
    classroom_tree_view_ = new ClassRoomTreeView() ;
    initBuildingName();
    initClassRoomName();
    classroom_tree_view_->setTreeView(ui->treeView_classroom,horizontalHeader);
    classroom_tree_view_->setBuildingName(build_list_);
    for(int i = 0 ; i < build_list_.length() ; i++)
    {
        classroom_tree_view_->setClassRoomName(build_list_.at(i),
                                               classroom_list_[build_list_.at(i)]);
    }
    this->classroom_tree_view_->getRoomTreeViewInit();
}

void Widget::initTcp()
{
    sever_ = new QTcpServer() ;
    bool is_listen = false ;
    is_listen =  sever_->listen(QHostAddress::Any,8001) ;
    if(!is_listen)
    {
        qDebug()<<"listen error!" ;
        exit(1);
    }
    connect(sever_,SIGNAL(newConnection()),this,SLOT(acceptConnection())) ;
}

void Widget::initTimer()
{
//    camera_check_timer_ = new QTimer(this) ;
    lcd_timer_ = new QTimer(this);
//    connect(lcd_timer_,SIGNAL(timeout()),this,SLOT(checkImage())) ;
    connect(lcd_timer_,SIGNAL(timeout()),this,SLOT(setCurrentTime())) ;
    lcd_timer_->start(1000);
//    camera_check_timer_->start(5000);
}

void Widget::addImageTime(const QString &classroom_name, QImage &current_image)
{
    QPainter painter(&current_image) ;
    painter.setPen(Qt::white);
    QFont font ;
    font.setPixelSize(10);
    QTime current_time = QTime::currentTime() ;
    QString image_info = classroom_name+current_time.toString("hh:mm:ss") ;
    painter.setFont(font);
    painter.drawText(0,0,image_info);
}

void Widget::setCurrentClassRoomImage(QString current_classroom_name)
{
    QImage no_image ;
    ui->frame_image->setImage(no_image);
    this->current_classroom_image_ = current_classroom_name ;
}

//void Widget::checkImage()
//{
//    if(ui->frame_image->getImage().isNull())
//    {
//        QImage no_image ;
//        ui->frame_image->setImage(no_image);
//    }
//}



void Widget::on_pushButton_database_clicked()
{
    if(GlobalFile::CURRENT_USER_ROLE==1)
    {
        database->showTabelView();
    }
    else
    {
        QMessageBox::critical(NULL,"权限不够","你的权限不够") ;
    }
}

void Widget::savePic(QString pic_name, QImage &current_image)
{
    QFile current_file  ;
    qDebug()<<"saving pic" ;
    QString pic_file_name = "/home/liao/Documents/classroom_info//"+pic_name+".png" ;
    current_file.setFileName(pic_file_name);
    current_image.save(current_file.fileName()) ;
}

void PicSaveThread::run()
{
    if(this->current_image_.isNull())return ;
    QFile current_file  ;
    QString pic_file_name = "/home/liao/Documents/classroom_info//"+this->file_name_+".png" ;
    current_file.setFileName(pic_file_name);
    this->current_image_.save(current_file.fileName()) ;
}

void PicSaveThread::setPicInfo(QString file_name, QImage &img)
{
    this->file_name_ = file_name ;
    this->current_image_ = img ;
}

PicSaveThread::PicSaveThread(QString file_name , QImage &current_image)
{
    this->file_name_ = file_name ;
    this->current_image_ = current_image ;
}

PicSaveThread::PicSaveThread()
{

}

 PicSaveThread::~PicSaveThread()
{

}

 void Widget::on_pushButton_operate_clicked()
 {

 }

void Widget::on_pushButton_user_clicked()
{
    this->classroom_tree_view_->refreshPeopleNum();
}

#include "database.h"
#include <QDebug>

DataBase::DataBase(QWidget *parent) : QWidget(parent)
{

}

//single model
QStringList DataBase::getTablesName()
{
    return db.tables() ;
}

DataBase* DataBase::database_ptr_ = NULL ;

DataBase* DataBase::getDataBasePtr()
{
    static QMutex database_mutex_ ;
    if(database_ptr_==NULL)
    {
        QMutexLocker locker(&database_mutex_) ;
        if(database_ptr_==NULL)
        {
            database_ptr_ = new DataBase() ;
            database_ptr_->getDataBaseInit() ;
        }
    }
    return database_ptr_ ;
}

bool DataBase::getDataBaseInit()
{
    db = QSqlDatabase::addDatabase("QMYSQL") ;
    db.setHostName("localhost");
    db.setDatabaseName("student");
    db.setUserName("root");
    db.setPassword("666666");
    if(!db.open())
    {
        qDebug()<<"can not open the database" ;
        return false ;
    }
    else
    {
        qDebug()<<"open the database successed!" ;
        return true ;
    }
}

bool DataBase::isDatabaseOpen()
{
    return db.isOpen() ;

}

DataBase::~DataBase()
{

}

//check login
bool DataBase::isUserLogin(QString user_name, QString user_password)
{
    QSqlQuery query ;
    QString current_query = QString("call login('%0','%1')").arg(user_name).arg(user_password) ;
    qDebug()<<current_query ;
    query.exec(current_query) ;
    qDebug()<<query.next() ;
    int num =  query.value(0).toInt() ;
    qDebug()<<num ;
    query.clear();
    if(num==1)
    {
        QString current_query = QString("select role from user_login where user_id='%0'").arg(user_name) ;
        query.exec(current_query) ;
        query.next() ;
        GlobalFile::CURRENT_USER_ROLE = query.value(0).toInt() ;
        query.clear();
    }
    return num==0?false:true ;
}

//add a new user of this app
bool DataBase::addNewUser(QString user_name, QString user_password, int role)
{
    QString add_new_user = QString("call create_user('%0','%1',%2)")
            .arg(user_name).arg(user_password).arg(role) ;
    QSqlQuery query  ;
    query.exec(add_new_user) ;
    query.next() ;
    int result = query.value(0).toInt() ;
    qDebug()<<result;
    if(result==0)return false ;
    else return true ;
}

//reset the classroom status
//such as 0 ---- means nothing happen in this room 
//        1 ---- means exam or lesson happen in this room
void DataBase::updateClassRoom(QString classroom_name, int classroom_status)
{
    QSqlQuery query ;
    QString update_classroom = QString("call update_classroom_info('%0',%1)")
            .arg(classroom_name).arg(classroom_status) ;
    qDebug()<<"update classroom";
    query.exec(update_classroom) ;
}

//get current people num according to the classroom_name
int DataBase::getPeopleNum(QString classroom_name)
{
    QSqlQuery query ;
    QString select_people = QString("select current_people from classroom_info where classroom_name='%0'")
            .arg(classroom_name) ;
    query.exec(select_people) ;
    query.next() ;
    int people_num = query.value(0).toInt() ;
    query.clear();
    return people_num ;
}

//update the people num according to the classroom
void DataBase::updatePeopleNum(QString classroom_name, int people_num)
{
    QSqlQuery query ;
    QString update_people = QString("update classroom_info set current_people = %0 where classroom_name='%1'")
            .arg(people_num).arg(classroom_name) ;
    query.exec(update_people) ;
}

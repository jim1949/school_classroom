#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMutex>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <global_file.h>

class DataBase : public QWidget
{
    Q_OBJECT
public:
    static DataBase* getDataBasePtr() ;
    bool isDatabaseOpen()  ;
    bool getDataBaseInit() ;
    QStringList getTablesName() ;
    bool isUserLogin(QString user_name , QString user_password) ;
    bool addNewUser(QString user_name , QString user_password , int role) ;
    void updateClassRoom(QString classroom_name , int classroom_status) ;
    int getPeopleNum(QString classroom_name) ;
    void updatePeopleNum(QString classroom_name , int people_num) ;
private:
    explicit DataBase(QWidget *parent = 0);
    ~DataBase() ;
signals:

public slots:

private:
    static DataBase *database_ptr_ ;
    QSqlDatabase db ;
//    static GlobalFile global_num_ ;
};

#endif // DATABASE_H
